#pragma once

#include "beddev_export.h"

#include <vector>

namespace beddev
{

class ITestCase;

class TestRunner
{
public:
    static BEDDEV_EXPORT TestRunner& Get();

    BEDDEV_EXPORT int RunAll(std::ostream& os, std::string const& filter = "");

    BEDDEV_EXPORT void Register(ITestCase* testCase);

protected:
    BEDDEV_EXPORT TestRunner();
    BEDDEV_EXPORT ~TestRunner();

private:
    std::vector<ITestCase*> m_testCases;
    bool m_isRunning;
};

}
