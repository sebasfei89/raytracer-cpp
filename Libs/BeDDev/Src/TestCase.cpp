#include "TestCase.h"

#include "Expression.h"
#include "SessionSummary.h"

namespace beddev
{

TestCase::TestCase(std::string const& desc, std::string const& file, long line, std::string const& category)
    : BaseTestCase(desc, file, line, category)
    , m_facts()
    , m_actions()
    , m_assertions()
{
}

void TestCase::RunInternal(std::ostream& os, SessionSummary& summary)
{
    if (RunImpl())
    {
        summary.passed++;
    }
    else
    {
        FormatHeader(os);
        ReportFailure(os);
        summary.failed++;
    }

    SummarizeAssertions(summary);
}

void TestCase::AddFact(std::string const& fact)
{
    m_facts.push_back(fact);
}

void TestCase::AddAction(std::string const& action)
{
    m_actions.push_back(action);
}

bool TestCase::AddTest(std::string const& test, std::string const& file, long line, IExpression const& expr)
{
    return AddTest({ test, file, line, expr.Succeeded(), expr.ExpandedExpression() });
}

bool TestCase::AddTest(Assertion const& assertion)
{
    m_assertions.push_back(assertion);
    return assertion.result;
}

void TestCase::ReportFailure(std::ostream& os) const
{
    FormatItems(os, m_facts, "Given");
    FormatItems(os, m_actions, "When");
    FormatAssertions(os, m_assertions, (m_facts.size() > 0 || m_actions.size() > 0) ? "Then" : "Require");
    FormatFileAndLineInfo(os);
    FormatFailedTests(os, m_assertions);
}

void TestCase::SummarizeAssertions(SessionSummary& summary) const
{
    for (auto const& assertion : m_assertions)
    {
        if (assertion.result)
        {
            summary.passedAssertions++;
        }
        else
        {
            summary.failedAssertions++;
        }
    }
}

} // EON beddev
