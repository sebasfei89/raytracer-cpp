#include <Beddev/Beddev.h>

#include <sstream>

class DummyTestRunner : public beddev::TestRunner {};

class DummyTestCase : public beddev::TestCase
{
public:
    DummyTestCase(std::string const& desc, std::string const& file, long line, std::string const& category = "")
        : beddev::TestCase(desc, file, line, category) {}

    bool RunImpl(ERunStep runStep) override
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
        m_assertions.push_back({ test, file, line, expr.Succeeded(), true, expr.ExpandedExpression(), beddev::NoParam{} });
    }

    void AddTestFact(std::string const& fact) { AddFact(fact); }
    void AddTestAction(std::string const& action) { AddAction(action); }

private:
    std::vector<TestAssertion> m_assertions;
};

template<typename PARAM_T>
class DummyPTestCase : public beddev::ParametrizedTestCase<PARAM_T>
{
public:
    DummyPTestCase(std::string const& desc, std::string const& file, long line, std::vector<PARAM_T> const& params, std::string const& category = "")
        : ParametrizedTestCase(desc, file, line, category)
        , m_params(params)
    {}

    bool RunImpl(ERunStep runStep) override
    {
        if (runStep == ERunStep::REGISTER_TEST_PARAMS)
        {
            SetParams(m_params);
            return true;
        }

        bool result = true;
        for (auto& a : m_assertions)
        {
            result &= AddTest(a.test + " == GetParam()", a.file, a.line, a.partialExpr == GetParam());
        }
        return result;
    }

    void AddAssertion(std::string const& test, std::string const& file, long line, beddev::UnaryExpression<PARAM_T> const& expr) { m_assertions.push_back({ test, file, line, expr }); }
    void AddTestFact(std::string const& fact) { AddFact(fact); }
    void AddTestAction(std::string const& action) { AddAction(action); }

    struct AssertionData
    {
        std::string test;
        std::string file;
        long line;
        beddev::UnaryExpression<PARAM_T> partialExpr;
    };

private:

    std::vector<AssertionData> m_assertions;
    std::vector<PARAM_T> m_params;
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
         , DummyTestCase tc("Test no assertion", __FILE__,__LINE__)
         , std::ostringstream oss )
    WHEN( tr.Register(&tc)
        , auto const failed = tr.RunAll(oss) )
    THEN( failed == 0
        , oss.str() == SUMMARY_SEP + "All tests passed (0 assertions in 1 test case)\n" )
}

SCENARIO("One test case registered with a succeeded assertion", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , DummyTestCase tc("Test no assertion", __FILE__, __LINE__)
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
         , DummyTestCase tc1("Test two assertions", __FILE__, __LINE__)
         , ADD_ASSERTION(tc1, 1 == 1)
         , ADD_ASSERTION(tc1, 2 == 2)
         , DummyTestCase tc2("Test one assertion", __FILE__, __LINE__)
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
    GIVEN( DummyTestRunner tr
         , auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
         , DummyTestCase tc("Test no assertion", __FILE__, __LINE__)
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
        , DummyTestCase tc("Test no assertion", __FILE__, __LINE__)
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
         , DummyTestCase tc1("Test some failed assertions", __FILE__, __LINE__, "TestCategory1")
         , ADD_ASSERTION(tc1, 1 == 2)
         , ADD_ASSERTION(tc1, 2 == 3)
         , DummyTestCase tc2("Test a failed assertion and a succeeded assertion", __FILE__, __LINE__, "TestCategory2")
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

SCENARIO("Filter by name with two test cases registered", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , DummyTestCase tc1("Test assertion 1", __FILE__, __LINE__)
         , ADD_ASSERTION(tc1, 1 == 1)
         , DummyTestCase tc2("Test assertion 2", __FILE__, __LINE__)
         , ADD_ASSERTION(tc2, 2 == 2)
         , std::ostringstream oss )
    WHEN( tr.Register(&tc1)
        , tr.Register(&tc2)
        , auto const failed = tr.RunAll(oss, "Test assertion 2") )
    THEN( failed == 0
        , oss.str() == SUMMARY_SEP + "All tests passed (1 assertion in 1 test case, 1 test case skipped)\n")
}

SCENARIO("Filter by name with two test cases registered and a failing assertion", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , DummyTestCase tc1("Test assertion 1", __FILE__, __LINE__)
         , ADD_ASSERTION(tc1, 1 == 1)
         , auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
         , DummyTestCase tc2("Test assertion 2", __FILE__, __LINE__)
         , ADD_ASSERTION(tc2, 2 == 3)
         , std::ostringstream oss)
    WHEN( tr.Register(&tc1)
        , tr.Register(&tc2)
        , auto const failed = tr.RunAll(oss, "Test assertion 2"))
    THEN( failed == 1
        , oss.str() == TEST_SEP
        + "Scenario: Test assertion 2\n"
        + " Require: 2 == 3\n"
        + TEST_SEP
        + fileAndLine + "\n"
        + ASSERT_SEP
        + fileAndLine + ": FAILED:\n"
        + "  2 == 3\n"
        + SUMMARY_SEP
        + "test cases: 2 | 0 passed | 1 failed | 1 skipped\n"
        + "assertions: 1 | 0 passed | 1 failed\n" )
}

SCENARIO("One parametrized test case registered with no assertions", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , DummyPTestCase<float> tc("Test no assertion", __FILE__, __LINE__, {1.f, 2.f})
         , std::ostringstream oss )
    WHEN( tr.Register(&tc)
        , auto const failed = tr.RunAll(oss) )
    THEN( failed == 0
        , oss.str() == SUMMARY_SEP + "All tests passed (0 assertions in 2 test cases)\n" )
}

SCENARIO("One parametrized test case registered with no params", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__);
         , DummyPTestCase<int> tc("PTest with no params", __FILE__, __LINE__, {})
         , ADD_ASSERTION(tc, 1)
         , std::ostringstream oss )
    WHEN( tr.Register(&tc)
        , auto const failed = tr.RunAll(oss))
    THEN( failed == 1
        , oss.str() == TEST_SEP
        + "Scenario: PTest with no params\n"
        + TEST_SEP
        + fileAndLine + "\n"
        + ASSERT_SEP
        + "  No parameters provided to a parametrized test case\n\n"
        + SUMMARY_SEP
        + "test cases: 1 | 0 passed | 1 failed\n"
        + "assertions: 0 | 0 passed | 0 failed\n")
}

SCENARIO("One parametrized test case registered with a succeeded assertion", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , DummyPTestCase<int> tc("Test no assertion", __FILE__, __LINE__, {1, 1})
         , ADD_ASSERTION(tc, 1)
         , std::ostringstream oss )
    WHEN( tr.Register(&tc)
        , auto const failed = tr.RunAll(oss) )
    THEN( failed == 0
        , oss.str() == SUMMARY_SEP + "All tests passed (2 assertions in 2 test cases)\n" )
}

SCENARIO("Two parametrized test cases registered with some succeeded assertions", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , DummyPTestCase<int> tc1("Test two assertions", __FILE__, __LINE__, {1, 1})
         , ADD_ASSERTION(tc1, 1)
         , ADD_ASSERTION(tc1, 1)
         , DummyPTestCase<int> tc2("Test one assertion", __FILE__, __LINE__, {3, 3})
         , ADD_ASSERTION(tc2, 3)
         , std::ostringstream oss )
    WHEN( tr.Register(&tc1)
        , tr.Register(&tc2)
        , auto const failed = tr.RunAll(oss) )
    THEN( failed == 0
        , oss.str() == SUMMARY_SEP + "All tests passed (6 assertions in 4 test cases)\n")
}

SCENARIO("One parametrized test cases registered with some failed assertion", "TestRunner")
{
    GIVEN( DummyTestRunner tr
         , auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
         , DummyPTestCase<int> tc("PTest with some failed assertion", __FILE__, __LINE__, {2, 3})
         , ADD_ASSERTION(tc, 1)
         , std::ostringstream oss )
    WHEN( tr.Register(&tc)
        , auto const failed = tr.RunAll(oss) )
    THEN( failed == 2
        , oss.str() == TEST_SEP
                    + "Scenario: PTest with some failed assertion\n"
                    + "   Param: 2\n"
                    + " Require: 1 == GetParam()\n"
                    + TEST_SEP
                    + fileAndLine + "\n"
                    + ASSERT_SEP
                    + fileAndLine + ": FAILED with argument [2]:\n"
                    + "  1 == GetParam()\n"
                    + "with expansion:\n"
                    + "  1 == 2\n"
                    + TEST_SEP
                    + "Scenario: PTest with some failed assertion\n"
                    + "   Param: 3\n"
                    + " Require: 1 == GetParam()\n"
                    + TEST_SEP
                    + fileAndLine + "\n"
                    + ASSERT_SEP
                    + fileAndLine + ": FAILED with argument [3]:\n"
                    + "  1 == GetParam()\n"
                    + "with expansion:\n"
                    + "  1 == 3\n"
                    + SUMMARY_SEP
                    + "test cases: 2 | 0 passed | 2 failed\n"
                    + "assertions: 2 | 0 passed | 2 failed\n" )
}

//SCENARIO("One test cases registered with a failed assertion and expanded expression", "TestRunner")
//{
//    GIVEN(DummyTestRunner tr
//        , auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
//        , DummyTestCase tc("Test no assertion", fileAndLine)
//        , ADD_FACT(tc, auto const a = 1)
//        , ADD_FACT(tc, auto const b = 2)
//        , ADD_ACTION(tc, auto const c = a + b)
//        , ADD_ASSERTION(tc, c + 1 == 3)
//        , std::ostringstream oss)
//        WHEN(tr.Register(&tc)
//            , auto const failed = tr.RunAll(oss))
//        THEN(failed == 1
//            , oss.str() == TEST_SEP
//            + "Scenario: Test no assertion\n"
//            + "   Given: auto const a = 1\n"
//            + "     And: auto const b = 2\n"
//            + "    When: auto const c = a + b\n"
//            + "    Then: c + 1 == 3\n"
//            + TEST_SEP
//            + fileAndLine + "\n"
//            + ASSERT_SEP
//            + fileAndLine + ": FAILED:\n"
//            + "  c + 1 == 3\n"
//            + "with expansion:\n"
//            + "  4 == 3\n"
//            + SUMMARY_SEP
//            + "test cases: 1 | 0 passed | 1 failed\n"
//            + "assertions: 1 | 0 passed | 1 failed\n")
//}
//
//SCENARIO("Two test cases registered with some failed assertions", "TestRunner")
//{
//    GIVEN(DummyTestRunner tr
//        , auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
//        , DummyTestCase tc1("Test some failed assertions", fileAndLine, "TestCategory1")
//        , ADD_ASSERTION(tc1, 1 == 2)
//        , ADD_ASSERTION(tc1, 2 == 3)
//        , DummyTestCase tc2("Test a failed assertion and a succeeded assertion", fileAndLine, "TestCategory2")
//        , ADD_ASSERTION(tc2, 3 == 4)
//        , ADD_ASSERTION(tc2, 4 == 4)
//        , std::ostringstream oss)
//        WHEN(tr.Register(&tc1)
//            , tr.Register(&tc2)
//            , auto const failed = tr.RunAll(oss))
//        THEN(failed == 2
//            , oss.str() == TEST_SEP
//            + "Scenario: Test some failed assertions  [TestCategory1]\n"
//            + " Require: 1 == 2\n"
//            + "     And: 2 == 3\n"
//            + TEST_SEP
//            + fileAndLine + "\n"
//            + ASSERT_SEP
//            + fileAndLine + ": FAILED:\n"
//            + "  1 == 2\n"
//            + ASSERT_SEP
//            + fileAndLine + ": FAILED:\n"
//            + "  2 == 3\n"
//            + TEST_SEP
//            + "Scenario: Test a failed assertion and a succeeded assertion  [TestCategory2]\n"
//            + " Require: 3 == 4\n"
//            + "     And: 4 == 4\n"
//            + TEST_SEP
//            + fileAndLine + "\n"
//            + ASSERT_SEP
//            + fileAndLine + ": FAILED:\n"
//            + "  3 == 4\n"
//            + SUMMARY_SEP
//            + "test cases: 2 | 0 passed | 2 failed\n"
//            + "assertions: 4 | 1 passed | 3 failed\n")
//}