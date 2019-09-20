#include <RayTracer/Camera.h>
#include <RayTracer/Canvas.h>
#include <RayTracer/Intersection.h>
#include <RayTracer/Lighting.h>
#include <RayTracer/Ray.h>
#include <RayTracer/Shapes/Plane.h>
#include <RayTracer/Shapes/Sphere.h>
#include <RayTracer/Transformations.h>

#include <SampleUtils.h>

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

    SampleUtils::RenderScene(camera, world, "Ch9_plane.ppm");

    return 0;
}
