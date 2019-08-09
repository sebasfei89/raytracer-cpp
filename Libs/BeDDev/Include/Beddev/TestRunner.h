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

    struct Config
    {
        std::string m_filter; // Run all tests which description matches the given regex.
        std::string m_tags;   // Comma separated list of tags. All test containing at least one tag in tags will be ran.
        bool m_fullReport;    // If true, print a full report even if all tests succeeded.
    };

    /*
     * \brief Run all tests registered, optionally filtering by description and/or tags.
     * \return The number of failed tests.
     */
    BEDDEV_EXPORT int RunAll(std::ostream& os, Config const& config = {"", "", false});

    BEDDEV_EXPORT void Register(ITestCase* testCase);

protected:
    BEDDEV_EXPORT TestRunner();
    BEDDEV_EXPORT ~TestRunner();

private:
    std::vector<ITestCase*> m_testCases;
    bool m_isRunning;
};

}
