#include "TestHelpers.h"

#include <RayTracer/Group.h>
#include <RayTracer/Plane.h>
#include <RayTracer/Ray.h>
#include <RayTracer/Sphere.h>
#include <RayTracer/Transformations.h>
#include <RayTracer/Util.h>
#include <RayTracer/World.h>

#include <Beddev/Beddev.h>

#include <sstream>

namespace
{
constexpr static char const* g_testWorld = R"({
    "objects": [{
        "name": "TestGroup",
        "archetype" : "TestArchetype",
        "scaling": [3.0, 3.0, 3.0]
    },{
        "name": "TestCube",
        "type" : "Cube"
    }],
    "lights": [{
        "name": "sun",
        "type" : "PointLight",
        "position" : [0, 5, 0],
        "intensity" : [1, 1, 1]
    }],
    "archetypes": [{
        "name": "TestArchetype",
        "type": "Group",
        "scaling": [2.0, 2.0, 2.0],
        "children": [{
            "name": "TestCylinder",
            "type": "Cylinder",
            "caps": [0, 1],
            "position": [0, 0, -1],
            "rotation": [0, 1.57079633, 0],
            "scaling": [0.25, 1.0, 0.25]
        },{
            "name": "TestSphere",
            "type": "Sphere",
            "position": [0, 0, -1],
            "scaling": [0.25, 0.25, 0.25]
        }]
    }]
})";

}

SCENARIO("Creating a world", "world")
{
    GIVEN( auto const w = World() )
    THEN( w.Objects().size() == 0
        , w.Lights().size() == 0 )
}

SCENARIO("The default world", "world")
{
    GIVEN( auto const light = PointLight(Point(-10.f, 10.f, -10.f), Color(1.f, 1.f, 1.f))
         , auto s1 = std::make_shared<Sphere>()
         , s1->ModifyMaterial().SetColor(Color(.8f, 1.f, .6f))
         , s1->ModifyMaterial().Diffuse(.7f)
         , s1->ModifyMaterial().Specular(.2f)
         , auto s2 = std::make_shared<Sphere>()
         , s2->SetTransform(matrix::Scaling(.5f, .5f, .5f)) )
    WHEN( auto const w = DefaultWorld() )
    THEN( w.Lights().size() == 1
        , w.Lights()[0] == light
        , w.Objects().size() == 2
        , w.Objects()[0]->operator==(*s1.get())
        , w.Objects()[1]->operator==(*s2.get()) )
}

SCENARIO("Intersect a world with a ray", "world")
{
    GIVEN( auto const w = DefaultWorld()
         , auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f)) )
    WHEN( std::vector<Intersection> xs = {}
        , r.Intersect(w, xs) )
    THEN( xs.size() == 4
        , xs[0].Distance() == 4.f
        , xs[1].Distance() == 4.5f
        , xs[2].Distance() == 5.5f
        , xs[3].Distance() == 6.f )
}

SCENARIO("Precomputing the state of an interdection", "scene")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto s = std::make_shared<Sphere>()
         , auto const i = Intersection(4.f, s) )
    WHEN( auto const c = r.Precompute(i) )
    THEN( c.m_distance == i.Distance()
        , c.m_object == i.Object()
        , c.m_point == Point(0.f, 0.f, -1.f)
        , c.m_eyev == Vector(0.f, 0.f, -1.f)
        , c.m_normalv == Vector(0.f, 0.f, -1.f) )
}

SCENARIO("The hit, when an intersection occurs on the outside", "scene")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto s = std::make_shared<Sphere>()
         , auto const i = Intersection(4.f, s) )
    WHEN( auto const c = r.Precompute(i) )
    THEN( c.m_inside == false )
}

SCENARIO("The hit, when an intersection occurs on the inside", "scene")
{
    GIVEN( auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f))
         , auto s = std::make_shared<Sphere>()
         , auto const i = Intersection(1.f, s) )
    WHEN( auto const c = r.Precompute(i) )
    THEN( c.m_point == Point(0.f, 0.f, 1.f)
        , c.m_eyev == Vector(0.f, 0.f, -1.f)
        , c.m_normalv == Vector(0.f, 0.f, -1.f)
        , c.m_inside == true )
}

SCENARIO("Shading and intersection", "scene")
{
    GIVEN( auto const w = DefaultWorld()
         , auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto const& s = w.Objects()[0]
         , auto const i = Intersection(4.f, s) )
    WHEN( auto const comps = r.Precompute(i)
        , auto const c = w.ShadeHit(comps) )
    THEN( c == Color(0.38066f, 0.47583f, 0.2855f) )
}

SCENARIO("Shading and intersection from the inside", "scene")
{
    GIVEN( auto w = DefaultWorld()
         , w.ModifyLights()[0] = PointLight(Point(0.f, 0.25f, 0.f), Color(1.f, 1.f, 1.f))
         , auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f))
         , auto const& s = w.Objects()[1]
         , auto const i = Intersection(.5f, s) )
    WHEN( auto const comps = r.Precompute(i)
        , auto const c = w.ShadeHit(comps) )
    THEN( c == Color(0.90498f, 0.90498f, 0.90498f) )
}

SCENARIO("The color when a ray misses", "scene")
{
    GIVEN( auto w = DefaultWorld()
         , auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 1.f, 0.f)) )
    WHEN( auto const c = w.ColorAt(r) )
    THEN( c == Color(0.f, 0.f, 0.f) )
}

SCENARIO("The color when a ray hits", "scene")
{
    GIVEN( auto w = DefaultWorld()
         , auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f)) )
    WHEN( auto const c = w.ColorAt(r) )
    THEN( c == Color(0.38066f, 0.47583f, 0.2855f) )
}

SCENARIO("The color with an intersection behind the ray", "scene")
{
    GIVEN( auto w = DefaultWorld()
         , auto& s1 = w.ModifyObjects()[0]
         , s1->ModifyMaterial().Ambient(1.f)
         , auto& s2 = w.ModifyObjects()[1]
         , s2->ModifyMaterial().Ambient(1.f)
         , auto const r = Ray(Point(0.f, 0.f, 0.75f), Vector(0.f, 0.f, -1.f)) )
    WHEN( auto const c = w.ColorAt(r) )
    THEN( c == s2->GetMaterial().GetColor() )
}

SCENARIO("There is no shadow when nothing is collinear with the point and light", "shadows")
{
    GIVEN( auto const w = DefaultWorld()
         , auto const point = Point(0.f, 10.f, 0.f) )
    THEN( !w.IsShadowed(point, w.Lights()[0]) )
}

SCENARIO("The shadow when an object is between the point and the light", "shadows")
{
    GIVEN( auto const w = DefaultWorld()
         , auto const point = Point(10.f, -10.f, 10.f) )
    THEN( w.IsShadowed(point, w.Lights()[0]) )
}

SCENARIO("The shadow when a cylinder isn't between the point and the light", "shadows")
{
    GIVEN( auto const w = DefaultWorld2()
         , auto const p1 = Point(0.f, -1.f, -2.f)
         , auto const p2 = Point(0.f, -5.f, -2.f)
         , auto const p3 = Point(0.f, -2.5f, -2.f) )
    THEN( !w.IsShadowed(p1, w.Lights()[0])
        , !w.IsShadowed(p2, w.Lights()[0])
        , !w.IsShadowed(p3, w.Lights()[0]) )
}

SCENARIO("There is no shadow when an object is behind the light", "shadows")
{
    GIVEN( auto const w = DefaultWorld()
         , auto const point = Point(-20.f, 20.f, -20.f) )
    THEN( !w.IsShadowed(point, w.Lights()[0]) )
}

SCENARIO("There is no shadow when an object is behind the point", "shadows")
{
    GIVEN( auto const w = DefaultWorld()
         , auto const point = Point(-2.f, 2.f, -2.f) )
    THEN( !w.IsShadowed(point, w.Lights()[0]) )
}

SCENARIO("ShadeHit is given an intersection in shadow", "shadows")
{
    GIVEN( auto w = World()
         , w.Add(PointLight(Point(0.f, 0.f, -10.f), Color(1.f, 1.f, 1.f)))
         , auto s1 = std::make_shared<Sphere>()
         , w.Add(s1);
         , auto s2 = std::make_shared<Sphere>()
         , s2->SetTransform(matrix::Translation(0.f, 0.f, 10.f))
         , w.Add(s2)
         , auto const r = Ray(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f))
         , auto const i = Intersection(4.f, s2) )
    WHEN( auto const comps = r.Precompute(i)
        , auto const c = w.ShadeHit(comps) )
    THEN( c == Color(0.1f, 0.1f, 0.1f) )
}

SCENARIO("ShadeHit is given an intersection occluded by an object with doesn't cast shadows", "shadows")
{
    GIVEN( auto w = World()
         , w.Add(PointLight(Point(0.f, 10.f, -10.f), Color(1.f, 1.f, 1.f)))
         , auto p1 = std::make_shared<Plane>()
         , p1->SetTransform(matrix::RotationX(PI/-4.f))
         , p1->ModifyMaterial().SetColor(Color::Red());
         , w.Add(p1);
         , auto p2 = std::make_shared<Plane>()
         , p2->SetTransform(matrix::Translation(0.f, 5.f, 0.f))
         , p2->CastShadows(false);
         , w.Add(p2)
         , auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto const i = Intersection(5.f, p1))
    WHEN( auto const comps = r.Precompute(i)
        , auto const c = w.ShadeHit(comps) )
    THEN( c == Color(1.f, 0.f, 0.f) )
}

SCENARIO("The reflective color for a nonreflective material", "reflection")
{
    GIVEN( auto w = DefaultWorld()
         , auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f))
         , auto shape = w.Objects()[0]
         , shape->ModifyMaterial().Ambient(1.f)
         , auto const i = Intersection(1.f, shape) )
    WHEN( auto const comps = r.Precompute(i)
        , auto const color = w.ReflectedColor(comps) )
    THEN( color == Color(0.f, 0.f, 0.f) )
}

SCENARIO("The reflective color for a reflective material", "reflection")
{
    GIVEN( auto w = DefaultWorld()
         , auto shape = std::make_shared<Plane>()
         , shape->ModifyMaterial().Reflective(0.5f)
         , shape->SetTransform(matrix::Translation(0.f, -1.f, 0.f))
         , w.Add(shape)
         , auto const r = Ray(Point(0.f, 0.f, -3.f), Vector(0.f, -SQRT2OVR2, SQRT2OVR2))
         , auto const i = Intersection(SQRT2, shape) )
    WHEN( auto const comps = r.Precompute(i)
        , auto const color = w.ReflectedColor(comps) )
    THEN( color == Color(0.19032f, 0.2379f, 0.14274f) )
}

SCENARIO("ShadeHit with a reflective material", "reflection")
{
    GIVEN( auto w = DefaultWorld()
         , auto shape = std::make_shared<Plane>()
         , shape->ModifyMaterial().Reflective(0.5f)
         , shape->SetTransform(matrix::Translation(0.f, -1.f, 0.f))
         , w.Add(shape)
         , auto const r = Ray(Point(0.f, 0.f, -3.f), Vector(0.f, -SQRT2OVR2, SQRT2OVR2))
         , auto const i = Intersection(SQRT2, shape) )
    WHEN( auto const comps = r.Precompute(i)
        , auto const color = w.ShadeHit(comps) )
    THEN( color == Color(0.87677f, 0.92436f, 0.82918f) )
}

SCENARIO("ColorAt with mutually reflective surfaces", "reflection")
{
    GIVEN( World w
         , w.Add(PointLight(Point(0.f, 0.f, 0.f), Vector(1.f, 1.f, 1.f)))
         , auto lower = std::make_shared<Plane>()
         , lower->ModifyMaterial().Reflective(1.f)
         , lower->SetTransform(matrix::Translation(0.f, -1.f, 0.f))
         , w.Add(lower)
         , auto upper = std::make_shared<Plane>()
         , upper->ModifyMaterial().Reflective(1.f)
         , upper->SetTransform(matrix::Translation(0.f, 1.f, 0.f))
         , w.Add(upper)
         , auto const r = Ray(Point(0.f, 0.f, 0.f), Vector(0.f, 1.f, 0.f)) )
    THEN( w.ColorAt(r) != Color::Black() )
}

SCENARIO("The reflected color at the maximum recursive depth", "reflection")
{
    GIVEN( auto w = DefaultWorld()
         , auto shape = std::make_shared<Plane>()
         , shape->ModifyMaterial().Reflective(0.5f)
         , shape->SetTransform(matrix::Translation(0.f, -1.f, 0.f))
         , w.Add(shape)
         , auto const r = Ray(Point(0.f, 0.f, -3.f), Vector(0.f, -SQRT2OVR2, SQRT2OVR2))
         , auto const i = Intersection(SQRT2, shape) )
    WHEN( auto const comps = r.Precompute(i)
        , auto const color = w.ReflectedColor(comps, 0) )
    THEN( color == Color(0.f, 0.f, 0.f) )
}

SCENARIO("The refracted color with an opaque surface", "refraction")
{
    GIVEN( auto w = DefaultWorld()
         , auto shape = w.Objects()[0]
         , auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto const xs = Intersections(Intersection{ 4.f, shape }, Intersection{ 6.f, shape }) )
    WHEN( auto const comps = r.Precompute(xs[0], xs)
        , auto const color = w.RefractedColor(comps) )
    THEN( color == Color(0.f, 0.f, 0.f) )
}

SCENARIO("The refracted color at the maximum recursive depth", "refraction")
{
    GIVEN( auto w = DefaultWorld()
         , auto shape = w.Objects()[0]
         , shape->ModifyMaterial().Transparency(1.f)
         , shape->ModifyMaterial().RefractiveIndex(1.5f)
         , auto const r = Ray(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f))
         , auto const xs = Intersections(Intersection{ 4.f, shape }, Intersection{ 6.f, shape }) )
    WHEN( auto const comps = r.Precompute(xs[0], xs)
        , auto const color = w.RefractedColor(comps, 0) )
    THEN( color == Color(0.f, 0.f, 0.f) )
}

SCENARIO("The refracted color under total internal reflection", "refraction")
{
    GIVEN( auto w = DefaultWorld()
         , auto shape = w.Objects()[0]
         , shape->ModifyMaterial().Transparency(1.f)
         , shape->ModifyMaterial().RefractiveIndex(1.5f)
         , auto const r = Ray(Point(0.f, 0.f, SQRT2OVR2), Vector(0.f, 1.f, 0.f))
         , auto const xs = Intersections(Intersection{ -SQRT2OVR2, shape }, Intersection{ SQRT2OVR2, shape }) )
    WHEN( auto const comps = r.Precompute(xs[1], xs)
        , auto const color = w.RefractedColor(comps, 5) )
    THEN( color == Color(0.f, 0.f, 0.f) )
}

SCENARIO("The refracted color with a refracted ray", "refraction")
{
    GIVEN( auto w = DefaultWorld()
         , auto a = w.Objects()[0]
         , a->ModifyMaterial().Ambient(1.f)
         , a->ModifyMaterial().Pattern(std::make_shared<TestPattern>())
         , auto b = w.Objects()[1]
         , b->ModifyMaterial().Transparency(1.f)
         , b->ModifyMaterial().RefractiveIndex(1.5f)
         , auto const r = Ray(Point(0.f, 0.f, .1f), Vector(0.f, 1.f, 0.f))
         , auto const xs = Intersections(
            Intersection{ -.9899f, a }, Intersection{ -.4899f, b },
            Intersection{ .4899f, b }, Intersection{ .9899f, a }) )
    WHEN( auto const comps = r.Precompute(xs[2], xs)
        , auto const color = w.RefractedColor(comps, 5) )
    THEN( color == Color(0.f, .99888f, .04725f) )
}

SCENARIO("ShadeHit() with a transparent material", "refraction")
{
    GIVEN( auto w = DefaultWorld()
         , auto floor = std::make_shared<Plane>()
         , floor->SetTransform(matrix::Translation(0.f, -1.f, 0.f));
         , floor->ModifyMaterial().Transparency(.5f)
         , floor->ModifyMaterial().RefractiveIndex(1.5f)
         , w.Add(floor);
         , auto ball = std::make_shared<Sphere>()
         , ball->SetTransform(matrix::Translation(0.f, -3.5f, -0.5f));
         , ball->ModifyMaterial().Ambient(.5f)
         , ball->ModifyMaterial().SetColor(Color::Red())
         , w.Add(ball);
         , auto const r = Ray(Point(0.f, 0.f, -3.f), Vector(0.f, -SQRT2OVR2, SQRT2OVR2))
         , auto const xs = Intersections(Intersection{ SQRT2, floor }) )
    WHEN( auto const comps = r.Precompute(xs[0], xs)
        , auto const color = w.ShadeHit(comps, 5) )
    THEN( color == Color(.93642f, .68642f, .68642f) )
}

SCENARIO("ShadeHit() with a reflective, transparent material", "refraction")
{
    GIVEN( auto w = DefaultWorld()
         , auto const r = Ray(Point(0.f, 0.f, -3.f), Vector(0.f, -SQRT2OVR2, SQRT2OVR2))
         , auto floor = std::make_shared<Plane>()
         , floor->SetTransform(matrix::Translation(0.f, -1.f, 0.f));
         , floor->ModifyMaterial().Reflective(.5f)
         , floor->ModifyMaterial().Transparency(.5f)
         , floor->ModifyMaterial().RefractiveIndex(1.5f)
         , w.Add(floor);
         , auto ball = std::make_shared<Sphere>()
         , ball->ModifyMaterial().SetColor(Color::Red())
         , ball->ModifyMaterial().Ambient(.5f)
         , ball->SetTransform(matrix::Translation(0.f, -3.5f, -0.5f));
         , w.Add(ball);
         , auto const xs = Intersections(Intersection{ SQRT2, floor }) )
    WHEN( auto const comps = r.Precompute(xs[0], xs)
        , auto const color = w.ShadeHit(comps, 5) )
    THEN( color == Color(.93391f, .69643f, .69243f) )
}

SCENARIO("Loading a world from an empty scene.json file", "world")
{
    GIVEN( auto w = World()
         , std::istringstream ss("") )
    THEN( !w.Load(ss) )
}

SCENARIO("Loading a world from an invalid scene.json file", "world")
{
    GIVEN( auto w = World()
         , std::istringstream ss(R"([{"objects":[]}])") )
    THEN( !w.Load(ss) )
}

SCENARIO("Loading a world from a scene.json file", "world")
{
    GIVEN( auto w = World()
         , std::istringstream ss(g_testWorld) )
    WHEN( w.Load(ss)
        , auto lights = w.Lights()
        , auto objects = w.Objects()
        , auto archetypes = w.Archetypes() )
    THEN( lights.size() == 1
        , lights[0].GetName() == "sun"
        , lights[0].Position() == Point(0.f, 5.f, 0.f)
        , lights[0].Intensity() == Color(1.f, 1.f, 1.f)
        , objects.size() == 2
        , objects[0]->Name() == "TestGroup"
        , objects[0]->Transform() == matrix::Scaling(3.f, 3.f, 3.f)
        , std::dynamic_pointer_cast<Group>(objects[0]) != nullptr
        , archetypes.size() == 1 )
}
