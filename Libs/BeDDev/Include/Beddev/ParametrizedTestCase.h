#pragma once

#include "BaseTestCase.h"
#include "Expression.h"
#include "SessionSummary.h"
#include "TestCase.h"

#include <cassert>
#include <iomanip>
#include <unordered_map>

namespace beddev
{

template<typename PARAM_T>
class ParametrizedTestCase : public BaseTestCase
{
public:
    ParametrizedTestCase(std::string const& desc, std::string const& file, long line, std::string const& category = "")
        : BaseTestCase(desc, file, line, category)
        , m_testInstances()
        , m_currentInstance(0)
    {
    }

protected:
    enum class ERunStep
    {
        REGISTER_TEST_PARAMS,
        RUN_TEST
    };

    virtual bool RunImpl(ERunStep runStep) = 0;

    PARAM_T const& GetParam() const { return RunningInstance().GetParam(); }
    void SetParams(std::vector<PARAM_T> const& params)
    {
        for (auto const& param : params)
        {
            m_testInstances.push_back(TestInstance(this, param));
        }
    }

    void RunInternal(std::ostream& os, SessionSummary& summary) override
    {
        RunImpl(ERunStep::REGISTER_TEST_PARAMS);
        if (m_testInstances.size() == 0)
        {
            FormatHeader(os);
            FormatFileAndLineInfo(os);
            os << std::setw(80) << std::setfill('.') << "." << std::endl << std::setfill(' ') << std::endl;
            os << "  No parameters provided to a parametrized test case" << std::endl << std::endl;
            summary.failed++;
            summary.nonAssertionFailures++;
            return;
        }

        m_currentInstance = 0;
        for (auto& testInstance : m_testInstances)
        {
            testInstance.RunInternal(os, summary);
            m_currentInstance++;
        }
    }

    void AddFact(std::string const& fact) override
    {
        RunningInstance().AddFact(fact);
    }

    void AddAction(std::string const& action) override
    {
        RunningInstance().AddAction(action);
    }

    bool AddTest(std::string const& test, std::string const& file, long line, IExpression const& expr)
    {
        std::ostringstream oss;
        oss << "FAILED with argument [" << RunningInstance().GetParamStr() << "]:";
        return AddTest({ test, file, line, expr.Succeeded(), expr.ExpandedExpression(), oss.str() });
    }

    bool AddTest(Assertion const& assertion)
    {
        RunningInstance().AddTest(assertion);
        return assertion.result;
    }

    void ReportFailure(std::ostream& os) const override
    {
        RunningInstance().ReportFailure(os);
    }

private:
    class TestInstance : public TestCase
    {
    public:
        TestInstance(ParametrizedTestCase<PARAM_T>* testCase, PARAM_T param)
            : TestCase(testCase->GetDescription(), testCase->GetFile(), testCase->GetLine(), testCase->GetCategory())
            , m_testCase(testCase)
            , m_param(param)
        {}

        PARAM_T const& GetParam() const { return m_param; }

        bool RunImpl() override { return m_testCase->RunImpl(ERunStep::RUN_TEST); }

        void FormatHeader(std::ostream& os) const override
        {
            TestCase::FormatHeader(os);
            os << "   Param: " << GetParamStr() << std::endl;
        }

        template<typename U = PARAM_T>
        typename std::enable_if<is_printable<U>::value, std::string>::type
        GetParamStr() const
        {
            std::ostringstream oss;
            oss << m_param;
            return oss.str();
        }

        template<typename U = PARAM_T>
        typename std::enable_if_t<!is_printable<U>::value, std::string>
        GetParamStr() const { return "??"; }

        ParametrizedTestCase<PARAM_T>* m_testCase;
        PARAM_T m_param;

        using TestCase::FormatHeader;
        using TestCase::AddFact;
        using TestCase::AddAction;
        using TestCase::AddTest;
        using TestCase::ReportFailure;
        using TestCase::SummarizeAssertions;
        using TestCase::RunInternal;
    };

    TestInstance const& RunningInstance() const
    {
        assert(m_currentInstance < m_testInstances.size());
        return m_testInstances[m_currentInstance];
    }

    TestInstance& RunningInstance()
    {
        assert(m_currentInstance < m_testInstances.size());
        return m_testInstances[m_currentInstance];
    }

    std::vector<TestInstance> m_testInstances;
    uint32_t m_currentInstance;
};

}
