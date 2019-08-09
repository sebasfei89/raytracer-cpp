#pragma once

#include "beddev_export.h"

#include <ostream>
#include <string>

namespace beddev
{

class SessionSummary
{
public:
    // Test cases
    uint32_t passed = 0;
    uint32_t failed = 0;
    uint32_t skipped = 0;

    // Assertions
    uint32_t failedAssertions = 0;
    uint32_t passedAssertions = 0;

    // Other failures such as no params provided to a parametrized test case
    uint32_t configFailures = 0;

    BEDDEV_EXPORT void Print(std::ostream& os) const;

private:
    void Print(std::ostream& os, uint32_t col1w, uint32_t col2w, uint32_t col3w, std::string const& header, uint32_t total, uint32_t passed, uint32_t failed, uint32_t skipped = 0) const;
};

} // EoF namespace beddev
