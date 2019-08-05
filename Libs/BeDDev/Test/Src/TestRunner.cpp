#include <Beddev/Beddev.h>

#include <sstream>

class DummyTestRunner : public beddev::TestRunner {};

class EmptyTestCase : public beddev::TestCase
{
public:
    EmptyTestCase(std::string const& desc, std::string const& fileAndLine, std::string const& category = "")
        : beddev::TestCase(desc, fileAndLine, category) {}

    bool RunImpl() override
    {
        bool result = true;
        for (auto const& a : m_assertions)
        {
            result &= AddTest(a);
        }
        return result;
    }

    void AddAssertion(std::string const& test, std::string const& file, long line, beddev::IExpression const& expr)
    {
        m_assertions.push_back({ test, file, line, expr.Succeeded(), expr.ExpandedExpression(), beddev::NoParam{} });
    }

    void AddTestFact(std::string const& fact) { AddFact(fact); }
    void AddTestAction(std::string const& action) { AddAction(action); }

private:
    std::vector<TestAssertion> m_assertions;
};

#define ADD_FACT(TC, FACT) TC.AddTestFact(#FACT); FACT
#define ADD_ACTION(TC, ACTION) TC.AddTestAction(#ACTION); ACTION
#define ADD_ASSERTION(TC, EXPR) TC.AddAssertion(#EXPR, __FILE__, __LINE__, beddev::ExpressionParser::Get() <= EXPR)

static std::string const SUMMARY_SEP = "================================================================================\n";
static std::string const TEST_SEP = "--------------------------------------------------------------------------------\n";
static std::string const ASSERT_SEP = "................................................................................\n\n";

SCENARIO("No test case registered", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , std::ostringstream oss )
    WHEN( auto const failed = tr.RunAll(oss) )
    THEN( failed == 0
        , oss.str() == "No tests ran\n" )
}

SCENARIO("One test case registered with no assertions", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , EmptyTestCase tc("Test no assertion", BEDDEV_MAKE_FILE_LINE(__FILE__,__LINE__))
         , std::ostringstream oss )
    WHEN( tr.Register(&tc)
        , auto const failed = tr.RunAll(oss) )
    THEN( failed == 0
        , oss.str() == SUMMARY_SEP + "All tests passed (0 assertions in 1 test case)\n" )
}

SCENARIO("One test case registered with a succeeded assertion", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , EmptyTestCase tc("Test no assertion", BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__))
         , ADD_ASSERTION(tc, 1 == 1)
         , std::ostringstream oss )
    WHEN( tr.Register(&tc)
        , auto const failed = tr.RunAll(oss) )
    THEN( failed == 0
        , oss.str() == SUMMARY_SEP + "All tests passed (1 assertion in 1 test case)\n" )
}

SCENARIO("Two test cases registered with some succeeded assertions", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , EmptyTestCase tc1("Test two assertions", BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__))
         , ADD_ASSERTION(tc1, 1 == 1)
         , ADD_ASSERTION(tc1, 2 == 2)
         , EmptyTestCase tc2("Test one assertion", BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__))
         , ADD_ASSERTION(tc2, 3 == 3)
         , std::ostringstream oss )
    WHEN( tr.Register(&tc1)
        , tr.Register(&tc2)
        , auto const failed = tr.RunAll(oss) )
    THEN( failed == 0
        , oss.str() == SUMMARY_SEP + "All tests passed (3 assertions in 2 test cases)\n" )
}

SCENARIO("One test cases registered with a failed assertion", "TestRunner")
{
    GIVEN(DummyTestRunner tr
         , auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
         , EmptyTestCase tc("Test no assertion", fileAndLine)
         , ADD_ASSERTION(tc, 1 == 2)
         , std::ostringstream oss)
    WHEN( tr.Register(&tc)
        , auto const failed = tr.RunAll(oss))
    THEN( failed == 1
        , oss.str() == TEST_SEP
                    + "Scenario: Test no assertion\n"
                    + " Require: 1 == 2\n"
                    + TEST_SEP
                    + fileAndLine + "\n"
                    + ASSERT_SEP
                    + fileAndLine + ": FAILED:\n"
                    + "  1 == 2\n"
                    + SUMMARY_SEP
                    + "test cases: 1 | 0 passed | 1 failed\n"
                    + "assertions: 1 | 0 passed | 1 failed\n" )
}

SCENARIO("One test cases registered with a failed assertion and expanded expression", "TestRunner")
{
    GIVEN(DummyTestRunner tr
        , auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
        , EmptyTestCase tc("Test no assertion", fileAndLine)
        , ADD_FACT(tc, auto const a = 1)
        , ADD_FACT(tc, auto const b = 2)
        , ADD_ACTION(tc, auto const c = a + b)
        , ADD_ASSERTION(tc, c + 1 == 3)
        , std::ostringstream oss)
        WHEN(tr.Register(&tc)
            , auto const failed = tr.RunAll(oss))
        THEN(failed == 1
            , oss.str() == TEST_SEP
            + "Scenario: Test no assertion\n"
            + "   Given: auto const a = 1\n"
            + "     And: auto const b = 2\n"
            + "    When: auto const c = a + b\n"
            + "    Then: c + 1 == 3\n"
            + TEST_SEP
            + fileAndLine + "\n"
            + ASSERT_SEP
            + fileAndLine + ": FAILED:\n"
            + "  c + 1 == 3\n"
            + "with expansion:\n"
            + "  4 == 3\n"
            + SUMMARY_SEP
            + "test cases: 1 | 0 passed | 1 failed\n"
            + "assertions: 1 | 0 passed | 1 failed\n")
}

SCENARIO("Two test cases registered with some failed assertions", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
         , EmptyTestCase tc1("Test some failed assertions", fileAndLine, "TestCategory1")
         , ADD_ASSERTION(tc1, 1 == 2)
         , ADD_ASSERTION(tc1, 2 == 3)
         , EmptyTestCase tc2("Test a failed assertion and a succeeded assertion", fileAndLine, "TestCategory2")
         , ADD_ASSERTION(tc2, 3 == 4)
         , ADD_ASSERTION(tc2, 4 == 4)
         , std::ostringstream oss )
    WHEN( tr.Register(&tc1)
        , tr.Register(&tc2)
        , auto const failed = tr.RunAll(oss) )
    THEN( failed == 2
        , oss.str() == TEST_SEP
                    + "Scenario: Test some failed assertions  [TestCategory1]\n"
                    + " Require: 1 == 2\n"
                    + "     And: 2 == 3\n"
                    + TEST_SEP
                    + fileAndLine + "\n"
                    + ASSERT_SEP
                    + fileAndLine + ": FAILED:\n"
                    + "  1 == 2\n"
                    + ASSERT_SEP
                    + fileAndLine + ": FAILED:\n"
                    + "  2 == 3\n"
                    + TEST_SEP
                    + "Scenario: Test a failed assertion and a succeeded assertion  [TestCategory2]\n"
                    + " Require: 3 == 4\n"
                    + "     And: 4 == 4\n"
                    + TEST_SEP
                    + fileAndLine + "\n"
                    + ASSERT_SEP
                    + fileAndLine + ": FAILED:\n"
                    + "  3 == 4\n"
                    + SUMMARY_SEP
                    + "test cases: 2 | 0 passed | 2 failed\n"
                    + "assertions: 4 | 1 passed | 3 failed\n" )
}
