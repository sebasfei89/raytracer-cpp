#include <RayTracer/Camera.h>
#include <RayTracer/Pattern.h>
#include <RayTracer/Plane.h>
#include <RayTracer/Sphere.h>
#include <RayTracer/Transformations.h>
#include <RayTracer/World.h>

#include <SampleUtils.h>

int main()
{
    auto world = World();

    world.Add(PointLight(Point(-5.f, 5.f, -2.f), { 1.f, .9f, .8f }));

    // Floot
    {
        auto floor = std::make_shared<Plane>();
        auto& material = floor->ModifyMaterial();
        auto p1 = std::make_shared<StripPattern>(Color(.6f, .6f, .6f), Color(.2f, .2f, .2f));
        p1->SetTransform(matrix::RotationY(PI / 4.f) * matrix::Scaling(.25f, 1.f, 1.f));
        auto p2 = std::make_shared<StripPattern>(Color(1.f, .6f, .6f), Color(.4f, .0f, .0f));
        p2->SetTransform(matrix::RotationY(PI / -4.f) * matrix::Scaling(.25f, 1.f, 1.f));
        auto pRoot = std::make_shared<CheckerPattern>(p1, p2);
        pRoot->SetTransform(matrix::Scaling(.5f, .5f, .5f));
        material.Pattern(pRoot);
        material.Specular(0.1f);

        world.Add(floor);
    }

    // Roof
    {
        auto roof = std::make_shared<Plane>();
        roof->SetTransform(matrix::Translation(0.f, 7.5f, 0.f));
        auto& material = roof->ModifyMaterial();
        material.Pattern(std::make_shared<RingPattern>(Color(1.f, 0.f, 0.f), Color(1.f, .5f, 0.f)));
        material.Specular(0.5f);
        world.Add(roof);
    }

    // Back Wall
    {
        auto backWall = std::make_shared<Plane>();
        backWall->SetTransform(matrix::Translation(-10.f, 0.f, 20.f) * matrix::RotationX(PI / 2.f));
        auto& material = backWall->ModifyMaterial();
        material.SetColor(Color(.0f, .0f, .0f));
        material.Reflective(.5f);
        //material.Ambient(1.f);
        world.Add(backWall);
    }

    // Right Wall
    {
        auto rightWall = std::make_shared<Plane>();
        rightWall->SetTransform(matrix::Translation(4.f, 0.f, 0.f) * matrix::RotationZ(PI / 2.f));
        auto& material = rightWall->ModifyMaterial();
        auto p1 = std::make_shared<StripPattern>(Color::White(), Color(.0f, .6f, .0f));
        p1->SetTransform(matrix::RotationY(PI / 2.f) * matrix::Scaling(.5f, 1.f, 1.f));
        auto p2 = std::make_shared<StripPattern>(Color::White(), Color(.0f, .6f, .0f));
        p2->SetTransform(matrix::Scaling(.5f, 1.f, 1.f));
        auto pRoot = std::make_shared<BlendPattern>(p1, p2);
        pRoot->SetTransform(matrix::Scaling(.75f, .75f, .75f));
        material.Pattern(pRoot);
        material.Specular(0.f);
        world.Add(rightWall);
    }

    // Left Wall
    {
        auto leftWall = std::make_shared<Plane>();
        leftWall->SetTransform(matrix::Translation(-10.f, 0.f, 6.f) * matrix::RotationZ(PI / 2.f));
        auto& material = leftWall->ModifyMaterial();
        material.Pattern(std::make_shared<RadialGradientPattern>(Color(1.f, 0.f, 0.f), Color(1.f, 1.f, 0.f)));
        material.ModifyPattern()->SetTransform(matrix::Scaling(30.f, 1.f, 15.f));
        material.Ambient(1.f);
        material.Specular(0.f);
        material.Diffuse(0.f);
        world.Add(leftWall);
    }

    // Middle Sphere
    {
        auto middleSphere = std::make_shared<Sphere>();
        middleSphere->SetTransform(matrix::Translation(-.5f, 1.f, .5f));
        auto& material = middleSphere->ModifyMaterial();
        auto p1 = std::make_shared<StripPattern>(Color(0.f, 1.f, 0.f), Color(0.f, .25f, 0.f));
        auto pRoot = std::make_shared<PerlinNoisePattern>(p1);
        material.Pattern(pRoot);
        material.ModifyPattern()->SetTransform(matrix::RotationZ(PI / 4.f) * matrix::Scaling(.2f, 1.f, 1.f));
        material.Diffuse(.7f);
        material.Specular(.3f);
        world.Add(middleSphere);
    }

    // Right Sphere
    {
        auto rightSphere = std::make_shared<Sphere>();
        rightSphere->SetTransform(matrix::Translation(1.5f, .5f, -.5f) * matrix::RotationZ(PI / 4.f) * matrix::Scaling(.5f, .5f, .5f));
        auto& material = rightSphere->ModifyMaterial();
        material.SetColor(Color(0.f, 0.f, 0.f));
        material.Diffuse(.1f);
        material.Transparency(.9f);
        material.Reflective(.9f);
        material.Specular(1.f);
        material.Shininess(300.f);
        material.RefractiveIndex(Material::GetIndexOfRefraction(Material::Type::Glass));
        world.Add(rightSphere);

        auto airBubble = std::make_shared<Sphere>();
        airBubble->SetTransform(matrix::Translation(1.5f, .5f, -.5f) * matrix::RotationZ(PI / 4.f) * matrix::Scaling(.25f, .25f, .25f));
        auto& m2 = airBubble->ModifyMaterial();
        m2.SetColor(Color(.0f, 0.f, 0.f));
        m2.Diffuse(.1f);
        m2.Transparency(.9f);
        m2.Reflective(.9f);
        m2.RefractiveIndex(Material::GetIndexOfRefraction(Material::Type::Air));
        world.Add(airBubble);
    }

    // Left Sphere
    {
        auto leftSphere = std::make_shared<Sphere>();
        leftSphere->SetTransform(matrix::Translation(-1.5f, .33f, -.75f) * matrix::Scaling(.33f, .33f, .33f));
        auto& material = leftSphere->ModifyMaterial();
        material.Pattern(std::make_shared<CheckerPattern>(Color(1.0f, 1.0f, .0f), Color(.6f, .6f, .0f)));
        material.ModifyPattern()->SetTransform(matrix::Scaling(.5f, .5f, .5f));
        material.Diffuse(.7f);
        material.Specular(.3f);
        world.Add(leftSphere);
    }

    auto camera = Camera(1362, 638, PI / 3.f);
    camera.SetTransform(matrix::View(Point(0.5f, 2.f, -8.f), Point(0.f, 1.75f, 0.f), Vector(0.f, 1.f, 0.f)));

    SampleUtils::RenderScene(camera, world, "Ch11_ReflectionAndRefraction.ppm");

    return 0;
}
