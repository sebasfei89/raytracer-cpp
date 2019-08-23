#include "TestHelpers.h"

#include <RayTracer/Cylinder.h>

World DefaultWorld()
{
    World w;
    auto s1 = std::make_shared<Sphere>();
    auto s2 = std::make_shared<Sphere>();

    s1->ModifyMaterial().SetColor(Color(.8f, 1.f, .6f));
    s1->ModifyMaterial().Diffuse(.7f);
    s1->ModifyMaterial().Specular(.2f);
    w.Add(s1);

    s2->SetTransform(matrix::Scaling(.5f, .5f, .5f));
    w.Add(s2);

    PointLight l(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f));
    w.Add(l);

    return w;
}

World DefaultWorld2()
{
    World w;
    auto cyl = std::make_shared<Cylinder>(0.f, 1.f);

    cyl->ModifyMaterial().SetColor(Color(.8f, 1.f, .6f));
    cyl->ModifyMaterial().Diffuse(.7f);
    cyl->ModifyMaterial().Specular(.2f);
    w.Add(cyl);

    PointLight l(Point(0.f, -1.f, 0.f), Color(1.f, 1.f, 1.f));
    w.Add(l);

    return w;
}

ShapePtr GlassySphere()
{
    auto s = std::make_shared<Sphere>();
    s->ModifyMaterial().Transparency(1.f);
    s->ModifyMaterial().RefractiveIndex(1.5f);
    return s;
}

std::ostream& operator<<(std::ostream& os, TestArg const& arg)
{
    os << "{ " << arg.origin
        << ", " << arg.direction
        << ", " << arg.t1
        << ", " << arg.t2 << " }";
    return os;
}
