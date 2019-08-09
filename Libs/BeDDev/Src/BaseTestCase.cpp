#include "BaseTestCase.h"

#include "ColoredOutput.h"
#include "Expression.h"
#include "SessionSummary.h"
#include "TestRunner.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <ostream>
#include <regex>

namespace beddev
{

namespace
{

std::vector<std::string> Split(std::string const& list, char delimiter)
{
    if (list.empty())
    {
        return {};
    }

    std::vector<std::string> tokens;
    std::size_t next = 0;
    do
    {
        std::size_t sep = list.find_first_of(delimiter, next);
        auto token = list.substr(next, sep);
        if (!token.empty())
        {
            tokens.push_back(token);
        }
        next = (sep != std::string::npos) ? sep + 1 : sep;
    } while (next != std::string::npos);

    return tokens;
}

}
BaseTestCase::Assertion::Assertion(std::string const& pexpr, std::string const& pfile, long pline, IExpression const& expression)
    : test(pexpr)
    , file(pfile)
    , line(pline)
    , result(expression.Succeeded())
    , expanded(expression.ExpandedExpression())
{
}

BaseTestCase::BaseTestCase(std::string const& desc, std::string const& file, long line, std::string const& tags)
    : m_description(desc)
    , m_tags(tags)
    , m_tagList(Split(tags, ','))
    , m_file(file)
    , m_line(line)
{
    TestRunner::Get().Register(this);
}

bool BaseTestCase::CanRun(std::string const& filter, std::string const& tags) const
{
    if (!filter.empty() && !std::regex_match(GetDescription(), std::regex(filter)))
    {
        return false;
    }

    if (tags.empty())
    {
        return true;
    }

    for (auto const& tag : Split(tags, ','))
    {
        if (std::find(m_tagList.begin(), m_tagList.end(), tag) != m_tagList.end())
        {
            return true;
        }
    }

    return false;
}

void BaseTestCase::Run(std::ostream& os, SessionSummary& summary, bool outputOnSuccess)
{
    try
    {
        RunInternal(os, summary, outputOnSuccess);
    }
    catch (std::exception const&)
    {
        // TODO: summary.nonAssertionFailures++;
        // ReportFailure(os);
    }
    catch (...)
    {
        // TODO: summary.nonAssertionFailures++;
        // ReportFailure(os);
    }
}

void BaseTestCase::FormatHeader(std::ostream& os) const
{
    os << std::setw(80) << std::setfill('-') << "-" << std::endl << std::setfill(' ');
    os << "Scenario: " << m_description;
    
    auto commaConcat = [](std::string a, std::string const& b) { return std::move(a) + ", " + b; };
    if (!m_tagList.empty())
    {
        os << "  [" << std::accumulate(std::next(m_tagList.begin()), m_tagList.end(), m_tagList[0], commaConcat) << "]";
    }
    os << std::endl;
}

void BaseTestCase::FormatFileAndLineInfo(std::ostream& os) const
{
    os << std::setw(80) << std::setfill('-') << "-" << std::endl << std::setfill(' ');
    os << TestingImpl::Colour(TestingImpl::ColorCode::FileName) << m_file << "(" << m_line << ")" << std::endl;
}

void BaseTestCase::FormatItems(std::ostream& os, std::vector<std::string> const& items, std::string const& tag) const
{
    bool isFirst = true;
    for (auto const& i : items)
    {
        os << std::setw(8) << (isFirst ? tag : "And") << ": " << i << std::endl;
        isFirst = false;
    }
}

void BaseTestCase::FormatAssertions(std::ostream& os, std::vector<Assertion> const& items, std::string const& tag) const
{
    bool isFirst = true;
    for (auto const& i : items)
    {
        os << std::setw(8) << (isFirst ? tag : "And") << ": " << i.test << std::endl;
        isFirst = false;
    }
}

void BaseTestCase::FormatFailedTests(std::ostream& os, std::vector<Assertion> const& assertions, bool outputOnSuccess) const
{
    for (auto const& a : assertions)
    {
        if (a.result && !outputOnSuccess) continue;

        os << std::setw(80) << std::setfill('.') << "." << std::endl << std::setfill(' ') << std::endl
           << TestingImpl::Colour(TestingImpl::ColorCode::FileName) << a.file << "(" << a.line << "): ";
        
        FormatAssertionMessage(os, a.result);

        os << TestingImpl::Colour(TestingImpl::ColorCode::OriginalExpression) << "  " << a.test << std::endl;

        if (a.expanded != a.test)
        {
            os << TestingImpl::Colour(TestingImpl::ColorCode::White) << "with expansion:" << std::endl
               << TestingImpl::Colour(TestingImpl::ColorCode::ReconstructedExpression) << "  " << a.expanded << std::endl;
        }
    }
}

void BaseTestCase::FormatAssertionMessage(std::ostream& os, bool assertionResult) const
{
    if (assertionResult)
    {
        os << TestingImpl::Colour(TestingImpl::ColorCode::Success) << "SUCCEEDED:";
    }
    else
    {
        os << TestingImpl::Colour(TestingImpl::ColorCode::Error) << "FAILED:";
    }
    os << std::endl;
}

void BaseTestCase::ReportConfigFailure(std::ostream& os, SessionSummary& summary, std::string const& errorMsg) const
{
    FormatHeader(os);
    FormatFileAndLineInfo(os);
    os << std::setw(80) << std::setfill('.') << "." << std::endl << std::setfill(' ') << std::endl;
    os << "  " << errorMsg << std::endl << std::endl;
    summary.failed++;
    summary.configFailures++;
}

} // EoF namespace beddev
