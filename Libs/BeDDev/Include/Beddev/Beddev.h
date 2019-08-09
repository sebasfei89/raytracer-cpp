#pragma once

#include "./BeddevImpl.h"

#define PSCENARIO(ARGT, DESC, ...) BEDDEV_PSCENARIO(ARGT, DESC, __VA_ARGS__)
#define PARAMS(...) BEDDEV_PARAMS(__VA_ARGS__)

#define SCENARIO(desc, ...) BEDDEV_SCENARIO(desc, __VA_ARGS__)
#define GIVEN(...) BEDDEV_GIVEN(__VA_ARGS__)
#define WHEN(...) BEDDEV_WHEN(__VA_ARGS__)
#define THEN(...) BEDDEV_THEN(__VA_ARGS__)
#define REQUIRE(...) THEN(__VA_ARGS__)

#if defined(BEDDEV_MAIN)
#include <iostream>
int main(int argc, char** argv)
{
    beddev::TestRunner::Config config = { "", "", false };
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-t")
        {
            if (((i + 1) < argc))
            {
                config.m_tags += argv[++i];
            }
        }
        else if (std::string(argv[i]) == "-v")
        {
            config.m_fullReport = true;
        }
        else
        {
            config.m_filter = argv[i];
        }
    }
    return beddev::TestRunner::Get().RunAll(std::cout, config);
}
#endif
