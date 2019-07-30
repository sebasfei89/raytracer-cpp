#pragma once

#include "Expression.h"

namespace beddev
{

class ExpressionParser
{
public:
    static ExpressionParser const& Get()
    {
        static ExpressionParser ep;
        return ep;
    }

    template<typename LHST>
    UnaryExpression<LHST> operator<=(LHST value) const { return value; }

private:
    ExpressionParser() = default;
};

}
