#include <Canvas.h>
#include <Intersection.h>
#include <Ray.h>
#include <Sphere.h>
#include <Transformations.h>

#include <fstream>
#include <iostream>

int main()
{
    uint32_t const canvasPixels = 100;
    auto const rayOrigin = Point(0.f, 0.f, -5.f);
    float const wallZ = 10.f;
    float const wallSize = 7.f;
    float const wallHalfSize = wallSize * 0.5f;
    float const pixelSize = wallSize / (float)canvasPixels;
    Color const red(1.f, 0.f, 0.f);
    auto sphere = std::make_shared<Sphere>();
    sphere->SetTransform(matrix::RotationZ(PI/4.f) * matrix::Scaling(0.5, 1.f, 1.f));

    Canvas canvas(canvasPixels, canvasPixels);
    Ray r(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 0.f));

    for (uint32_t y = 0; y < canvasPixels; y++)
    {
        float const worldY = wallHalfSize - pixelSize * (float)y;
        for (uint32_t x = 0; x < canvasPixels; x++)
        {
            float const worldX = -wallHalfSize + pixelSize * (float)x;

            auto const pointInWall = Point(worldX, worldY, wallZ);
            auto const r = Ray(rayOrigin, (pointInWall - rayOrigin));
            std::vector<Intersection> xs;
            r.Intersect(sphere, xs);

            if (Hit(xs).Object() != nullptr)
            {
                canvas.WritePixel(x, y, red);
            }
        }
    }

    std::ofstream ppmFile;
    ppmFile.open("SphereSilhouette.ppm");
    ppmFile << canvas.GetAsPPM();
    ppmFile.close();

    return 0;
}
