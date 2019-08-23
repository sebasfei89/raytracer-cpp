#include "Application.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main(int argc, char** argv)
{
    VulkanApplication vkApp;

    try
    {
        vkApp.Run();
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
