#include <Beddev/Beddev.h>

#include <sstream>

using namespace beddev;

namespace
{

#define DUMMY_PTESTCASE(PARGS, TARGS) DummyPTestCase<decltype(PARGS)::value_type>(BEDDEV_MAKE_FILE_LINE(__FILE__, __LINE__), PARGS, TARGS)
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
    DummyPTestCase(std::string const& fl, std::vector<PARAM_T> const& params, std::vector<PARAM_T> const& testValues)
        : ParametrizedTestCase("DummyPTestCase", fl, "ptest", params)
        , m_usedParams()
        , m_testValues(testValues)
    {}

    bool RunImpl() override
    {
        auto const& paramValue = GetParam();
        m_usedParams.push_back(paramValue);
        bool succeed = true;
        for (auto const& testValue : m_testValues)
        {
            succeed &= AddTest(TOSTR(paramValue == testValue), __FILE__, __LINE__, ExpressionParser::Get() <= paramValue == testValue);
        }
        return succeed;
    }

    std::string GetExpectedOutput() const
    {
        std::ostringstream oss;
        ReportFailure(oss);
        return oss.str();
    }

    std::vector<PARAM_T> m_usedParams;

private:
    std::vector<PARAM_T> m_testValues;
};

}

SCENARIO("An empty parametrized test case")
{
    auto const pArgs = { 1, 1 };
    std::vector<int> const tArgs = {};
    GIVEN(auto tc = DUMMY_PTESTCASE(pArgs, tArgs)
         , std::ostringstream oss)
    WHEN( tc.Run(oss)
        , auto tests = tc.Assertions() )
    THEN( std::get<0>(tests) == 0
        , std::get<1>(tests) == 0
        , oss.str() == ""
        , tc.m_usedParams.size() == 2
        , tc.m_usedParams[0] == 1
        , tc.m_usedParams[1] == 1 )
}

SCENARIO("A parametrized test case with a single assertion")
{
    auto const pArgs = { 1, 2 };
    GIVEN( auto tc = DUMMY_PTESTCASE(pArgs, {1} )
         , std::ostringstream oss )
    WHEN( tc.Run(oss)
        , auto tests = tc.Assertions() )
    THEN( std::get<0>(tests) == 1
        , std::get<1>(tests) == 1
        , oss.str() == tc.GetExpectedOutput()
        , tc.m_usedParams.size() == 2
        , tc.m_usedParams[0] == 1
        , tc.m_usedParams[1] == 2 )
}

SCENARIO("A parametrized test case with some assertions")
{
    auto const pArgs = { 1, 2, 3 };
    auto const tArgs = { 1, 2, 3, 3 };
    GIVEN( auto tc = DUMMY_PTESTCASE(pArgs, tArgs)
         , std::ostringstream oss )
    WHEN( tc.Run(oss)
        , auto tests = tc.Assertions() )
    THEN( std::get<0>(tests) == 4
        , std::get<1>(tests) == 8
        , oss.str() == tc.GetExpectedOutput()
        , tc.m_usedParams.size() == 3
        , tc.m_usedParams[0] == 1
        , tc.m_usedParams[1] == 2
        , tc.m_usedParams[2] == 3 )
}

SCENARIO("A test case parametrized with structured args")
{
    std::vector<ArgT> const pArgs = { ArgT{1,"one"}, ArgT{2,"two"}, ArgT{3,"three"} };
    std::vector<ArgT> const tArgs = { ArgT{1,"one"} };
    GIVEN( auto tc = DUMMY_PTESTCASE(pArgs, tArgs)
         , std::ostringstream oss )
    WHEN( tc.Run(oss)
        , auto tests = tc.Assertions() )
    THEN( std::get<0>(tests) == 1
        , std::get<1>(tests) == 2
        , oss.str() == tc.GetExpectedOutput()
        , tc.m_usedParams.size() == 3
        , tc.m_usedParams[0] == (ArgT{ 1,"one" })
        , tc.m_usedParams[1] == (ArgT{ 2,"two" })
        , tc.m_usedParams[2] == (ArgT{ 3,"three" }) )
}
