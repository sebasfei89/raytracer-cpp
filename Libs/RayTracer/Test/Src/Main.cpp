#include <Beddev/TestRunner.h>
#define CATCH_CONFIG_RUNNER
#include <Catch2/catch.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    int res = Catch::Session().run(argc, argv);
    res += beddev::TestRunner::Get().RunAll(std::cout);
    return res;
}
