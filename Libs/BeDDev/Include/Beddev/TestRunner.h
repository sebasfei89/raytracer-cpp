#pragma once

#include "beddev_export.h"

#include <ostream>
#include <vector>

namespace beddev
{

class ITestCase;

class TestRunner
{
public:
    static BEDDEV_EXPORT TestRunner& Get();

    BEDDEV_EXPORT int RunAll(std::ostream& os);

    BEDDEV_EXPORT void Register(ITestCase* testCase);

private:
    TestRunner();
    ~TestRunner();

    void OutputSummary(std::ostream& os, uint32_t col1w, uint32_t col2w, uint32_t col3w, std::string const& header, uint32_t total, uint32_t passed, uint32_t failed) const;

    std::vector<ITestCase*> m_testCases;
    bool m_isRunning;
};

}
