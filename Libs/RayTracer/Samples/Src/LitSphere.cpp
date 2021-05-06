#include <RayTracer/Canvas.h>
#include <RayTracer/Intersection.h>
#include <RayTracer/Lighting.h>
#include <RayTracer/Ray.h>
#include <RayTracer/Shapes/Sphere.h>
#include <RayTracer/Transformations.h>

#include <SampleUtils.h>

int main()
{
    uint32_t const canvasPixels = 400;
    auto const rayOrigin = Point(0.f, 0.f, -5.f);
    float const wallZ = 10.f;
    float const wallSize = 7.f;
    float const wallHalfSize = wallSize * 0.5f;
    float const pixelSize = wallSize / (float)canvasPixels;
    auto sphere = std::make_shared<Sphere>();
    Material m;
    m.SetColor({ 1.f, 0.2f, 1.f });
    sphere->SetMaterial(m);
    PointLight const light(Point(-10.f, 10.f, -10.f), {1.f, 1.f, 1.f});

    Canvas canvas(canvasPixels, canvasPixels);
    Ray r(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 0.f));

    for (uint32_t y = 0; y < canvasPixels; y++)
    {
        float const worldY = wallHalfSize - pixelSize * (float)y;
        for (uint32_t x = 0; x < canvasPixels; x++)
        {
            float const worldX = -wallHalfSize + pixelSize * (float)x;

            auto const pointInWall = Point(worldX, worldY, wallZ);
            auto const rayDir = (pointInWall - rayOrigin).Normalized();
            auto const ray = Ray(rayOrigin, rayDir);
            std::vector<Intersection> xs;
            ray.Intersect(sphere, xs);

            auto const hit = Hit(xs);
            if (hit.Object() != nullptr)
            {
                auto const hitPos = ray.Position(hit.Distance());
                auto const normal = hit.Object()->NormalAt(hitPos);
                auto const eye = -ray.Direction();
                auto const color = Lighting(hit.Object()->GetMaterial(), hit.Object(), light, hitPos, eye, normal);
                canvas.WritePixel(x, y, color);
            }
        }
    }

    SampleUtils::RenderScene(canvas, "LitSphere.ppm");

    return 0;
}
