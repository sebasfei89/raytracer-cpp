#pragma once

#include "BaseTestCase.h"
#include "ColoredOutput.h"
#include "Expression.h"
#include "TestCase.h"

#include <algorithm>
#include <functional>

namespace beddev
{

template<typename PARAM_T>
class ParametrizedInstance : public TestCase
{
public:
    using RunCallback = std::function<bool()>;

    ParametrizedInstance();
    ParametrizedInstance(std::string const& desc, std::string const& file, long line, std::string const& category, PARAM_T param, RunCallback const& runCallback);

    PARAM_T const& GetParam() const { return m_param; }

    bool RunImpl() override { return m_runCallback(); }

    void FormatHeader(std::ostream& os) const override;

    template<typename U = PARAM_T>
    typename std::enable_if<is_printable<U>::value, std::string>::type GetParamStr() const
    {
        std::ostringstream oss;
        oss << m_param;
        return oss.str();
    }

    template<typename U = PARAM_T>
    typename std::enable_if_t<!is_printable<U>::value, std::string> GetParamStr() const { return "??"; }

    void FormatAssertionMessage(std::ostream& os, bool assertionResult) const;

    using TestCase::FormatHeader;
    using TestCase::AddFact;
    using TestCase::AddAction;
    using TestCase::AddTest;
    using TestCase::ReportFailure;
    using TestCase::SummarizeAssertions;
    using TestCase::RunInternal;

private:
    PARAM_T m_param;
    RunCallback m_runCallback;
};

template<typename PARAM_T>
class ParametrizedTestCase : public BaseTestCase
{
public:
    ParametrizedTestCase(std::string const& desc, std::string const& file, long line, std::string const& category = "");

protected:
    enum class ERunStep { REGISTER_TEST_PARAMS, RUN_TEST };

    virtual bool RunImpl(ERunStep runStep) = 0;

    PARAM_T const& GetParam() const { return RunningInstance().GetParam(); }
    void SetParams(std::vector<PARAM_T> const& params);

    void RunInternal(std::ostream& os, SessionSummary& summary, bool outputOnSuccess) override;

    void AddFact(std::string const& fact) override     { RunningInstance().AddFact(fact); }
    void AddAction(std::string const& action) override { RunningInstance().AddAction(action); }
    bool AddTest(Assertion const& assertion) override  { return RunningInstance().AddTest(assertion); }

    void ReportFailure(std::ostream& os, bool outputOnSuccess) const override { RunningInstance().ReportFailure(os, outputOnSuccess); }

private:
    using PTestInstance = ParametrizedInstance<PARAM_T>;
    PTestInstance const& RunningInstance() const { return m_testInstances[m_currentInstance]; }
    PTestInstance& RunningInstance() { return m_testInstances[m_currentInstance]; }

    std::vector<PTestInstance> m_testInstances;
    uint32_t m_currentInstance;
};

template<typename PARAM_T>
ParametrizedTestCase<PARAM_T>::ParametrizedTestCase( std::string const& desc, std::string const& file
                                                   , long line, std::string const& category )
    : BaseTestCase(desc, file, line, category)
    , m_testInstances()
    , m_currentInstance(0)
{
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::SetParams(std::vector<PARAM_T> const& params)
{
    auto instanceFactory = [this](PARAM_T const& p)
    {
        auto instanceRunner = [this]() { return RunImpl(ERunStep::RUN_TEST); };
        return PTestInstance(GetDescription(), GetFile(), GetLine(), GetTags(), p, instanceRunner);
    };

    m_testInstances.resize(params.size());
    std::transform(params.begin(), params.end(), m_testInstances.begin(), instanceFactory);
}

template<typename PARAM_T>
void ParametrizedTestCase<PARAM_T>::RunInternal(std::ostream& os, SessionSummary& summary, bool outputOnSuccess)
{
    RunImpl(ERunStep::REGISTER_TEST_PARAMS);
    if (m_testInstances.size() == 0)
    {
        ReportConfigFailure(os, summary, "No parameters provided to a parametrized test case");
        return;
    }

    m_currentInstance = 0;
    for (auto& testInstance : m_testInstances)
    {
        testInstance.RunInternal(os, summary, outputOnSuccess);
        m_currentInstance++;
    }
}

template<typename PARAM_T>
ParametrizedInstance<PARAM_T>::ParametrizedInstance()
    : TestCase("", "", 0)
{
}

template<typename PARAM_T>
ParametrizedInstance<PARAM_T>::ParametrizedInstance(std::string const& desc, std::string const& file, long line, std::string const& category, PARAM_T param, RunCallback const& runCallback)
    : TestCase(desc, file, line, category)
    , m_param(param)
    , m_runCallback(runCallback)
{
}

template<typename PARAM_T>
void ParametrizedInstance<PARAM_T>::FormatHeader(std::ostream& os) const
{
    TestCase::FormatHeader(os);
    os << "   Param: " << GetParamStr() << std::endl;
}

template<typename PARAM_T>
void ParametrizedInstance<PARAM_T>::FormatAssertionMessage(std::ostream& os, bool assertionResult) const
{
    if (assertionResult)
    {
        os << TestingImpl::Colour(TestingImpl::ColorCode::Success) << "SUCCEEDED with argument [" << GetParamStr() << "]:";
    }
    else
    {
        os << TestingImpl::Colour(TestingImpl::ColorCode::Error) << "FAILED with argument [" << GetParamStr() << "]:";
    }
    os << std::endl;
}

}
