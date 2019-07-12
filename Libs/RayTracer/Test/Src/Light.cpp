#include "Lighting.h"
#include <Testing.h>

SCENARIO("A point light has a position and intesity", "[Lighting]")
{
    GIVEN_2(
        auto const intensity = Color(1.f, 1.f, 1.f);
        auto const position = Point(0.f, 0.f, 0.f);
    WHEN_,
        auto const light = PointLight(position, intensity);
    REQUIRE_,
        light.Position() == position,
        light.Intensity() == intensity )
}

SCENARIO("The default material")
{
    GIVEN_1(
        auto const m = Material();
    REQUIRE_,
        m.GetColor() == Color(1.f, 1.f, 1.f),
        m.Ambient() == 0.1f,
        m.Diffuse() == 0.9f,
        m.Specular() == 0.9f,
        m.Shininess() == 200.f )
}

SCENARIO("Lighting with the eye between the light and the surface")
{
    GIVEN_2(
        auto const m = Material();
        auto const position = Point(0.f, 0.f, 0.f);
        auto const eyev = Vector(0.f, 0.f, -1.f);
        auto const normalv = Vector(0.f, 0.f, -1.f);
        auto const light = PointLight(Point(0.f, 0.f, -10.f), {1.f, 1.f, 1.f});
    WHEN_,
        auto const result = Lighting(m, light, position, eyev, normalv);
    REQUIRE_,
        result == Color(1.9f, 1.9f, 1.9f) )
}

SCENARIO("Lighting with the eye between the light and the surface, eye offset 45 deg")
{
    float const coord = std::sqrt(2.f) / 2.f;
    GIVEN_2(
        auto const m = Material();
        auto const position = Point(0.f, 0.f, 0.f);
        auto const eyev = Vector(0.f, coord, -coord);
        auto const normalv = Vector(0.f, 0.f, -1.f);
        auto const light = PointLight(Point(0.f, 0.f, -10.f), { 1.f, 1.f, 1.f });
    WHEN_,
        auto const result = Lighting(m, light, position, eyev, normalv);
    REQUIRE_,
        result == Color(1.0f, 1.0f, 1.0f) )
}

SCENARIO("Lighting with eye oposite surface, light offset 45 deg")
{
    GIVEN_2(
        auto const m = Material();
        auto const position = Point(0.f, 0.f, 0.f);
        auto const eyev = Vector(0.f, 0.f, -1.f);
        auto const normalv = Vector(0.f, 0.f, -1.f);
        auto const light = PointLight(Point(0.f, 10.f, -10.f), { 1.f, 1.f, 1.f });
    WHEN_,
        auto const result = Lighting(m, light, position, eyev, normalv);
    REQUIRE_,
        result == Color(0.7364f, 0.7364f, 0.7364f) )
}

SCENARIO("Lighting with eye in the path of the reflection vector")
{
    float const coord = std::sqrt(2.f) / 2.f;
    GIVEN_2(
        auto const m = Material();
        auto const position = Point(0.f, 0.f, 0.f);
        auto const eyev = Vector(0.f, -coord, -coord);
        auto const normalv = Vector(0.f, 0.f, -1.f);
        auto const light = PointLight(Point(0.f, 10.f, -10.f), { 1.f, 1.f, 1.f });
    WHEN_,
        auto const result = Lighting(m, light, position, eyev, normalv);
    REQUIRE_,
        result == Color(1.63639f, 1.63639f, 1.63639f) )
}

SCENARIO("Lighting with the light behind the surface")
{
    GIVEN_2(
        auto const m = Material();
        auto const position = Point(0.f, 0.f, 0.f);
        auto const eyev = Vector(0.f, 0.f, -1.f);
        auto const normalv = Vector(0.f, 0.f, -1.f);
        auto const light = PointLight(Point(0.f, 0.f, 10.f), { 1.f, 1.f, 1.f });
    WHEN_,
        auto const result = Lighting(m, light, position, eyev, normalv);
    REQUIRE_,
        result == Color(0.1f, 0.1f, 0.1f) )
}
