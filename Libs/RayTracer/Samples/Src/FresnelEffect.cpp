#include <RayTracer/Camera.h>
#include <RayTracer/Pattern.h>
#include <RayTracer/Shapes/Plane.h>
#include <RayTracer/Shapes/Sphere.h>
#include <RayTracer/Transformations.h>
#include <RayTracer/World.h>

#include <SampleUtils.h>

int main()
{
    auto world = World();

    auto const sunPosition = Point(-50.f, 15.f, 99.f);
    world.Add(PointLight(sunPosition, { 1.f, 1.f, 1.f }));

    // Water
    {
        auto water = std::make_shared<Plane>();
        water->CastShadows(false);
        auto& m = water->ModifyMaterial();
        auto p1 = std::make_shared<StripPattern>(Color::White(), Color::Blue() * .1f);
        p1->SetTransform(matrix::RotationY(PI/2.f));
        m.Pattern(std::make_shared<PerlinNoisePattern>(p1));
        m.Diffuse(.1f);
        m.Specular(.9f);
        m.Shininess(200.f);
        m.Reflective(1.f);
        m.Transparency(1.f);
        m.RefractiveIndex(Material::GetIndexOfRefraction(Material::Type::Water));
        world.Add(water);
    }

    // Sand
    {
        auto sand = std::make_shared<Plane>();
        sand->SetTransform(matrix::RotationX(PI / 16.f));
        auto& m = sand->ModifyMaterial();
        m.Pattern(std::make_shared<CheckerPattern>(Color(.85f, .80f, .50f), Color(.99f, .99f, .61f)));
        m.Ambient(.3f);
        m.Diffuse(1.f);
        m.Specular(0.f);
        world.Add(sand);
    }

    // Sky
    {
        auto sky = std::make_shared<Plane>();
        sky->SetTransform(matrix::Translation(0.f, 0.f, 100.f) * matrix::RotationX(PI / 2.f));
        auto& m = sky->ModifyMaterial();
        m.SetColor(Color(.6f, .6f, 1.f));
        m.Ambient(1.f);
        m.Diffuse(0.f);
        m.Specular(0.f);
        world.Add(sky);
    }

    // Sun
    {
        auto sun = std::make_shared<Sphere>();
        sun->SetTransform(matrix::Translation(sunPosition.X(), sunPosition.Y(), sunPosition.Z()) * matrix::Scaling(4.f, 4.f, 4.f));
        sun->CastShadows(false);
        auto& m = sun->ModifyMaterial();
        m.SetColor(Color::Yellow());
        m.Ambient(1.f);
        m.Specular(0.f);
        m.Diffuse(0.f);
        world.Add(sun);
    }

    // Rocks
    {
        auto r1 = std::make_shared<Sphere>();
        r1->SetTransform(matrix::Translation(0.f, -.3f, .75f) * matrix::Scaling(.5f, .25f, .35f));
        r1->ModifyMaterial().Pattern(std::make_shared<GradientPattern>(Color(.2f, .4f, .1f), Color::Black()));
        r1->ModifyMaterial().Ambient(.2f);
        world.Add(r1);

        auto r2 = std::make_shared<Sphere>();
        r2->SetTransform(matrix::Translation(1.f, -.3f, .6f) * matrix::Scaling(.2f, .5f, .25f));
        r2->ModifyMaterial().Pattern(std::make_shared<GradientPattern>(Color(.8f, .4f, .2f), Color::White()));
        r2->ModifyMaterial().Ambient(.2f);
        world.Add(r2);
    }

    auto camera = Camera(1362, 638, PI / 3.f);
    camera.SetTransform(matrix::View(Point(0.f, 1.f, -3.5f), Point(0.f, 0.f, 5.f), Vector(0.f, 1.f, 0.f)));

    SampleUtils::RenderScene(camera, world, "Ch11b_FresnelEffect.ppm");

    return 0;
}
