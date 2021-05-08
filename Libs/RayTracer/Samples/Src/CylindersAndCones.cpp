#include <RayTracer/Camera.h>
#include <RayTracer/World.h>

#include <SampleUtils.h>

#include <fstream>

int main()
{
    auto world = World();
    world.Load(std::ifstream("Resources/Ch13_CylindersAndCones.scene.json"));

    auto camera = Camera(800, 600, PI / 3.f);
    camera.SetTransform(matrix::View(Point(29.f, 10.f, -29.f), Point(-5.f, 9.f, 0.f), Vector(0.f, 1.f, 0.f)));

    SampleUtils::RenderScene(camera, world, "Ch13_CylindersAndCones.ppm");

    return 0;
}
