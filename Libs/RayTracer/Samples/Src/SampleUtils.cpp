#include "SampleUtils.h"

#include <RayTracer/Camera.h>
#include <RayTracer/Canvas.h>
#include <RayTracer/World.h>
#include <VulkApp/Application.h>

#include <chrono>
#include <iostream>
#include <fstream>

namespace SampleUtils
{

void ShowCanvas(Canvas const& canvas)
{
    VulkanApplication vkApp;

    try
    {
        vkApp.Init();

        auto const w = canvas.Width();
        auto const h = canvas.Height();
        unsigned char *pixels = new unsigned char[w * h * 4];
        for (int j = 0; j < h; j++)
        {
            for (int i = 0; i < w; i++)
            {
                for (auto p = 0; p < 4; p++)
                {
                    pixels[(j * w * 4) + (i * 4) + p] = (unsigned char)Clamp(std::roundf(canvas.PixelAt(i, j)[p] * 255.f), 0.f, 255.f);
                }
            }
        }
        vkApp.UpdateTexture(pixels, canvas.Width(), canvas.Height());
        delete[] pixels;

        vkApp.Run();
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void SaveCanvas(Canvas const& canvas, std::string const& outputFile)
{
    std::ofstream ppmFile;
    ppmFile.open(outputFile);
    ppmFile << canvas.GetAsPPM();
    ppmFile.close();
}

void RenderScene(Camera const& camera, World const& world, std::string const& outputFile)
{
    std::cout << "Rendering scene..." << std::endl;
    using hrc = std::chrono::high_resolution_clock;
    auto t1 = hrc::now();
    auto const canvas = camera.Render(world);
    auto t2 = hrc::now();

    auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Scene rendered in " << time_span.count() << " seconds." << std::endl;

#ifdef SAVE_SCENE_AS_PPM
    if (!outputFile.empty())
    {
        std::cout << "Saving scene to ppm file " << outputFile << " ..." << std::endl;
        SaveCanvas(canvas, outputFile);
    }
    else
#endif
    {
        std::cout << "Displaying scene with vulkan renderer ..." << std::endl;
        ShowCanvas(canvas);
    }
}

}
