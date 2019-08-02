#pragma once

#include "./BeddevImpl.h"

#define PSCENARIO(desc, cat, args) BEDDEV_PSCENARIO(desc, cat, args)
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
    return beddev::TestRunner::Get().RunAll(std::cout);
}
#endif
