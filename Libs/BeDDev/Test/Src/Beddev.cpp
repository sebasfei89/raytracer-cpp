#include <Beddev/Beddev.h>

SCENARIO("Basic test")
{
    REQUIRE(
        "1" == "1",
        1 == 1,
        1.f == 1.f,
        true,
        true != false )
}

SCENARIO("Given/Then test")
{
    GIVEN(int const a = 1,
          int const b = 2 )
    THEN( a + 1 == b )
}

SCENARIO("When/Then test")
{
    WHEN( int const sum = 1 + 2 )
    THEN( sum == 3 )
}

SCENARIO("Given/When/Then test")
{
    GIVEN(int const a = 1,
          int const b = 2)
    WHEN( int const c = a + b )
    THEN( a + b == c )
}

PSCENARIO(float, "Basic parametrized test", "ptest")
{
    PARAMS( 1.f, 2.f, 3.f )
    GIVEN( auto p = GetParam()
         , static float s_total = 0.f
         , static int s_iteration = 0 )
    WHEN( s_total += p
        , s_iteration++ )
    THEN( s_total == s_iteration * (s_iteration + 1) * .5f)
}
