#include <RayTracer/Camera.h>
#include <RayTracer/Pattern.h>
#include <RayTracer/Shapes/Cone.h>
#include <RayTracer/Shapes/Cube.h>
#include <RayTracer/Shapes/Sphere.h>
#include <RayTracer/Transformations.h>
#include <RayTracer/World.h>

#include <SampleUtils.h>

int main()
{
    auto world = World();

    auto const lightPos = Point(0.f, 18.f, 0.f);
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
        m.Ambient(.5f);
        auto p = std::make_shared<CheckerPattern>(Color::Black(), Color(.6f, .6f, .6f));
        p->SetTransform(matrix::Scaling(.05f, .05f, .05f));
        m.Pattern(p);
        world.Add(floorAndRoof);
    }

    // Walls
    {
        auto walls = std::make_shared<Cube>();
        walls->SetTransform(matrix::Translation(0.f, 10.f, 0.f) * matrix::Scaling(30.f, 100.f, 30.f));
        auto& m = walls->ModifyMaterial();
        m.Specular(0.1f);
        m.Ambient(.5f);
        auto p = std::make_shared<StripPattern>(Color(.6f, .3f, .1f), Color(.5f, .2f, .05f));
        p->SetTransform(matrix::Scaling(.1f, .1f, .1f));
        m.Pattern(p);
        world.Add(walls);
    }

    // Lamp
    {
        auto lamp = std::make_shared<Cylinder>(-1.f, 0.f, true);
        lamp->SetTransform(matrix::Translation(lightPos.X(), lightPos.Y() + 2.f, lightPos.Z()));
        lamp->ModifyMaterial().SetColor(Color::Yellow());
        lamp->ModifyMaterial().Reflective(.1f);
        world.Add(lamp);

        auto lampHat = std::make_shared<Cone>(-1.5f, -0.25f, false);
        lampHat->SetTransform(matrix::Translation(lightPos.X(), lightPos.Y() + 1.35f, lightPos.Z()) * matrix::Scaling(2.5f, 1.f, 2.5f));
        lampHat->ModifyMaterial().SetColor(Color::Yellow());
        lampHat->ModifyMaterial().Reflective(.1f);
        world.Add(lampHat);
    }

    // Light bulb
    {
        auto bulb = std::make_shared<Sphere>();
        bulb->SetTransform(matrix::Translation(lightPos.X(), lightPos.Y(), lightPos.Z()) * matrix::Scaling(.4f, .6f, .4f));
        auto& m = bulb->ModifyMaterial();
        bulb->CastShadows(false);
        m.Ambient(1.f);
        m.SetColor(Color::White());
        world.Add(bulb);
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
        top->SetTransform(matrix::Translation(-5.f, 6.45f, 0.f) * matrix::Scaling(5.25f, .5f, 8.25f));
        top->ModifyMaterial().SetColor(Color::Red());
        top->ModifyMaterial().Ambient(.25f);
        top->ModifyMaterial().Reflective(.25f);
        world.Add(top);
    }

    auto camera = Camera(800, 600, PI / 3.f);
    camera.SetTransform(matrix::View(Point(29.f, 10.f, -29.f), Point(-5.f, 9.f, 0.f), Vector(0.f, 1.f, 0.f)));

    SampleUtils::RenderScene(camera, world, "Ch13_CylindersAndCones.ppm");

    return 0;
}
