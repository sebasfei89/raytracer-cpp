#include <RayTracer/Lighting.h>
#include <RayTracer/Pattern.h>
#include <RayTracer/Shapes/Sphere.h>

#include <Beddev/Beddev.h>

SCENARIO("A point light has a position and intesity", "lighting")
{
    GIVEN( auto const intensity = Color(1.f, 1.f, 1.f)
         , auto const position = Point(0.f, 0.f, 0.f) )
    WHEN( auto const light = PointLight(position, intensity) )
    THEN( light.Position() == position
        , light.Intensity() == intensity )
}

SCENARIO("Lighting with the eye between the light and the surface", "lighting")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const m = Material()
         , auto const position = Point(0.f, 0.f, 0.f)
         , auto const eyev = Vector(0.f, 0.f, -1.f)
         , auto const normalv = Vector(0.f, 0.f, -1.f)
         , auto const light = PointLight(Point(0.f, 0.f, -10.f), {1.f, 1.f, 1.f}) )
    WHEN( auto const color = Lighting(m, s, light, position, eyev, normalv) )
    THEN( color == Color(1.9f, 1.9f, 1.9f) )
}

SCENARIO("Lighting with the eye between the light and the surface, eye offset 45 deg", "lighting")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const m = Material()
         , auto const position = Point(0.f, 0.f, 0.f)
         , auto const eyev = Vector(0.f, SQRT2OVR2, -SQRT2OVR2)
         , auto const normalv = Vector(0.f, 0.f, -1.f)
         , auto const light = PointLight(Point(0.f, 0.f, -10.f), { 1.f, 1.f, 1.f }) )
    WHEN( auto const color = Lighting(m, s, light, position, eyev, normalv) )
    THEN( color == Color(1.0f, 1.0f, 1.0f) )
}

SCENARIO("Lighting with eye oposite surface, light offset 45 deg", "lighting")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const m = Material()
         , auto const position = Point(0.f, 0.f, 0.f)
         , auto const eyev = Vector(0.f, 0.f, -1.f)
         , auto const normalv = Vector(0.f, 0.f, -1.f)
         , auto const light = PointLight(Point(0.f, 10.f, -10.f), { 1.f, 1.f, 1.f }) )
    WHEN( auto const color = Lighting(m, s, light, position, eyev, normalv) )
    THEN( color == Color(0.7364f, 0.7364f, 0.7364f) )
}

SCENARIO("Lighting with eye in the path of the reflection vector", "lighting")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const m = Material()
         , auto const position = Point(0.f, 0.f, 0.f)
         , auto const eyev = Vector(0.f, -SQRT2OVR2, -SQRT2OVR2)
         , auto const normalv = Vector(0.f, 0.f, -1.f)
         , auto const light = PointLight(Point(0.f, 10.f, -10.f), { 1.f, 1.f, 1.f }) )
    WHEN( auto const color = Lighting(m, s, light, position, eyev, normalv) )
    THEN( color == Color(1.63639f, 1.63639f, 1.63639f) )
}

SCENARIO("Lighting with the light behind the surface", "lighting")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const m = Material()
         , auto const position = Point(0.f, 0.f, 0.f)
         , auto const eyev = Vector(0.f, 0.f, -1.f)
         , auto const normalv = Vector(0.f, 0.f, -1.f)
         , auto const light = PointLight(Point(0.f, 0.f, 10.f), { 1.f, 1.f, 1.f }) )
    WHEN( auto const color = Lighting(m, s, light, position, eyev, normalv) )
    THEN( color == Color(0.1f, 0.1f, 0.1f) )
}

SCENARIO("Lighting with the surface in shadow", "lighting")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto const m = Material()
         , auto const position = Point(0.f, 0.f, 0.f)
         , auto const eyev = Vector(0.f, 0.f, -1.f)
         , auto const normalv = Vector(0.f, 0.f, -1.f)
         , auto const light = PointLight(Point(0.f, 0.f, -10.f), { 1.f, 1.f, 1.f })
         , auto const inShadow = true )
    WHEN( auto const color = Lighting(m, s, light, position, eyev, normalv, inShadow) )
    THEN( color == Color(.1f, .1f, .1f) )
}

SCENARIO("Lighting with a pattern applied", "lighting")
{
    GIVEN( auto const s = std::make_shared<Sphere>()
         , auto m = Material()
         , m.Pattern(std::make_shared<StripPattern>(Color::White(), Color::Black()))
         , m.Ambient(1.f)
         , m.Diffuse(0.f)
         , m.Specular(0.f)
         , auto const eyev = Vector(0.f, 0.f, -1.f)
         , auto const normalv = Vector(0.f, 0.f, -1.f)
         , auto const light = PointLight(Point(0.f, 0.f, -10.f), Color::White()) )
    WHEN( auto const c1 = Lighting(m, s, light, Point(.9f, 0.f, 0.f), eyev, normalv, false)
        , auto const c2 = Lighting(m, s, light, Point(1.1f, 0.f, 0.f), eyev, normalv, false) )
    THEN( c1 == Color::White()
        , c2 == Color::Black() )
}
