#include "Camera.h"
#include "World.h"
#include "TestHelpers.h"

#include <Beddev/Beddev.h>

SCENARIO("Constructing a camera", "Scene")
{
    GIVEN( auto const hSize = 160,
           auto const vSize = 120,
           auto const fov = PI / 2.f )
    WHEN( auto const c = Camera(hSize, vSize, fov) )
    THEN( c.HorizontalSize() == hSize
        , c.VerticalSize() == vSize
        , c.FieldOfView() == fov
        , c.Transform() == Mat44::Identity() )
}

SCENARIO("The pixel size for a horizontal canvas", "Scene")
{
    GIVEN( auto const c = Camera(200, 125, PI/2.f) )
    THEN( c.PixelSize() == 0.01f )
}

SCENARIO("The pixel size for a vertical canvas", "Scene")
{
    GIVEN( auto const c = Camera(125, 200, PI / 2.f) )
    THEN( c.PixelSize() == 0.01f )
}

SCENARIO("Constructing a ray through the center of the canvas", "Scene")
{
    GIVEN( auto const c = Camera(201, 101, PI/2) )
    WHEN( auto const r = c.RayForPixel(100, 50) )
    THEN( r.Origin() == Point(0.f, 0.f, 0.f)
        , r.Direction() == Vector(0.f, 0.f, -1.f) )
}

SCENARIO("Constructing a ray through a corner of the canvas", "Scene")
{
    GIVEN( auto const c = Camera(201, 101, PI / 2) )
    WHEN ( auto const r = c.RayForPixel(0, 0) )
    THEN ( r.Origin() == Point(0.f, 0.f, 0.f)
         , r.Direction() == Vector(0.66519f, 0.33259f, -0.66851f) )
}

SCENARIO("Constructing a ray when the camera is transformed", "Scene")
{
    auto const coord = std::sqrtf(2.f) / 2.f;
    GIVEN( auto c = Camera(201, 101, PI / 2) )
    WHEN ( c.SetTransform(matrix::RotationY(PI/4.f) * matrix::Translation(0.f, -2.f, 5.f))
         , auto const r = c.RayForPixel(100, 50) )
    THEN ( r.Origin() == Point(0.f, 2.f, -5.f)
         , r.Direction() == Vector(coord, 0., -coord) )
}

SCENARIO("Rendering a world with a camera", "Scene")
{
    GIVEN( auto const w = DefaultWorld(),
           auto c = Camera(11, 11, PI / 2),
           auto const from = Point(0.f, 0.f, -5.f),
           auto const to = Point(0.f, 0.f, 0.f),
           auto const up = Vector(0.f, 1.f, 0.f),
           c.SetTransform(matrix::View(from, to, up)) )
    WHEN( auto const image = c.Render(w) )
    THEN( image.PixelAt(5, 5) == Color(0.38066f, 0.47583f, 0.2855f) )
}
