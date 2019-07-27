#include "TestHelpers.h"

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

ShapePtr GlassySphere()
{
    auto s = std::make_shared<Sphere>();
    s->ModifyMaterial().Transparency(1.f);
    s->ModifyMaterial().RefractiveIndex(1.5f);
    return s;
}
