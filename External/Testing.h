#include "Catch2/catch.hpp"

#define THEN_REQUIRE(expr) THEN((#expr)) { REQUIRE(expr); }
#define AND_THEN_REQUIRE(expr) AND_THEN(#expr) { REQUIRE(expr); }
#define WHEN_REQUIRE(expr, req) WHEN(#expr) { expr; THEN_REQUIRE(req) }
#define GIVEN_WHEN_REQUIRE(fact, action, result) { fact; GIVEN(#fact) { WHEN_REQUIRE(action, result) }}
