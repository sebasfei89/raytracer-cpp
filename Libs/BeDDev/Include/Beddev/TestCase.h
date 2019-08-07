#pragma once

#include "beddev_export.h"
#include "ColoredOutput.h"
#include "Expression.h"
#include "SessionSummary.h"
#include "TestRunner.h"

#include <functional>
#include <iomanip>
#include <numeric>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace beddev
{

class SessionSummary;

class ITestCase
{
public:
    virtual void Run(std::ostream& os, SessionSummary& summary) = 0;

    virtual std::string const& GetDescription() const = 0;
};

class IExpression;

struct NoParam {};

template<typename PARAM_T>
class ParametrizedTestCase : public ITestCase
{
public:
    ParametrizedTestCase(std::string const& desc, std::string const& file, long line, std::string const& category = "")
        : m_description(desc)
        , m_category(category)
        , m_file(file)
        , m_line(line)
        , m_facts()
        , m_actions()
        , m_tests()
        , m_params()
        , m_currentParam(0)
    {
        TestRunner::Get().Register(this);
    }

    std::string const& GetDescription() const override
    {
        return m_description;
    }

    void Run(std::ostream& os, SessionSummary& summary) override;

    PARAM_T const& GetParam() const { return m_params[m_currentParam]; }

    struct TestAssertion
    {
        std::string test;
        std::string file;
        long line;
        bool result;
        bool isAssertion;
        std::string expanded;
        PARAM_T param;
    };

protected:
    enum class ERunStep
    {
        REGISTER_TEST_PARAMS,
        RUN_TEST
    };

    virtual bool RunImpl(ERunStep runStep) = 0;

    void SetParams(std::vector<PARAM_T> const& params)
    {
        m_params = params;
    }

    template<typename U = PARAM_T>
    typename std::enable_if_t<std::is_same_v<U, NoParam>, void>
    RunInternal(std::ostream& os, SessionSummary& summary)
    {
        if (RunImpl(ERunStep::RUN_TEST))
        {
            summary.passed++;
        }
        else
        {
            ReportFailure(os);
            summary.failed++;
        }

        for (auto const& assertion : m_tests[0])
        {
            if (!assertion.isAssertion)
            {
                summary.nonAssertionFailures++;
            }
            else if (assertion.result)
            {
                summary.passedAssertions++;
            }
            else
            {
                summary.failedAssertions++;
            }
        }
    }

    template<typename U = PARAM_T>
    typename std::enable_if_t<!std::is_same_v<U, NoParam>, void>
    RunInternal(std::ostream& os, SessionSummary& summary)
    {
        RunImpl(ERunStep::REGISTER_TEST_PARAMS);
        if (m_params.size() == 0)
        {
            m_tests[0].push_back({ "No parameters provided to a parametrized test case", m_file, m_line, false, false, "", PARAM_T{} });
            ReportFailure(os);
            summary.failed++;
            summary.nonAssertionFailures++;
            return;
        }

        for (m_currentParam = 0; m_currentParam < m_params.size(); m_currentParam++)
        {
            if (RunImpl(ERunStep::RUN_TEST))
            {
                summary.passed++;
            }
            else
            {
                ReportFailure(os);
                summary.failed++;
            }

            for (auto const& assertion : m_tests[m_currentParam])
            {
                if (!assertion.isAssertion)
                {
                    summary.nonAssertionFailures++;
                }
                else if (assertion.result)
                {
                    summary.passedAssertions++;
                }
                else
                {
                    summary.failedAssertions++;
                }
            }
        }
    }

    void AddFact(std::string const& fact) { m_facts[m_currentParam].push_back(fact); }
    void AddAction(std::string const& action) { m_actions[m_currentParam].push_back(action); }

    template<typename U = PARAM_T>
    typename std::enable_if_t<std::is_same_v<U, NoParam>, bool>
    AddTest(std::string const& test, std::string const& file, long line, IExpression const& expr)
    {
        return AddTest({ test, file, line, expr.Succeeded(), true, expr.ExpandedExpression(), NoParam{} });
    }

    template<typename U = PARAM_T>
    typename std::enable_if_t<!std::is_same_v<U, NoParam>, bool>
    AddTest(std::string const& test, std::string const& file, long line, IExpression const& expr)
    {
        return AddTest({ test, file, line, expr.Succeeded(), true, expr.ExpandedExpression(), GetParam() });
    }

    bool AddTest(TestAssertion const& assertion)
    {
        m_tests[m_currentParam].push_back(assertion);
        return assertion.result;
    }

    void ReportFailure(std::ostream& os);
    void FormatHeader(std::ostream& os) const;
    void FormatItems(std::ostream& os, std::vector<std::string> const& items, std::string const& tag) const;
    void FormatItems(std::ostream& os, std::vector<TestAssertion> const& items, std::string const& tag) const;
    void FormatFileAndLineInfo(std::ostream& os) const;
    void FormatFailedTest(std::ostream& os, bool isAssertion, std::string const& file, long line, std::string const& expr, std::string const& expandedExpr, PARAM_T const& param) const;

    template<typename U = PARAM_T>
    typename std::enable_if<is_printable<U>::value, std::string>::type
    GetParamStr(U const& param) const
    {
        std::ostringstream oss;
        oss << param;
        return oss.str();
    }

    template<typename U = PARAM_T>
    typename std::enable_if_t<!is_printable<U>::value, std::string>
    GetParamStr(U const& param) const
    {
        return "??";
    }

    std::string FailedArgumentStr(PARAM_T const& param) const;

private:
    std::string m_description;
    std::string m_category;
    std::string m_file;
    long m_line;

    using TestElements = std::unordered_map<uint32_t, std::vector<std::string>>;
    using TestAssertions = std::unordered_map<uint32_t, std::vector<TestAssertion>>;

    TestElements m_facts;
    TestElements m_actions;
    TestAssertions m_tests;
    std::vector<PARAM_T> m_params;
    uint32_t m_currentParam;
};

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::Run(std::ostream& os, SessionSummary& summary)
{
    try
    {
        RunInternal(os, summary);
    }
    catch (std::exception const&)
    {
        // TODO: summary.nonAssertionFailures++;
        // ReportFailure(os);
    }
    catch (...)
    {
        // TODO: summary.nonAssertionFailures++;
        // ReportFailure(os);
    }
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::FormatHeader(std::ostream& os) const
{
    os << std::setw(80) << std::setfill('-') << "-" << std::endl << std::setfill(' ');
    os << "Scenario: " << m_description;
    if (!m_category.empty()) os << "  [" << m_category << "]";
    os << std::endl;
    if (m_params.size() > 0)
    {
        os << "   Param: " << GetParamStr(GetParam()) << std::endl;
    }
}

template<>
inline void ParametrizedTestCase<NoParam>::FormatHeader(std::ostream& os) const
{
    os << std::setw(80) << std::setfill('-') << "-" << std::endl << std::setfill(' ');
    os << "Scenario: " << m_description;
    if (!m_category.empty()) os << "  [" << m_category << "]";
    os << std::endl;
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::FormatItems(std::ostream& os, std::vector<std::string> const& items, std::string const& tag) const
{
    bool isFirst = true;
    for (auto const& i : items)
    {
        os << std::setw(8) << (isFirst ? tag : "And") << ": " << i << std::endl;
        isFirst = false;
    }
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::FormatItems(std::ostream& os, std::vector<TestAssertion> const& items, std::string const& tag) const
{
    bool isFirst = true;
    for (auto const& i : items)
    {
        if (!i.isAssertion) continue;
        os << std::setw(8) << (isFirst ? tag : "And") << ": " << i.test << std::endl;
        isFirst = false;
    }
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::FormatFileAndLineInfo(std::ostream& os) const
{
    os << std::setw(80) << std::setfill('-') << "-" << std::endl << std::setfill(' ');
    os << TestingImpl::Colour(TestingImpl::ColorCode::FileName) << m_file << "(" << m_line << ")" << std::endl;
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::FormatFailedTest(std::ostream& os, bool isAssertion, std::string const& file, long line, std::string const& expr, std::string const& expandedExpr, PARAM_T const& param) const
{
    os << std::setw(80) << std::setfill('.') << "." << std::endl << std::setfill(' ') << std::endl;
    if (!isAssertion)
    {
        os << "  " << expr << std::endl << std::endl;
        return;
    }

    os << TestingImpl::Colour(TestingImpl::ColorCode::FileName) << file << "(" << line << "): "
        << TestingImpl::Colour(TestingImpl::ColorCode::Error) << "FAILED" << FailedArgumentStr(param) << ":" << std::endl
        << TestingImpl::Colour(TestingImpl::ColorCode::OriginalExpression) << "  " << expr << std::endl;
    if (expandedExpr != expr)
    {
        os << TestingImpl::Colour(TestingImpl::ColorCode::White) << "with expansion:" << std::endl
           << TestingImpl::Colour(TestingImpl::ColorCode::ReconstructedExpression) << "  " << expandedExpr << std::endl;
    }
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::ReportFailure(std::ostream& os)
{
    FormatHeader(os);
    FormatItems(os, m_facts[m_currentParam], "Given");
    FormatItems(os, m_actions[m_currentParam], "When");
    FormatItems(os, m_tests[m_currentParam], (m_facts[m_currentParam].size() > 0 || m_actions[m_currentParam].size() > 0) ? "Then" : "Require");
    FormatFileAndLineInfo(os);

    auto const testInstance = m_tests[m_currentParam];
    for (auto const& a : testInstance)
    {
        if (a.result) continue;
        FormatFailedTest(os, a.isAssertion, a.file, a.line, a.test, a.expanded, a.param);
    }
}

template<typename PARAM_T>
std::string ParametrizedTestCase<PARAM_T>::FailedArgumentStr(PARAM_T const& param) const
{
    std::ostringstream oss;
    oss << " with argument [" << GetParamStr(param) << "]";
    return oss.str();
}

template<>
inline std::string ParametrizedTestCase<NoParam>::FailedArgumentStr(NoParam const& param) const
{
    return "";
}

using TestCase = ParametrizedTestCase<NoParam>;

} // EON beddev
