#pragma once

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace
{

template<class T>
struct is_c_str : std::integral_constant<bool, std::is_same<char const *, typename std::decay<T>::type>::value || std::is_same<char *, typename std::decay<T>::type>::value> {};

template<typename T>
class is_printable
{
    template<typename SS, typename TT>
    static auto test(int) -> decltype(std::declval<SS&>() << std::declval<TT>(), std::true_type());

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<std::ostream, T const&>(0))::value;
};

}

namespace beddev
{

class IExpression
{
public:
    enum EType {
        UNARY,
        BINARY
    };

    enum class EOperator
    {
        EQUALS,
        NOTEQUALS,
        LESS,
        GREATER,
        LESSEQUAL,
        GREATEREQUAL
    };

    virtual EType       Type() const = 0;
    virtual bool        Succeeded() const = 0;
    virtual std::string ExpandedExpression() const = 0;
};

template<typename LHST, typename RHST>
class BinaryExpression : public IExpression
{
public:
    BinaryExpression(LHST lhs, RHST rhs, EOperator op, bool value) : m_lhs(lhs), m_rhs(rhs), m_op(op), m_value(value) {}

    EType       Type() const override { return EType::BINARY; }
    bool        Succeeded() const override { return m_value; }
    std::string ExpandedExpression() const override { return GetExpandedExpression(m_lhs) + GetOperator() + GetExpandedExpression(m_rhs); }

protected:
    template<typename T = LHST>
    typename std::enable_if<!is_printable<T>::value, std::string>::type
    GetExpandedExpression(T const& value) const { return "<?>"; }

    template<typename T = LHST>
    typename std::enable_if<is_printable<T>::value && !is_c_str<T>::value && !std::is_same_v<T, std::string>, std::string>::type
    GetExpandedExpression(T const& value) const { std::ostringstream oss; oss << std::boolalpha << value; return oss.str(); }

    template<typename T = LHST>
    typename std::enable_if_t<is_c_str<T>::value || std::is_same_v<T, std::string>, std::string>
    GetExpandedExpression(T const& value) const { std::ostringstream oss; oss << std::boolalpha << std::quoted(value); return oss.str(); }

    constexpr std::string GetOperator() const
    {
        static std::unordered_map<EOperator, char const*> opToString =
        {
            { EOperator::EQUALS      , " == " },
            { EOperator::NOTEQUALS   , " != " },
            { EOperator::LESS        , " < "  },
            { EOperator::GREATER     , " > "  },
            { EOperator::LESSEQUAL   , " <= " },
            { EOperator::GREATEREQUAL, " >= " }
        };
        return opToString[m_op];
    }

private:
    LHST m_lhs;
    RHST m_rhs;
    EOperator m_op;
    bool m_value;
};

template<typename LHST>
class UnaryExpression : public IExpression
{
public:
    UnaryExpression(LHST lhs) : m_lhs{ lhs } {}

    EType       Type() const override { return EType::UNARY; }
    bool        Succeeded() const override { return GetSucceeded(); }
    std::string ExpandedExpression() const override { return GetExpandedExpression(); }

    template<typename RHST>
    BinaryExpression<LHST, RHST> operator==(RHST value) const { return { m_lhs, value, EOperator::EQUALS, static_cast<bool>(m_lhs == value) }; }

    template<typename RHST>
    BinaryExpression<LHST, RHST> operator!=(RHST value) const { return { m_lhs, value, EOperator::NOTEQUALS, static_cast<bool>(m_lhs != value) }; }

    template<typename RHST>
    BinaryExpression<LHST, RHST> operator<(RHST value) const { return { m_lhs, value, EOperator::LESS, static_cast<bool>(m_lhs < value) }; }

    template<typename RHST>
    BinaryExpression<LHST, RHST> operator>(RHST value) const { return { m_lhs, value, EOperator::GREATER, static_cast<bool>(m_lhs > value) }; }

    template<typename RHST>
    BinaryExpression<LHST, RHST> operator<=(RHST value) const { return { m_lhs, value, EOperator::LESSEQUAL, static_cast<bool>(m_lhs <= value) }; }

    template<typename RHST>
    BinaryExpression<LHST, RHST> operator>=(RHST value) const { return { m_lhs, value, EOperator::GREATEREQUAL, static_cast<bool>(m_lhs >= value) }; }

    // Operators == and != for c-strings
    BinaryExpression<LHST, char const*> operator==(char const* value) const { return { m_lhs, value, EOperator::EQUALS, static_cast<bool>(m_lhs == std::string(value)) }; }
    BinaryExpression<LHST, char const*> operator!=(char const* value) const { return { m_lhs, value, EOperator::NOTEQUALS, static_cast<bool>(m_lhs != std::string(value)) }; }

protected:
    template<typename T = LHST>
    typename std::enable_if<!is_printable<T>::value, std::string>::type
    GetExpandedExpression() const { return "<?>"; }

    template<typename T = LHST>
    typename std::enable_if<is_printable<T>::value && !is_c_str<T>::value && !std::is_same_v<T, std::string>, std::string>::type
    GetExpandedExpression() const { std::ostringstream oss; oss << std::boolalpha << m_lhs; return oss.str(); }

    template<typename T = LHST>
    typename std::enable_if_t<is_c_str<T>::value || std::is_same_v<T, std::string>, std::string>
    GetExpandedExpression() const { std::ostringstream oss; oss << std::boolalpha << std::quoted(m_lhs); return oss.str(); }

    template<typename T = LHST>
    typename std::enable_if<!is_c_str<T>::value && !std::is_class<T>::value, bool>::type
    GetSucceeded() const { return static_cast<bool>(m_lhs); }

    template<typename T = LHST>
    typename std::enable_if<is_c_str<T>::value || std::is_same<T, std::string>::value, bool>::type
    GetSucceeded() const { return !std::string(m_lhs).empty(); }

    template<typename T = LHST>
    typename std::enable_if<std::is_class<T>::value && !std::is_same<T, std::string>::value, bool> ::type
    GetSucceeded() const { return false; }

private:
    LHST m_lhs{};
};

}
