#include "Catch2/catch.hpp"

// GIVEN some facts THEN test results
#define GIVEN_1(facts, ...) GIVEN_1_IMPL(facts, __VA_ARGS__)

// GIVEN some facts WHEN applying some action THEN test results
#define GIVEN_2(facts, actions, ...) GIVEN_2_IMPL(facts, actions, __VA_ARGS__)

#define THEN_REQUIRE(expr) THEN((#expr)) { REQUIRE(expr); }
#define GIVEN_1_IMPL(facts, ...) { facts; GIVEN(#facts) { FOR_EACH(THEN_REQUIRE,__VA_ARGS__) }}
#define GIVEN_2_IMPL(facts, actions, ...) { facts; GIVEN(#facts) { WHEN(#actions) { actions; FOR_EACH(THEN_REQUIRE,__VA_ARGS__) }}}
#define WHEN_
#define REQUIRE_

// FOR_EACH macro allows applying another macro to each element in __VA_ARGS__
#define EXPAND(X) X
#define FE_1(WHAT, X) WHAT(X) 
#define FE_2(WHAT, X, ...) EXPAND( WHAT(X)FE_1(WHAT, __VA_ARGS__))
#define FE_3(WHAT, X, ...) EXPAND( WHAT(X)FE_2(WHAT, __VA_ARGS__))
#define FE_4(WHAT, X, ...) EXPAND( WHAT(X)FE_3(WHAT, __VA_ARGS__))
#define FE_5(WHAT, X, ...) EXPAND( WHAT(X)FE_4(WHAT, __VA_ARGS__))
#define FE_6(WHAT, X, ...) EXPAND( WHAT(X)FE_5(WHAT, __VA_ARGS__))
#define FE_7(WHAT, X, ...) EXPAND( WHAT(X)FE_6(WHAT, __VA_ARGS__))
#define FE_8(WHAT, X, ...) EXPAND( WHAT(X)FE_7(WHAT, __VA_ARGS__))
#define FE_9(WHAT, X, ...) EXPAND( WHAT(X)FE_8(WHAT, __VA_ARGS__))

#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, NAME, ...) NAME
#define FOR_EACH(action, ...) EXPAND( GET_MACRO(__VA_ARGS__, FE_9, FE_8, FE_7, FE_6, FE_5, FE_4, FE_3, FE_2, FE_1)(action, __VA_ARGS__) )
