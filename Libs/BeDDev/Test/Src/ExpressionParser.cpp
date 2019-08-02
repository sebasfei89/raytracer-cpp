#include <Beddev/Beddev.h>
#include <Beddev/ExpressionParser.h>

using namespace beddev;

namespace
{
template<typename T>
T GetValue(T const& value) { return value; }
}

////////////////////////////////////////////////////////////////////////////////
////    bool unary expressions
////////////////////////////////////////////////////////////////////////////////
SCENARIO("Unary expression with true constant", "expressions")
{
    GIVEN( auto const ep = ExpressionParser::Get() <= true )
    THEN( ep.Type() == IExpression::EType::UNARY
        , ep.Succeeded()
        , ep.ExpandedExpression() == "true" )
}

SCENARIO("UnaryExpressionwith false constant", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= false )
    THEN( ep.Type() == IExpression::EType::UNARY
        , !ep.Succeeded()
        , ep.ExpandedExpression() == "false" )
}

SCENARIO("Unary expression that returns true", "expressions")
{
    GIVEN(auto ep = ExpressionParser::Get() <= GetValue<bool>(true))
        THEN(ep.ExpandedExpression() == "true"
            , ep.Type() == IExpression::EType::UNARY
            , ep.Succeeded())
}

SCENARIO("Unary expression that returns false", "expressions")
{
    GIVEN(auto ep = ExpressionParser::Get() <= GetValue<bool>(false))
        THEN(ep.ExpandedExpression() == "false"
            , ep.Type() == IExpression::EType::UNARY
            , !ep.Succeeded())
}

////////////////////////////////////////////////////////////////////////////////
////    int unary expressions
////////////////////////////////////////////////////////////////////////////////
SCENARIO("Unary expression with integer constant (true)", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 )
    THEN( ep.ExpandedExpression() == "1"
        , ep.Type() == IExpression::EType::UNARY
        , ep.Succeeded() )
}

SCENARIO("Unary expression with integer constant (false)", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 0 )
    THEN( ep.ExpandedExpression() == "0"
        , ep.Type() == IExpression::EType::UNARY
        , !ep.Succeeded() )
}

SCENARIO("Unary expression that returns 1", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= GetValue<int>(1) )
    THEN( ep.ExpandedExpression() == "1"
        , ep.Type() == IExpression::EType::UNARY
        , ep.Succeeded() )
}

SCENARIO("Unary expression that returns -1", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= GetValue<int>(-1) )
    THEN( ep.ExpandedExpression() == "-1"
        , ep.Type() == IExpression::EType::UNARY
        , ep.Succeeded() )
}

SCENARIO("Unary expression that returns 0", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= GetValue<int>(0))
    THEN( ep.ExpandedExpression() == "0"
        , ep.Type() == IExpression::EType::UNARY
        , !ep.Succeeded() )
}

////////////////////////////////////////////////////////////////////////////////
////    c-string unary expressions
////////////////////////////////////////////////////////////////////////////////
SCENARIO("Unary expression with empty c-string", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= "" )
    THEN( ep.ExpandedExpression() == "\"\""
        , ep.Type() == IExpression::EType::UNARY
        , !ep.Succeeded() )
}

SCENARIO("Unary expression with non-empty c-string", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= "test" )
    THEN( ep.ExpandedExpression() == "\"test\""
        , ep.Type() == IExpression::EType::UNARY
        , ep.Succeeded() )
}

////////////////////////////////////////////////////////////////////////////////
////    float unary expressions
////////////////////////////////////////////////////////////////////////////////
SCENARIO("Unary expression with float constant (true)", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1.5f)
    THEN( ep.ExpandedExpression() == "1.5"
        , ep.Type() == IExpression::EType::UNARY
        , ep.Succeeded() )
}

SCENARIO("Unary expression with float constant (false)", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 0.f )
    THEN( ep.ExpandedExpression() == "0"
        , ep.Type() == IExpression::EType::UNARY
        , !ep.Succeeded() )
}

SCENARIO("Unary expression that returns 1.f", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= GetValue<float>(1.f) )
    THEN( ep.ExpandedExpression() == "1"
        , ep.Type() == IExpression::EType::UNARY
        , ep.Succeeded() )
}

SCENARIO("Unary expression that returns -1.0f", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= GetValue<float>(-1.f) )
    THEN( ep.ExpandedExpression() == "-1"
        , ep.Type() == IExpression::EType::UNARY
        , ep.Succeeded() )
}

SCENARIO("Unary expression that returns 0.f", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= GetValue<float>(0.f) )
    THEN( ep.ExpandedExpression() == "0"
        , ep.Type() == IExpression::EType::UNARY
        , !ep.Succeeded() )
}

SCENARIO("Unary expression that returns 3.1416f", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= GetValue<float>(3.14159265359f) )
    THEN( ep.ExpandedExpression() == "3.14159"
        , ep.Type() == IExpression::EType::UNARY
        , ep.Succeeded() )
}

////////////////////////////////////////////////////////////////////////////////
////    bool binary expressions
////////////////////////////////////////////////////////////////////////////////
SCENARIO("Binary expression where true == true", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= true == true )
    THEN( ep.ExpandedExpression() == "true == true"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where false == false", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= false == false )
    THEN( ep.ExpandedExpression() == "false == false"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where true == false", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= true == false )
    THEN( ep.ExpandedExpression() == "true == false"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where false == true", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= false == true )
    THEN( ep.ExpandedExpression() == "false == true"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where true != true", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= true != true )
    THEN( ep.ExpandedExpression() == "true != true"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where false != false", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= false != false )
    THEN( ep.ExpandedExpression() == "false != false"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where true != false", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= true != false )
    THEN(ep.ExpandedExpression() == "true != false"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where false != true", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= false != true )
    THEN( ep.ExpandedExpression() == "false != true"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

////////////////////////////////////////////////////////////////////////////////
////    c-string binary expressions
////////////////////////////////////////////////////////////////////////////////
SCENARIO("Binary expression where \"test\" == \"test\"", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= "test" == "test" )
    THEN( ep.ExpandedExpression() == "\"test\" == \"test\""
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where \"test\" == \"test2\"", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= "test" == "test2" )
    THEN( ep.ExpandedExpression() == "\"test\" == \"test2\""
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where \"test\" != \"test\"", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= "test" != "test" )
    THEN( ep.ExpandedExpression() == "\"test\" != \"test\""
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where \"test\" != \"test2\"", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= "test" != "test2" )
    THEN( ep.ExpandedExpression() == "\"test\" != \"test2\""
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

////////////////////////////////////////////////////////////////////////////////
////    int binary expressions
////////////////////////////////////////////////////////////////////////////////
SCENARIO("Binary expression where 1 == 1", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 == 1 )
    THEN( ep.ExpandedExpression() == "1 == 1"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where 1 == -3", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 == -3 )
    THEN( ep.ExpandedExpression() == "1 == -3"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where 1 != 1", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 != 1 )
    THEN( ep.ExpandedExpression() == "1 != 1"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where 1 != -3", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 != -3 )
    THEN( ep.ExpandedExpression() == "1 != -3"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where 1 > 1", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 > 1 )
    THEN( ep.ExpandedExpression() == "1 > 1"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where 1 > -3", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 > -3 )
    THEN( ep.ExpandedExpression() == "1 > -3"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where 1 < 1", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 < 1 )
    THEN( ep.ExpandedExpression() == "1 < 1"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where 1 < 3", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 < 3 )
    THEN( ep.ExpandedExpression() == "1 < 3"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where 1 >= -3", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 >= -3)
    THEN( ep.ExpandedExpression() == "1 >= -3"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where 1 >= 1", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 >= 1 )
    THEN( ep.ExpandedExpression() == "1 >= 1"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where 1 >= 3", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 >= 3 )
    THEN( ep.ExpandedExpression() == "1 >= 3"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where 1 <= -3", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 <= -3)
    THEN( ep.ExpandedExpression() == "1 <= -3"
        , ep.Type() == IExpression::EType::BINARY
        , !ep.Succeeded() )
}

SCENARIO("Binary expression where 1 <= 1", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 <= 1 )
    THEN( ep.ExpandedExpression() == "1 <= 1"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}

SCENARIO("Binary expression where 1 <= 3", "expressions")
{
    GIVEN( auto ep = ExpressionParser::Get() <= 1 <= 3 )
    THEN( ep.ExpandedExpression() == "1 <= 3"
        , ep.Type() == IExpression::EType::BINARY
        , ep.Succeeded() )
}
