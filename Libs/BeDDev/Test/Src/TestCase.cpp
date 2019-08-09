#include <Beddev/Beddev.h>
#include <Beddev/SessionSummary.h>

#include <sstream>

using namespace beddev;

namespace
{

#define DUMMY_TESTCASE(...) DummyTestCase(__FILE__, __LINE__, __VA_ARGS__)
#define TEST_ASSERTION(TC, EXPR) TC.AddTestHelper({#EXPR, __FILE__, __LINE__, ExpressionParser::Get() <= EXPR})

class DummyTestCase : public TestCase
{
public:
    DummyTestCase(std::string const& f, long l, std::string const& cat="")
        : TestCase("DummyTestCase", f, l, cat)
        , m_succeed(true)
    {}

    bool RunImpl() override { return m_succeed; }

    void AddTestHelper(Assertion const& assertion)
    {
        AddTest(assertion);
        m_succeed &= assertion.result;
    }

    std::string GetExpectedOutput()
    {
        std::ostringstream oss;
        FormatHeader(oss);
        ReportFailure(oss, false);
        return oss.str();
    }

private:
    bool m_succeed;
};

}

SCENARIO("An empty test case")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss
         , beddev::SessionSummary ss )
    WHEN( tc.Run(oss, ss) )
    THEN( ss.passed == 1
        , ss.failed == 0
        , ss.passedAssertions == 0
        , ss.failedAssertions == 0
        , oss.str() == "" )
}

SCENARIO("A test case with a single assertion that passes")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss
         , beddev::SessionSummary ss )
    WHEN( TEST_ASSERTION(tc, 1 + 2 == 3)
        , tc.Run(oss, ss) )
    THEN( ss.passed == 1
        , ss.failed == 0
        , ss.passedAssertions == 1
        , ss.failedAssertions == 0
        , oss.str() == "")
}

SCENARIO("A test case with multiple assertions that passes")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss
         , beddev::SessionSummary ss )
    WHEN( TEST_ASSERTION(tc, 1 + 2 == 3)
        , TEST_ASSERTION(tc, 3 + 4 == 7)
        , tc.Run(oss, ss) )
    THEN( ss.passed == 1
        , ss.failed == 0
        , ss.passedAssertions == 2
        , ss.failedAssertions == 0
        , oss.str() == "")
}

SCENARIO("A test case with a single assertion that fails")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss
         , beddev::SessionSummary ss )
    WHEN( TEST_ASSERTION(tc, 1 + 2 == 4)
        , tc.Run(oss, ss) )
    THEN( ss.passed == 0
        , ss.failed == 1
        , ss.passedAssertions == 0
        , ss.failedAssertions == 1
        , oss.str() == tc.GetExpectedOutput() )
}

SCENARIO("A test case with multiple assertions")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss
         , beddev::SessionSummary ss )
    WHEN( TEST_ASSERTION(tc, 1 + 2 == 4)
        , TEST_ASSERTION(tc, 3 + 4 == 6)
        , TEST_ASSERTION(tc, 5 + 6 == 11)
        , tc.Run(oss, ss) )
    THEN( ss.passed == 0
        , ss.failed == 1
        , ss.passedAssertions == 1
        , ss.failedAssertions == 2
        , oss.str() == tc.GetExpectedOutput() )
}
