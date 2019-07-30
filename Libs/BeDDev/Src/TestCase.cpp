#include "TestCase.h"

#include "ColoredOutput.h"
#include "Expression.h"
#include "TestRunner.h"

#include <iomanip>
#include <numeric>
#include <ostream>

#define FORMAT_OUTPUT(OS, V, T)\
    if (V.size() > 0)\
    {\
        bool isFirst = true;\
        for (auto const& i : V)\
        {\
            OS << std::setw(11) << (isFirst ? T": " : "And: ") << i << std::endl;\
            isFirst = false;\
        }\
    }

namespace beddev
{

std::ostream& operator<<(std::ostream& os, TestCase::Assertion const& a)
{
    os << a.test;
    return os;
}

TestCase::TestCase(std::string const& desc, std::string const& fileAndLine, std::string const& category)
    : m_description(desc)
    , m_category(category)
    , m_fileAndLine(fileAndLine)
    , m_facts()
    , m_actions()
    , m_tests()
{
    TestRunner::Get().Register(this);
}

bool TestCase::Run(std::ostream& os)
{
    bool succeed = false;
    try
    {
        succeed = RunImpl();
    }
    catch (std::exception const&)
    {
        succeed = false;
    }
    catch (...)
    {
        succeed = false;
    }

    if (!succeed)
    {
        os << std::setw(80) << std::setfill('-') << "-" << std::endl << std::setfill(' ');
        os << "Scenario: " << m_description;
        if (!m_category.empty()) os << "  [" << m_category << "]";
        os << std::endl;
        FORMAT_OUTPUT(os, m_facts, "Given");
        FORMAT_OUTPUT(os, m_actions, "When");
        FORMAT_OUTPUT(os, m_tests, "Then");
        os << std::setw(80) << std::setfill('-') << "-" << std::endl << std::setfill(' ');
        os << TestingImpl::Colour(TestingImpl::ColorCode::FileName) << m_fileAndLine << std::endl;

        for (auto const& a : m_tests)
        {
            if (a.result) continue;
            os << std::setw(80) << std::setfill('.') << "." << std::endl << std::setfill(' ') << std::endl;
            os << TestingImpl::Colour(TestingImpl::ColorCode::FileName) << a.file << "(" << a.line << "): "
                << TestingImpl::Colour(TestingImpl::ColorCode::Error) << "FAILED:" << std::endl
                << TestingImpl::Colour(TestingImpl::ColorCode::OriginalExpression) << "  " << a.test << std::endl
                << TestingImpl::Colour(TestingImpl::ColorCode::White) << "with expansion:" << std::endl
                << TestingImpl::Colour(TestingImpl::ColorCode::ReconstructedExpression) << "  " << a.expanded << std::endl;
        }
    }
    return succeed;
}

TestCase::AssertionsSummary TestCase::Assertions() const
{
    AssertionsSummary summary{ 0, 0 };
    return std::reduce(m_tests.begin(), m_tests.end(), summary, [](AssertionsSummary summary, Assertion const& a)
    {
        if (a.result)
        {
            std::get<0>(summary)++;
        }
        else
        {
            std::get<1>(summary)++;
        }
        return summary;
    });
}

bool TestCase::AddTest(std::string const& test, std::string const& file, long line, IExpression const& expr)
{
    m_tests.push_back({ test, file, line, expr.Succeeded(), expr.ExpandedExpression() });
    return expr.Succeeded();
}

} // EON beddev
