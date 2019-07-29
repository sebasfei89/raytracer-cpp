#include <RayTracer/Transformations.h>

#include <Beddev/Beddev.h>

SCENARIO("Multiplying by a translation matrix", "math")
{
    GIVEN( auto const tx = matrix::Translation(5.f, -3.f, 2.f)
         , auto const p = Point(-3.f, 4.f, 5.f) )
    THEN( tx * p == Point(2.f, 1.f, 7.f) )
}

SCENARIO("Multiplying by a the inverse of a translation matrix", "math")
{
    GIVEN( auto const tx = matrix::Translation(5.f, -3.f, 2.f)
         , auto const itx = tx.Inverse()
         , auto const p = Point(-3.f, 4.f, 5.f) )
    THEN( itx * p == Point(-8.f, 7.f, 3.f) )
}

SCENARIO("Translation does not affect vectors", "math")
{
    GIVEN( auto const tx = matrix::Translation(5.f, -3.f, 2.f)
         , auto const v = Vector(-3.f, 4.f, 5.f) )
    THEN( tx * v == v )
}

SCENARIO("A scaling matrix applied to a point", "math")
{
    GIVEN( auto const tx = matrix::Scaling(2.f, 3.f, 4.f)
         , auto const p = Point(-4.f, 6.f, 8.f) )
    THEN( tx * p == Point(-8.f, 18.f, 32.f) )
}

SCENARIO("A scaling matrix applied to a vector", "math")
{
    GIVEN( auto const tx = matrix::Scaling(2.f, 3.f, 4.f)
         , auto const v = Vector(-4.f, 6.f, 8.f) )
    THEN( tx * v == Vector(-8.f, 18.f, 32.f) )
}

SCENARIO("Multiplying by the inverse of a scaling matrix", "math")
{
    GIVEN( auto const tx = matrix::Scaling(2.f, 3.f, 4.f)
         , auto const itx = tx.Inverse()
         , auto const v = Vector(-4.f, 6.f, 8.f) )
    THEN( itx * v == Vector(-2.f, 2.f, 2.f) )
}

SCENARIO("Reflection is scaling by a negative value", "math")
{
    GIVEN( auto const tx = matrix::Scaling(-1.f, 1.f, 1.f)
         , auto const p = Point(2.f, 3.f, 4.f) )
    THEN( tx * p == Point(-2.f, 3.f, 4.f) )
}

SCENARIO("Rotating a point 45 degrees around the x axis", "math")
{
    GIVEN(auto const tx = matrix::RotationX(PI / 4.f)
        , auto const p = Point(0.f, 1.f, 0.f))
    THEN(tx * p == Point(0.f, sqrt(2.f) / 2.f, sqrt(2.f) / 2.f))
}

SCENARIO("Rotating a point 90 degrees around the x axis", "math")
{
    GIVEN( auto const tx = matrix::RotationX(PI / 2.f)
         , auto const p = Point(0.f, 1.f, 0.f) )
    THEN( tx * p == Point(0.f, 0.f, 1.f) )
}

SCENARIO("The inverse of an x-rotation rotates in the opposite direction", "math")
{
    GIVEN( auto const tx = matrix::RotationX(PI / 4.f)
         , auto const itx = tx.Inverse()
         , auto const p = Point(0.f, 1.f, 0.f) )
    THEN( itx * p == Point(0.f, sqrt(2.f) / 2.f, -sqrt(2.f) / 2.f) )
}

SCENARIO("Rotating a point 45 degrees around the y axis", "math")
{
    GIVEN( auto const tx = matrix::RotationY(PI / 4.f)
         , auto const p = Point(0.f, 0.f, 1.f) )
    THEN( tx * p == Point(sqrt(2.f) / 2.f, 0.f, sqrt(2.f) / 2.f) )
}

SCENARIO("Rotating a point 90 degrees around the y axis", "math")
{
    GIVEN( auto const tx = matrix::RotationY(PI / 2.f)
         , auto const p = Point(0.f, 0.f, 1.f) )
    THEN( tx * p == Point(1.f, 0.f, 0.f) )
}

SCENARIO("Rotating a point 45 degrees around the z axis", "math")
{
    GIVEN( auto const tx = matrix::RotationZ(PI / 4.f)
         , auto const p = Point(0.f, 1.f, 0.f) )
    THEN( tx * p == Point(-sqrt(2.f) / 2.f, sqrt(2.f) / 2.f, 0.f) )
}

SCENARIO("Rotating a point 90 degrees around the z axis", "math")
{
    GIVEN( auto const tx = matrix::RotationZ(PI / 2.f)
         , auto const p = Point(0.f, 1.f, 0.f) )
    THEN( tx * p == Point(-1.f, 0.f, 0.f) )
}

SCENARIO("A xy shearing transformation moves one component in proportion to other", "math")
{
    GIVEN(auto const tx = matrix::Shearing(1.f, 0.f, 0.f, 0.f, 0.f, 0.f)
        , auto const p = Point(2.f, 3.f, 4.f))
    THEN(tx * p == Point(5.f, 3.f, 4.f))
}

SCENARIO("A xz shearing transformation moves one component in proportion to other", "math")
{
    GIVEN( auto const tx = matrix::Shearing(0.f, 1.f, 0.f, 0.f, 0.f, 0.f)
         , auto const p = Point(2.f, 3.f, 4.f) )
    THEN( tx * p == Point(6.f, 3.f, 4.f) )
}

SCENARIO("A yx shearing transformation moves one component in proportion to other", "math")
{
    GIVEN( auto const tx = matrix::Shearing(0.f, 0.f, 1.f, 0.f, 0.f, 0.f)
         , auto const p = Point(2.f, 3.f, 4.f) )
    THEN( tx * p == Point(2.f, 5.f, 4.f) )
}

SCENARIO("A yz shearing transformation moves one component in proportion to other", "math")
{
    GIVEN( auto const tx = matrix::Shearing(0.f, 0.f, 0.f, 1.f, 0.f, 0.f)
         , auto const p = Point(2.f, 3.f, 4.f) )
    THEN( tx * p == Point(2.f, 7.f, 4.f) )
}

SCENARIO("A zx shearing transformation moves one component in proportion to other", "math")
{
    GIVEN( auto const tx = matrix::Shearing(0.f, 0.f, 0.f, 0.f, 1.f, 0.f)
         , auto const p = Point(2.f, 3.f, 4.f) )
    THEN( tx * p == Point(2.f, 3.f, 6.f) )
}

SCENARIO("A zy shearing transformation moves one component in proportion to other", "math")
{
    GIVEN( auto const tx = matrix::Shearing(0.f, 0.f, 0.f, 0.f, 0.f, 1.f)
         , auto const p = Point(2.f, 3.f, 4.f) )
    THEN( tx * p == Point(2.f, 3.f, 7.f) )
}

SCENARIO("Individual transformations are applied in sequence", "math")
{
    GIVEN( auto const p = Point(1.f, 0.f, 1.f)
         , auto const mr = matrix::RotationX(PI / 2.f)
         , auto const ms = matrix::Scaling(5.f, 5.f, 5.f)
         , auto const mt = matrix::Translation(10.f, 5.f, 7.f) )
    WHEN( auto const p2 = mr * p
        , auto const p3 = ms * p2
        , auto const p4 = mt * p3 )
    THEN( p2 == Point(1.f, -1.f, 0.f)
        , p3 == Point(5.f, -5.f, 0.f)
        , p4 == Point(15.f, 0.f, 7.f) )
}

SCENARIO("Chained transformations must be applied in reverse order", "math")
{
    GIVEN( auto const p = Point(1.f, 0.f, 1.f)
         , auto const mr = matrix::RotationX(PI / 2.f)
         , auto const ms = matrix::Scaling(5.f, 5.f, 5.f)
         , auto const mt = matrix::Translation(10.f, 5.f, 7.f) )
    WHEN( auto const tx = mt * ms * mr )
    THEN( tx * p == Point(15.f, 0.f, 7.f) )
}

SCENARIO("The view transformation matrix for the default orientation", "math")
{
    GIVEN( auto const from = Point(0.f, 0.f, 0.f)
         , auto const to = Point(0.f, 0.f, -1.f)
         , auto const up = Vector(0.f, 1.f, 0.f) )
    WHEN( auto const t = matrix::View(from, to, up) )
    THEN( t == Mat44::Identity() )
}

SCENARIO("The view transformation matrix looking in positive z direction", "math")
{
    GIVEN( auto const from = Point(0.f, 0.f, 0.f)
         , auto const to = Point(0.f, 0.f, 1.f)
         , auto const up = Vector(0.f, 1.f, 0.f) )
    WHEN( auto const t = matrix::View(from, to, up) )
    THEN( t == matrix::Scaling(-1.f, 1.f, -1.f) )
}

SCENARIO("The view transformation moves the world", "math")
{
    GIVEN( auto const from = Point(0.f, 0.f, 8.f)
         , auto const to = Point(0.f, 0.f, 0.f)
         , auto const up = Vector(0.f, 1.f, 0.f) )
    WHEN( auto const t = matrix::View(from, to, up) )
    THEN( t == matrix::Translation(0.f, 0.f, -8.f) )
}

SCENARIO("An arbitrarry view transformation", "math")
{
    GIVEN( auto const from = Point(1.f, 3.f, 2.f)
         , auto const to = Point(4.f, -2.f, 8.f)
         , auto const up = Vector(1.f, 1.f, 0.f) )
    WHEN( auto const t = matrix::View(from, to, up) )
    THEN( t == Mat44({{-0.50709f, 0.50709f,  0.67612f, -2.36643f},
                      { 0.76772f, 0.60609f,  0.12122f, -2.82843f},
                      {-0.35857f, 0.59761f, -0.71714f,  0.f     },
                      { 0.f     , 0.f     ,  0.f     ,  1.f     }}) )
}
