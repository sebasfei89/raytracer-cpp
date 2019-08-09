#include <Beddev/Beddev.h>
#include <Beddev/SessionSummary.h>

#include <sstream>

using namespace beddev;

namespace
{

#define DUMMY_PTESTCASE(PARGS, TARGS) DummyPTestCase<decltype(PARGS)::value_type>(__FILE__, __LINE__, PARGS, TARGS)
#define TEST_ASSERTION(TC, EXPR) TC.AddTestHelper(#EXPR, __FILE__, __LINE__, ExpressionParser::Get() <= EXPR)
#define TOSTR(EXPR) #EXPR

struct ArgT
{
    int i;
    std::string s;

    bool operator==(ArgT const& other) const
    {
        return (i == other.i) && (s == other.s);
    }
};

std::ostream& operator<<(std::ostream& os, ArgT const& arg)
{
    os << "[" << arg.i << "," << arg.s << "]";
    return os;
}

template<typename PARAM_T>
class DummyPTestCase : public ParametrizedTestCase<PARAM_T>
{
public:
    DummyPTestCase(std::string const& f, long l, std::vector<PARAM_T> const& params, std::vector<PARAM_T> const& testValues)
        : ParametrizedTestCase("DummyPTestCase", f, l, "ptest")
        , m_usedParams()
        , m_testValues(testValues)
        , m_testParams(params)
    {
    }

    bool RunImpl(ERunStep runStep) override
    {
        if (runStep == ERunStep::REGISTER_TEST_PARAMS)
        {
            SetParams(m_testParams);
            return true;
        }

        auto const& paramValue = GetParam();
        m_usedParams.push_back(paramValue);
        bool succeed = true;
        for (auto const& testValue : m_testValues)
        {
            succeed &= AddTest({TOSTR(paramValue == testValue), __FILE__, __LINE__, ExpressionParser::Get() <= paramValue == testValue});
        }
        return succeed;
    }

    std::vector<PARAM_T> m_usedParams;

private:
    std::vector<PARAM_T> m_testValues;
    std::vector<PARAM_T> m_testParams;
};

static std::string const SUMMARY_SEP = "================================================================================\n";
static std::string const TEST_SEP = "--------------------------------------------------------------------------------\n";
static std::string const ASSERT_SEP = "................................................................................\n\n";

} // EoF anonymous namespace

SCENARIO("An empty parametrized test case")
{
    auto const pArgs = { 1, 1 };
    std::vector<int> const tArgs = {};
    GIVEN( auto tc = DUMMY_PTESTCASE(pArgs, tArgs)
         , std::ostringstream oss
         , beddev::SessionSummary ss )
    WHEN( tc.Run(oss, ss) )
    THEN( ss.passed == 2
        , ss.failed == 0
        , ss.skipped == 0
        , ss.passedAssertions == 0
        , ss.failedAssertions == 0
        , ss.configFailures == 0
        , oss.str() == ""
        , tc.m_usedParams.size() == 2
        , tc.m_usedParams[0] == 1
        , tc.m_usedParams[1] == 1 )
}

SCENARIO("A parametrized test case with a single assertion")
{
    auto const pArgs = { 1, 2 };
    GIVEN( auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
         , auto tc = DUMMY_PTESTCASE(pArgs, {1} )
         , std::ostringstream oss
         , beddev::SessionSummary ss )
    WHEN( tc.Run(oss, ss) )
    THEN( ss.passed == 1
        , ss.failed == 1
        , ss.passedAssertions == 1
        , ss.failedAssertions == 1
        , oss.str() == TEST_SEP
                    + "Scenario: DummyPTestCase  [ptest]\n"
                    + "   Param: 2\n"
                    + " Require: paramValue == testValue\n"
                    + TEST_SEP
                    + fileAndLine + "\n"
                    + ASSERT_SEP
                    + BEDDEV_MAKE_FILE_LINE(__FILE__, 57) + ": FAILED with argument [2]:\n"
                    + "  paramValue == testValue\n"
                    + "with expansion:\n"
                    + "  2 == 1\n"
        , tc.m_usedParams.size() == 2
        , tc.m_usedParams[0] == 1
        , tc.m_usedParams[1] == 2 )
}

#define MAKE_SCENARIO(ARG) TEST_SEP\
    + "Scenario: DummyPTestCase  [ptest]\n"\
    + "   Param: " ARG "\n"\
    + " Require: paramValue == testValue\n"

#define MAKE_SCENARIO1(ARG) MAKE_SCENARIO(ARG)\
    + TEST_SEP\
    + fileAndLine + "\n"

#define MAKE_SCENARIO4(ARG) MAKE_SCENARIO(ARG)\
    + "     And: paramValue == testValue\n"\
    + "     And: paramValue == testValue\n"\
    + "     And: paramValue == testValue\n"\
    + TEST_SEP\
    + fileAndLine + "\n"

#define MAKE_ASSERT(ARG, VALUE) ASSERT_SEP\
    + BEDDEV_MAKE_FILE_LINE(__FILE__, 57) + ": FAILED with argument [" ARG "]:\n"\
    + "  paramValue == testValue\nwith expansion:\n  " ARG " == " VALUE "\n"

SCENARIO("A parametrized test case with some assertions")
{
    auto const pArgs = { 1, 2, 3 };
    auto const tArgs = { 1, 2, 3, 3 };
    GIVEN( auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
         , auto tc = DUMMY_PTESTCASE(pArgs, tArgs)
         , std::ostringstream oss
         , beddev::SessionSummary ss )
    WHEN( tc.Run(oss, ss) )
    THEN( ss.passed == 0
        , ss.failed == 3
        , ss.passedAssertions == 4
        , ss.failedAssertions == 8
        , oss.str() == MAKE_SCENARIO4("1")
                    + MAKE_ASSERT("1", "2")
                    + MAKE_ASSERT("1", "3")
                    + MAKE_ASSERT("1", "3")
                    + MAKE_SCENARIO4("2")
                    + MAKE_ASSERT("2", "1")
                    + MAKE_ASSERT("2", "3")
                    + MAKE_ASSERT("2", "3")
                    + MAKE_SCENARIO4("3")
                    + MAKE_ASSERT("3", "1")
                    + MAKE_ASSERT("3", "2")
        , tc.m_usedParams.size() == 3
        , tc.m_usedParams[0] == 1
        , tc.m_usedParams[1] == 2
        , tc.m_usedParams[2] == 3 )
}

SCENARIO("A parametrized test case with structured args")
{
    std::vector<ArgT> const pArgs = { ArgT{1,"one"}, ArgT{2,"two"}, ArgT{3,"three"} };
    std::vector<ArgT> const tArgs = { ArgT{1,"one"} };
    GIVEN( auto const fileAndLine = BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__)
         , auto tc = DUMMY_PTESTCASE(pArgs, tArgs)
         , std::ostringstream oss
         , beddev::SessionSummary ss )
    WHEN( tc.Run(oss, ss) )
    THEN( ss.passed == 1
        , ss.failed == 2
        , ss.passedAssertions == 1
        , ss.failedAssertions == 2
        , oss.str() == MAKE_SCENARIO1("[2,two]")
                    + MAKE_ASSERT("[2,two]", "[1,one]")
                    + MAKE_SCENARIO1("[3,three]")
                    + MAKE_ASSERT("[3,three]", "[1,one]")
        , tc.m_usedParams.size() == 3
        , tc.m_usedParams[0] == (ArgT{ 1,"one" })
        , tc.m_usedParams[1] == (ArgT{ 2,"two" })
        , tc.m_usedParams[2] == (ArgT{ 3,"three" }) )
}
