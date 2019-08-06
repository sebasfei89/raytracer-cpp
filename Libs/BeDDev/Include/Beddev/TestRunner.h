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

    BEDDEV_EXPORT int RunAll(std::ostream& os, std::string const& filter="");

    BEDDEV_EXPORT void Register(ITestCase* testCase);

protected:
    BEDDEV_EXPORT TestRunner();
    BEDDEV_EXPORT ~TestRunner();

private:
    void OutputSummary(std::ostream& os, uint32_t col1w, uint32_t col2w, uint32_t col3w, std::string const& header, uint32_t total, uint32_t passed, uint32_t failed, uint32_t skipped=0) const;

    std::vector<ITestCase*> m_testCases;
    bool m_isRunning;
};

}
