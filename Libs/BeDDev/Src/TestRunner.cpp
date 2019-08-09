#include "TestRunner.h"

#include "SessionSummary.h"
#include "TestCase.h"

#ifdef _WIN32
#   include <crtdbg.h>
#endif

namespace beddev
{

// static
TestRunner& TestRunner::Get()
{
    static TestRunner s_testRunner;
    return s_testRunner;
}

TestRunner::TestRunner()
    : m_testCases()
    , m_isRunning(false)
{
#ifdef _WIN32
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif
}

TestRunner::~TestRunner()
{
    m_testCases.clear();
}

void TestRunner::Register(ITestCase* testCase)
{
    if (!m_isRunning)
    {
        m_testCases.push_back(testCase);
    }
}

int TestRunner::RunAll(std::ostream& os, Config const& config)
{
    m_isRunning = true;

    SessionSummary summary;
    for (auto const& test : m_testCases)
    {
        if (test->CanRun(config.m_filter, config.m_tags))
        {
            test->Run(os, summary, config.m_fullReport);
        }
        else
        {
            summary.skipped++;
        }
    }

    summary.Print(os);

    m_isRunning = false;
    return summary.failed;
}

} // EON beddev
