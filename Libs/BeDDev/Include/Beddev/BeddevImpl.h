#pragma once

#include "./ExpressionParser.h"
#include "./ParametrizedTestCase.h"
#include "./TestCase.h"
#include "./TestRunner.h"

// BEDDEV_FOREACH macro allows applying another macro to each element in __VA_ARGS__
#define BEDDEV_EXPAND(...) __VA_ARGS__
#define BEDDEV_CAT(a, ...) BEDDEV_EXPAND( BEDDEV_PRIMITIVE_CAT(a, __VA_ARGS__) )
#define BEDDEV_PRIMITIVE_CAT(a, ...) BEDDEV_EXPAND( a ## __VA_ARGS__ )

#define BEDDEV_FE_1(WHAT, X) WHAT(X) 
#define BEDDEV_FE_2(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_1(WHAT, __VA_ARGS__))
#define BEDDEV_FE_3(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_2(WHAT, __VA_ARGS__))
#define BEDDEV_FE_4(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_3(WHAT, __VA_ARGS__))
#define BEDDEV_FE_5(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_4(WHAT, __VA_ARGS__))
#define BEDDEV_FE_6(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_5(WHAT, __VA_ARGS__))
#define BEDDEV_FE_7(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_6(WHAT, __VA_ARGS__))
#define BEDDEV_FE_8(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_7(WHAT, __VA_ARGS__))
#define BEDDEV_FE_9(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_8(WHAT, __VA_ARGS__))
#define BEDDEV_FE_10(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_9(WHAT, __VA_ARGS__))
#define BEDDEV_FE_11(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_10(WHAT, __VA_ARGS__))
#define BEDDEV_FE_12(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_11(WHAT, __VA_ARGS__))
#define BEDDEV_FE_13(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_12(WHAT, __VA_ARGS__))
#define BEDDEV_FE_14(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_13(WHAT, __VA_ARGS__))
#define BEDDEV_FE_15(WHAT, X, ...) BEDDEV_EXPAND( WHAT(X)BEDDEV_FE_14(WHAT, __VA_ARGS__))

#define BEDDEV_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, NAME, ...) NAME
#define BEDDEV_FOREACH(action, ...) BEDDEV_EXPAND( BEDDEV_GET_MACRO(__VA_ARGS__, BEDDEV_FE_15, BEDDEV_FE_14, BEDDEV_FE_13, BEDDEV_FE_12, BEDDEV_FE_11, BEDDEV_FE_10, BEDDEV_FE_9, BEDDEV_FE_8, BEDDEV_FE_7, BEDDEV_FE_6, BEDDEV_FE_5, BEDDEV_FE_4, BEDDEV_FE_3, BEDDEV_FE_2, BEDDEV_FE_1)(action, __VA_ARGS__) )
// END BEDDEV_FOREACH

#define BEDDEV_SCENARIO_DECL(D, F, L, CN, BASE, RUNIMPL, ...) namespace {\
    class CN : public BASE\
    {\
    private:\
        static CN m_instance;\
        CN() : BASE(D, F, L, __VA_ARGS__) {}\
        bool RUNIMPL override;\
    };\
    CN CN::m_instance;\
    bool CN::RUNIMPL

#define BEDDEV_START_SCENARIO(D, F, L, CN, ...) BEDDEV_SCENARIO_DECL(D, F, L, CN, beddev::TestCase, RunImpl(), __VA_ARGS__)
#define BEDDEV_START_PSCENARIO(ARGT, D, F, L, CN, ...) BEDDEV_SCENARIO_DECL(D, F, L, CN, beddev::ParametrizedTestCase<ARGT>, RunImpl(ERunStep runStep), __VA_ARGS__)

#define BEDDEV_END_SCENARIO() }

#define BEDDEV_MAKE_TESTCASE_NAME(P,L) BEDDEV_CAT(P,L)
#define BEDDEV_LINE_STR(L) #L
#define BEDDEV_MAKE_FILE_LINE(F,L) BEDDEV_CAT(F, "(" BEDDEV_LINE_STR(L) ")")

#define BEDDEV_FACT(fact) AddFact(#fact); fact;
#define BEDDEV_ACTION(action) AddAction(#action); action;
#define BEDDEV_TEST(test) __beddev__result &= AddTest({#test, __FILE__, __LINE__, beddev::ExpressionParser::Get() <= test});

#define BEDDEV_SCENARIO(desc, ...) BEDDEV_START_SCENARIO(desc, __FILE__, __LINE__, BEDDEV_MAKE_TESTCASE_NAME(TestCase_, __LINE__), __VA_ARGS__)
#define BEDDEV_PSCENARIO(ARGT, DESC, ...) BEDDEV_START_PSCENARIO(ARGT, DESC, __FILE__, __LINE__, BEDDEV_MAKE_TESTCASE_NAME(TestCase_, __LINE__), __VA_ARGS__)
#define BEDDEV_GIVEN(...) BEDDEV_FOREACH(BEDDEV_FACT,__VA_ARGS__)
#define BEDDEV_WHEN(...) BEDDEV_FOREACH(BEDDEV_ACTION,__VA_ARGS__)
#define BEDDEV_THEN(...) bool __beddev__result = true; BEDDEV_FOREACH(BEDDEV_TEST,__VA_ARGS__) return __beddev__result; BEDDEV_END_SCENARIO()
#define BEDDEV_PARAMS(...) if (runStep == ERunStep::REGISTER_TEST_PARAMS) { SetParams({__VA_ARGS__}); return true; }
