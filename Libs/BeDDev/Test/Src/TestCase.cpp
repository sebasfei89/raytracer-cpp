#include <Beddev/Beddev.h>

#include <sstream>

using namespace beddev;

namespace
{

#define DUMMY_TESTCASE(...) DummyTestCase(__FILE__, __LINE__, __VA_ARGS__)
#define TEST_ASSERTION(TC, EXPR) TC.AddTestHelper(#EXPR, __FILE__, __LINE__, ExpressionParser::Get() <= EXPR)

class DummyTestCase : public TestCase
{
public:
    DummyTestCase(std::string const& f, long l, std::string const& cat="")
        : TestCase("DummyTestCase", f, l, cat)
        , m_succeed(true)
    {}

    bool RunImpl(ERunStep runStep) override { return m_succeed; }

    void AddTestHelper(std::string const& test, std::string const& file, long line, IExpression const& expr)
    {
        AddTest(test, file, line, expr);
        m_succeed &= expr.Succeeded();
    }

    std::string GetExpectedOutput() const
    {
        std::ostringstream oss;
        ReportFailure(oss);
        return oss.str();
    }

private:
    bool m_succeed;
};

}

SCENARIO("An empty test case")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss )
    WHEN( tc.Run(oss)
        , auto tests = tc.Assertions() )
    THEN( std::get<0>(tests) == 0
        , std::get<1>(tests) == 0
        , oss.str() == "" )
}

SCENARIO("A test case with a single assertion that passes")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss )
    WHEN( TEST_ASSERTION(tc, 1 + 2 == 3)
        , auto const succeed = tc.Run(oss)
        , auto const tests = tc.Assertions() )
    THEN( succeed
        , std::get<0>(tests) == 1
        , std::get<1>(tests) == 0
        , oss.str() == "")
}

SCENARIO("A test case with multiple assertions that passes")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss)
    WHEN( TEST_ASSERTION(tc, 1 + 2 == 3)
        , TEST_ASSERTION(tc, 3 + 4 == 7)
        , auto const succeed = tc.Run(oss)
        , auto const tests = tc.Assertions())
    THEN( succeed
        , std::get<0>(tests) == 2
        , std::get<1>(tests) == 0
        , oss.str() == "")
}

SCENARIO("A test case with a single assertion that fails")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss )
    WHEN( TEST_ASSERTION(tc, 1 + 2 == 4)
        , bool const succeed = tc.Run(oss)
        , auto const tests = tc.Assertions() )
    THEN( !succeed
        , std::get<0>(tests) == 0
        , std::get<1>(tests) == 1
        , oss.str() == tc.GetExpectedOutput() )
}

SCENARIO("A test case with multiple assertions")
{
    GIVEN( auto tc = DUMMY_TESTCASE()
         , std::ostringstream oss )
    WHEN( TEST_ASSERTION(tc, 1 + 2 == 4)
        , TEST_ASSERTION(tc, 3 + 4 == 6)
        , TEST_ASSERTION(tc, 5 + 6 == 11)
        , bool const succeed = tc.Run(oss)
        , auto const tests = tc.Assertions() )
    THEN( !succeed
        , std::get<0>(tests) == 1
        , std::get<1>(tests) == 2
        , oss.str() == tc.GetExpectedOutput() )
}
