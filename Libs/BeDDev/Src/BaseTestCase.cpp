#include "BaseTestCase.h"

#include "ColoredOutput.h"
#include "TestRunner.h"

#include <iomanip>
#include <ostream>

namespace beddev
{

BaseTestCase::Assertion::Assertion(std::string const& pexpr, std::string const& pfile, long pline, bool presult, std::string const& pexpanded, std::string const& perrorMsg)
    : test(pexpr)
    , file(pfile)
    , line(pline)
    , result(presult)
    , expanded(pexpanded)
    , errorMsg(perrorMsg)
{
}

BaseTestCase::BaseTestCase(std::string const& desc, std::string const& file, long line, std::string const& category)
    : m_description(desc)
    , m_category(category)
    , m_file(file)
    , m_line(line)
{
    TestRunner::Get().Register(this);
}

void BaseTestCase::Run(std::ostream& os, SessionSummary& summary)
{
    try
    {
        RunInternal(os, summary);
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
    if (!m_category.empty()) os << "  [" << m_category << "]";
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

void BaseTestCase::FormatFailedTests(std::ostream& os, std::vector<Assertion> const& assertions) const
{
    for (auto const& a : assertions)
    {
        if (a.result) continue;

        os << std::setw(80) << std::setfill('.') << "." << std::endl << std::setfill(' ') << std::endl
           << TestingImpl::Colour(TestingImpl::ColorCode::FileName) << a.file << "(" << a.line << "): "
           << TestingImpl::Colour(TestingImpl::ColorCode::Error) << a.errorMsg << std::endl
           << TestingImpl::Colour(TestingImpl::ColorCode::OriginalExpression) << "  " << a.test << std::endl;

        if (a.expanded != a.test)
        {
            os << TestingImpl::Colour(TestingImpl::ColorCode::White) << "with expansion:" << std::endl
               << TestingImpl::Colour(TestingImpl::ColorCode::ReconstructedExpression) << "  " << a.expanded << std::endl;
        }
    }
}

} // EoF namespace beddev
