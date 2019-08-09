#include "SessionSummary.h"

#include "ColoredOutput.h"

#include <iomanip>

namespace beddev
{

namespace
{

uint32_t GetMaxWidth(std::initializer_list<uint32_t> args)
{
    uint32_t tmp = 1;
    for (auto i : args)
    {
        for (uint32_t exp = 1; ; exp++)
        {
            auto upper = (uint32_t)std::pow(10, exp);
            if (i < upper)
            {
                if (tmp < exp) tmp = exp;
                break;
            }
        }
    }
    return tmp;
}

} // EoF anonymous namespace

void SessionSummary::Print(std::ostream& os) const
{
    uint32_t const ranTestCases = passed + failed;

    if (ranTestCases == 0)
    {
        os << TestingImpl::Colour(TestingImpl::ColorCode::Warning) << "No tests ran" << std::endl;
    }
    else if (ranTestCases == passed)
    {
        os << TestingImpl::Colour(TestingImpl::ColorCode::ResultSuccess)
            << std::setw(80) << std::setfill('=') << "=" << std::endl
            << "All tests passed"
            << TestingImpl::Colour(TestingImpl::ColorCode::White)
            << " (" << passedAssertions << " assertion" << ((passedAssertions != 1) ? "s" : "")
            << " in " << passed << " test case" << ((passed != 1) ? "s" : "");
        if (skipped > 0)
        {
            os << ", " << skipped << " test case" << ((skipped != 1) ? "s" : "") << " skipped";
        }
        os << ")" << std::endl;
    }
    else
    {
        uint32_t const totalAssertions = passedAssertions + failedAssertions;
        float const factor = 80.f / (float)(totalAssertions + configFailures);
        os << TestingImpl::Colour(TestingImpl::ColorCode::ResultError)
            << std::setw((int)(std::roundf((float)(failedAssertions + configFailures) * factor))) << std::setfill('=') << "=";
        if (passedAssertions > 0)
        {
            os << TestingImpl::Colour(TestingImpl::ColorCode::ResultSuccess)
                << std::setw((int)(std::roundf((float)passedAssertions * factor))) << std::setfill('=') << "=";
        }
        os << std::setfill(' ') << std::endl;
        uint32_t const allTestCases = ranTestCases + skipped;
        int const col1w = GetMaxWidth({ allTestCases, totalAssertions });
        int const col2w = GetMaxWidth({ passed, passedAssertions });
        int const col3w = GetMaxWidth({ failed, failedAssertions });
        Print(os, col1w, col2w, col3w, "test cases", allTestCases, passed, failed, skipped);
        Print(os, col1w, col2w, col3w, "assertions", totalAssertions, passedAssertions, failedAssertions);
    }
}

void SessionSummary::Print(std::ostream& os, uint32_t col1w, uint32_t col2w, uint32_t col3w, std::string const& header, uint32_t total, uint32_t passed, uint32_t failed, uint32_t skipped) const
{
    os << TestingImpl::Colour(TestingImpl::ColorCode::White)
        << header << ": " << std::setw(col1w) << total
        << TestingImpl::Colour(TestingImpl::ColorCode::SecondaryText) << " | "
        << TestingImpl::Colour(TestingImpl::ColorCode::Success) << std::setw(col2w) << passed << " passed"
        << TestingImpl::Colour(TestingImpl::ColorCode::SecondaryText) << " | "
        << TestingImpl::Colour(TestingImpl::ColorCode::Error) << std::setw(col3w) << failed << " failed";
    if (skipped > 0)
    {
        os << TestingImpl::Colour(TestingImpl::ColorCode::SecondaryText) << " | "
            << TestingImpl::Colour(TestingImpl::ColorCode::Cyan) << skipped << " skipped";
    }
    os << std::endl;
}

} // EoF namespace beddev
