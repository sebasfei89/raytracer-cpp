#pragma once

#include "beddev_export.h"
#include "ColoredOutput.h"
#include "Expression.h"
#include "TestRunner.h"

#include <functional>
#include <iomanip>
#include <numeric>
#include <ostream>
#include <string>
#include <vector>

namespace beddev
{

class ITestCase
{
public:
    virtual bool Run(std::ostream& os) = 0;

    using AssertionsSummary = std::tuple<uint32_t, uint32_t>; // <passed, failed>
    virtual AssertionsSummary Assertions() const = 0;
};

class IExpression;

struct NoParam {};

template<typename PARAM_T>
class ParametrizedTestCase : public ITestCase
{
public:
    ParametrizedTestCase(std::string const& desc, std::string const& fileAndLine, std::string const& category = "")
        : m_description(desc)
        , m_category(category)
        , m_fileAndLine(fileAndLine)
        , m_facts()
        , m_actions()
        , m_tests()
        , m_params()
        , m_currentParam(0)
    {
        TestRunner::Get().Register(this);
    }

    bool Run(std::ostream& os) override;

    AssertionsSummary Assertions() const override;

    PARAM_T const& GetParam() { return m_params[m_currentParam]; }

    struct TestAssertion
    {
        std::string test;
        std::string file;
        long line;
        bool result;
        std::string expanded;
        PARAM_T param;
    };

protected:
    virtual bool RunImpl() = 0;

    void SetParams(std::vector<PARAM_T> const& params)
    {
        m_params = params;
    }

    template<typename U = PARAM_T>
    typename std::enable_if_t<std::is_same_v<U, NoParam>, bool>
    RunInternal() { return RunImpl(); }

    template<typename U = PARAM_T>
    typename std::enable_if_t<!std::is_same_v<U, NoParam>, bool>
    RunInternal()
    {
        bool succeed = true;
        for (m_currentParam = 0; m_currentParam < m_params.size(); m_currentParam++)
        {
            succeed &= RunImpl();
        }
        return succeed;
    }

    void AddFact(std::string const& fact) { m_facts.push_back(fact); }
    void AddAction(std::string const& action) { m_actions.push_back(action); }

    template<typename U = PARAM_T>
    typename std::enable_if_t<std::is_same_v<U, NoParam>, bool>
    AddTest(std::string const& test, std::string const& file, long line, IExpression const& expr)
    {
        return AddTest({ test, file, line, expr.Succeeded(), expr.ExpandedExpression(), NoParam{} });
    }

    template<typename U = PARAM_T>
    typename std::enable_if_t<!std::is_same_v<U, NoParam>, bool>
    AddTest(std::string const& test, std::string const& file, long line, IExpression const& expr)
    {
        return AddTest({ test, file, line, expr.Succeeded(), expr.ExpandedExpression(), GetParam() });
    }

    bool AddTest(TestAssertion const& assertion)
    {
        m_tests.push_back(assertion);
        return assertion.result;
    }

    std::vector<TestAssertion> const& GetTests() const { return m_tests; }

    void ReportFailure(std::ostream& os) const;
    void FormatHeader(std::ostream& os) const;
    void FormatItems(std::ostream& os, std::vector<std::string> const& items, std::string const& tag) const;
    void FormatItems(std::ostream& os, std::vector<TestAssertion> const& items, std::string const& tag) const;
    void FormatFileAndLineInfo(std::ostream& os) const;
    void FormatFailedTest(std::ostream& os, std::string const& file, long line, std::string const& expr, std::string const& expandedExpr, PARAM_T const& param) const;

    template<typename U = PARAM_T>
    typename std::enable_if<is_printable<U>::value, std::string>::type
    FailedArgumentStr(U const& param) const
    {
        std::ostringstream oss;
        oss << " with argument [" << param << "]";
        return oss.str();
    }

    template<typename U = PARAM_T>
    typename std::enable_if_t<!is_printable<U>::value, std::string>
    FailedArgumentStr(U const& param) const
    {
        std::ostringstream oss;
        oss << " with argument [??]";
        return oss.str();
    }

    template<typename U = PARAM_T>
    typename std::enable_if_t<std::is_same_v<U, NoParam>, std::string>
    FailedArgumentStr(NoParam const& param) const
    {
        return "";
    }

private:
    std::string m_description;
    std::string m_category;
    std::string m_fileAndLine;
    std::vector<std::string> m_facts;
    std::vector<std::string> m_actions;
    std::vector<TestAssertion> m_tests;
    std::vector<PARAM_T> m_params;
    uint32_t m_currentParam;
};

template<typename PARAM_T>
bool ParametrizedTestCase<PARAM_T>::Run(std::ostream& os)
{
    bool succeed = false;
    try
    {
        succeed = RunInternal();
    }
    catch (std::exception const&)
    {
        succeed = false;
    }
    catch (...)
    {
        succeed = false;
    }

    if (!succeed)
    {
        ReportFailure(os);
    }

    return succeed;
}

template<typename PARAM_T>
typename ParametrizedTestCase<PARAM_T>::AssertionsSummary ParametrizedTestCase<PARAM_T>::Assertions() const
{
    return std::reduce(m_tests.begin(), m_tests.end(), AssertionsSummary{ 0, 0 }, [](auto& summary, auto const& a)
    {
        if (a.result)
        {
            std::get<0>(summary)++;
        }
        else
        {
            std::get<1>(summary)++;
        }
        return summary;
    });
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::FormatHeader(std::ostream& os) const
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
        os << std::setw(8) << (isFirst ? tag : "And") << ": " << i.test << std::endl;
        isFirst = false;
    }
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::FormatFileAndLineInfo(std::ostream& os) const
{
    os << std::setw(80) << std::setfill('-') << "-" << std::endl << std::setfill(' ');
    os << TestingImpl::Colour(TestingImpl::ColorCode::FileName) << m_fileAndLine << std::endl;
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::FormatFailedTest(std::ostream& os, std::string const& file, long line, std::string const& expr, std::string const& expandedExpr, PARAM_T const& param) const
{
    os << std::setw(80) << std::setfill('.') << "." << std::endl << std::setfill(' ') << std::endl
        << TestingImpl::Colour(TestingImpl::ColorCode::FileName) << file << "(" << line << "): "
        << TestingImpl::Colour(TestingImpl::ColorCode::Error) << "FAILED" << FailedArgumentStr(param) << ":" << std::endl
        << TestingImpl::Colour(TestingImpl::ColorCode::OriginalExpression) << "  " << expr << std::endl;
    if (expandedExpr != expr)
    {
        os << TestingImpl::Colour(TestingImpl::ColorCode::White) << "with expansion:" << std::endl
           << TestingImpl::Colour(TestingImpl::ColorCode::ReconstructedExpression) << "  " << expandedExpr << std::endl;
    }
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::ReportFailure(std::ostream& os) const
{
    FormatHeader(os);
    FormatItems(os, m_facts, "Given");
    FormatItems(os, m_actions, "When");
    FormatItems(os, m_tests, (m_facts.size() > 0 || m_actions.size() > 0) ? "Then" : "Require");
    FormatFileAndLineInfo(os);

    for (auto const& a : m_tests)
    {
        if (a.result) continue;
        FormatFailedTest(os, a.file, a.line, a.test, a.expanded, a.param);
    }
}

using TestCase = ParametrizedTestCase<NoParam>;

} // EON beddev
