#include <Camera.h>
#include <Canvas.h>
#include <Intersection.h>
#include <Lighting.h>
#include <Plane.h>
#include <Ray.h>
#include <Sphere.h>
#include <Transformations.h>

#include <chrono>
#include <fstream>
#include <iostream>

int main()
{
    auto floor = std::make_shared<Plane>();
    {
        auto& material = floor->ModifyMaterial();
        material.SetColor({ 1.f, .9f, .9f });
        material.Specular(0.f);
    }

    auto roof = std::make_shared<Plane>();
    {
        roof->SetTransform(matrix::Translation(0.f, 10.f, 0.f));
        auto& material = roof->ModifyMaterial();
        material.SetColor({ .2f, .9f, .1f });
        material.Specular(0.1f);
    }

    auto middleSphere = std::make_shared<Sphere>();
    {
        middleSphere->SetTransform(matrix::Translation(-.5f, 1.f, .5f));
        auto& material = middleSphere->ModifyMaterial();
        material.SetColor({ .1f, 1.f, .5f });
        material.Diffuse(.7f);
        material.Specular(.3f);
    }

    auto rightSphere = std::make_shared<Sphere>();
    {
        rightSphere->SetTransform(matrix::Translation(1.5f, .5f, -.5f) * matrix::Scaling(.5f, .5f, .5f));
        auto& material = rightSphere->ModifyMaterial();
        material.SetColor({ .5f, 1.f, .1f });
        material.Diffuse(.7f);
        material.Specular(.3f);
    }

    auto leftSphere = std::make_shared<Sphere>();
    {
        leftSphere->SetTransform(matrix::Translation(-1.5f, .33f, -.75f) * matrix::Scaling(.33f, .33f, .33f));
        auto& material = leftSphere->ModifyMaterial();
        material.SetColor({ 1.f, .8f, .1f });
        material.Diffuse(.7f);
        material.Specular(.3f);
    }

    auto world = World();
    world.Add(PointLight(Point(-5.f, 5.f, -2.f), { 1.f, 1.f, 1.f }));
    world.Add(floor);
    world.Add(roof);
    world.Add(middleSphere);
    world.Add(leftSphere);
    world.Add(rightSphere);

    auto camera = Camera(800, 600, PI / 3.f);
    camera.SetTransform(matrix::View(Point(5.f, 2.5f, -5.f), Point(-3.f, 2.2f, 0.f), Vector(0.f, 1.f, 0.f)));

    std::cout << "Rendering scene..." << std::endl;
    using hrc = std::chrono::high_resolution_clock;
    auto t1 = hrc::now();
    auto const canvas = camera.Render(world);
    auto t2 = hrc::now();

    auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Scene rendered in " << time_span.count() << " seconds." << std::endl;

    std::ofstream ppmFile;
    ppmFile.open("Ch9_plane.ppm");
    ppmFile << canvas.GetAsPPM();
    ppmFile.close();

    return 0;
}
