#include <RayTracer/Camera.h>
#include <RayTracer/World.h>

#include <SampleUtils.h>
#include <fstream>

int main()
{
    auto world = World();
    std::ifstream ifs("Resources/Ch14_groups.scene.json");
    world.Load(ifs);

    auto camera = Camera(800, 600, PI / 3.f);
    camera.SetTransform(matrix::View(Point(0.f, 3.f, -3.f), Point(0.f, -1.f, 1.f), Vector(0.f, 1.f, 0.f)));

    SampleUtils::RenderScene(camera, world, "Ch14_Groups.ppm");

    return 0;
}
