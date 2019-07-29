#include "SampleUtils.h"

#include <RayTracer/Camera.h>
#include <RayTracer/Canvas.h>
#include <RayTracer/World.h>

#include <chrono>
#include <iostream>
#include <fstream>

namespace SampleUtils
{

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

    SaveCanvas(canvas, outputFile);
}

}
