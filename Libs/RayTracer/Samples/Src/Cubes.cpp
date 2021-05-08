#include <RayTracer/Camera.h>
#include <RayTracer/Pattern.h>
#include <RayTracer/Shapes/Cube.h>
#include <RayTracer/Shapes/Sphere.h>
#include <RayTracer/Transformations.h>
#include <RayTracer/World.h>

#include <SampleUtils.h>

int main()
{
    auto world = World();

    auto const lightPos = Point(0.f, 19.f, 0.f);
    world.Add(PointLight(lightPos, { 1.f, 1.f, 1.f }));

    // Mirror
    {
        auto mirror = std::make_shared<Cube>();
        mirror->SetTransform(matrix::Translation(-29.9f, 10.f, 0.f) * matrix::RotationZ(PI * .5f) * matrix::Scaling(6.f, .1f, 20.f));
        auto& m = mirror->ModifyMaterial();
        m.SetColor(Color::Black());
        m.Diffuse(.9f);
        m.Specular(.9f);
        m.Shininess(300.f);
        m.Reflective(1.f);
        world.Add(mirror);
    }

    // Floor && roof
    {
        auto floorAndRoof = std::make_shared<Cube>();
        floorAndRoof->SetTransform(matrix::Translation(0.f, 10.f, 0.f) * matrix::Scaling(100.f, 10.f, 100.f));
        auto& m = floorAndRoof->ModifyMaterial();
        m.Specular(0.1f);
        m.Ambient(.25f);
        auto p = std::make_shared<CheckerPattern>(Color::Black(), Color(.6f, .6f, .6f));
        p->SetTransform(matrix::Scaling(.2f, .2f, .2f));
        m.Pattern(p);
        world.Add(floorAndRoof);
    }

    // Walls
    {
        auto walls = std::make_shared<Cube>();
        walls->SetTransform(matrix::Translation(0.f, 10.f, 0.f) * matrix::Scaling(30.f, 100.f, 30.f));
        auto& m = walls->ModifyMaterial();
        m.Specular(0.1f);
        m.Ambient(.25f);
        auto p = std::make_shared<StripPattern>(Color(.6f, .3f, .1f), Color(.5f, .2f, .05f));
        p->SetTransform(matrix::Scaling(.1f, .1f, .1f));
        m.Pattern(p);
        world.Add(walls);
    }

    // Lamp
    {
        auto lamp = std::make_shared<Sphere>();
        lamp->SetTransform(matrix::Translation(lightPos.X(), lightPos.Y(), lightPos.Z()) * matrix::Scaling(2.f, .5f, 2.f));
        lamp->CastShadows(false);
        auto& m = lamp->ModifyMaterial();
        m.SetColor(Color::Yellow());
        m.Ambient(1.f);
        m.Specular(0.f);
        m.Diffuse(0.f);
        world.Add(lamp);
    }

    // Table
    {
        std::vector<std::pair<float, float>> legsPos = {
            {-10.f, -8.f},
            {  0.f, -8.f},
            {  0.f,  8.f},
            {-10.f,  8.f},
        };
        for (auto pos2d : legsPos)
        {
            auto leg = std::make_shared<Cube>();
            leg->SetTransform(matrix::Translation(pos2d.first, 3.f, pos2d.second) * matrix::Scaling(.25f, 3.f, .25f));
            leg->ModifyMaterial().SetColor(Color::Red());
            leg->ModifyMaterial().Ambient(.25f);
            world.Add(leg);
        }

        auto top = std::make_shared<Cube>();
        top->SetTransform(matrix::Translation(-5.f, 6.25f, 0.f) * matrix::Scaling(5.25f, .5f, 8.25f));
        top->ModifyMaterial().SetColor(Color::Red());
        top->ModifyMaterial().Ambient(.25f);
        world.Add(top);
    }

    auto camera = Camera(1362, 638, PI / 3.f);
    camera.SetTransform(matrix::View(Point(29.f, 10.f, -29.f), Point(-5.f, 9.f, 0.f), Vector(0.f, 1.f, 0.f)));

    SampleUtils::RenderScene(camera, world, "Ch12_Cubes.ppm");

    return 0;
}
