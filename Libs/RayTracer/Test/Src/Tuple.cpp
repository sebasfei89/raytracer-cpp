#include <RayTracer/Tuple.h>
#include <RayTracer/Util.h>

#include <Beddev/Beddev.h>

SCENARIO("A default constructed Tuple is initialized to all 0s", "math")
{
    GIVEN( Tuple t )
    THEN( t[0] == 0.f
        , t[1] == 0.f
        , t[2] == 0.f
        , t[3] == 0.f
        , t.IsPoint() == false
        , t.IsVector() == true )
}

SCENARIO("A Tuple with w == 1.0 is a Point", "math")
{
    GIVEN( Tuple t(4.3f, -4.2f, 3.1f, 1.0f) )
    THEN( t[0] == 4.3f
        , t[1] == -4.2f
        , t[2] == 3.1f
        , t[3] == 1.0f
        , t.IsPoint() == true
        , t.IsVector() == false )
}

SCENARIO("A Tuple with w == 0.0 is a Vector", "math")
{
    GIVEN( Tuple t(4.3f, -4.2f, 3.1f, 0.0f) )
    THEN( t[0] == 4.3f
        , t[1] == -4.2f
        , t[2] == 3.1f
        , t[3] == 0.0f
        , t.IsPoint() == false
        , t.IsVector() == true )
}

SCENARIO("Point() cretes Tuples with w == 1.0", "math")
{
    GIVEN( Tuple p = Point(4.3f, -4.2f, 3.1f) )
    THEN( p[3] == 1.0f )
}

SCENARIO("Vector() cretes Tuples with w == 0.0", "math")
{
    GIVEN( Tuple v = Vector(4.3f, -4.2f, 3.1f) )
    THEN( v[3] == 0.0f )
}

SCENARIO("Adding two Tuples", "math")
{
    GIVEN( Tuple p(3.f, -2.f, 5.f, 1.f)
         , Tuple v(-2.f, 3.f, 1.f, 0.f) )
    THEN( p + v == Tuple(1.f, 1.f, 6.f, 1.f) )
}

SCENARIO("Substracting two points", "math")
{
    GIVEN( Tuple p1 = Point(3.f, 2.f, 1.f)
         , Tuple p2 = Point(5.f, 6.f, 7.f) )
    THEN( p1 - p2 == Vector(-2.f, -4.f, -6.f) )
}

SCENARIO("Substracting a point and a vector", "math")
{
    GIVEN( Tuple p = Point(3.f, 2.f, 1.f)
         , Tuple v = Vector(5.f, 6.f, 7.f) )
    THEN( p - v == Point(-2.f, -4.f, -6.f) )
}

SCENARIO("Substracting two vectors", "math")
{
    GIVEN( Tuple v1 = Vector(3.f, 2.f, 1.f)
         , Tuple v2 = Vector(5.f, 6.f, 7.f) )
    THEN( v1 - v2 == Vector(-2.f, -4.f, -6.f) )
}

SCENARIO("Substracting a vector from the zero vector", "math")
{
    GIVEN( Tuple zeroVector = Vector(0.f, 0.f, 0.f)
         , Tuple v = Vector(1.f, -2.f, 3.f) )
    THEN( zeroVector - v == Vector(-1.f, 2.f, -3.f) )
}

SCENARIO("Negating a tuple", "math")
{
    GIVEN( Tuple t(1.f, -2.f, 3.f, -4.f) )
    THEN( -t == Tuple(-1.f, 2.f, -3.f, 4.f) )
}

SCENARIO("Multiplication/Divition of a tuple by a scalar", "math")
{
    GIVEN( Tuple const t(1.f, -2.f, 3.f, -4.f)
         , float const s = 0.5f )
    THEN( t * 3.5f == Tuple(3.5f, -7.f, 10.5f, -14.f)
        , t / 2.f == Tuple(0.5f, -1.f, 1.5f, -2.f)
        , t * s == Tuple(0.5f, -1.f, 1.5f, -2.f) )
}

SCENARIO("Magnitude of the x-unit Vector", "math")
{
    GIVEN( Tuple const v = Vector(1.f, 0.f, 0.f) )
    THEN( v.Length() == 1.f )
}

SCENARIO("Magnitude of the y-unit Vector", "math")
{
    GIVEN( Tuple const v = Vector(0.f, 1.f, 0.f) )
    THEN( v.Length() == 1.f )
}

SCENARIO("Magnitude of the z-unit Vector", "math")
{
    GIVEN( Tuple const v = Vector(0.f, 0.f, 1.f) )
    THEN( v.Length() == 1.f )
}

SCENARIO("Magnitude of an arbitrary Vector", "math")
{
    GIVEN( Tuple const v = Vector(1.f, 2.f, 3.f) )
    THEN( v.Length() == std::sqrtf(14.f)
        , (-v).Length() == std::sqrtf(14.f) )
}

SCENARIO("Vector normalization", "math")
{
    GIVEN( Tuple v = Vector(4.f, 0.f, 0.f) )
    WHEN( v.Normalize() )
    THEN( v == Vector(1.f, 0.f, 0.f) )
}

SCENARIO("Vector normalization", "math")
{
    GIVEN( Tuple v = Vector(1.f, 2.f, 3.f) )
    WHEN( v.Normalize() )
    THEN( v == Vector(0.26726f, 0.53452f, 0.80178f)
        , Equals(v.Length(), 1.f) )
}

SCENARIO("Vectors dot product", "math")
{
    GIVEN( Tuple const v1 = Vector(1.f, 2.f, 3.f)
         , Tuple const v2 = Vector(2.f, 3.f, 4.f) )
    THEN( v1.Dot(v2) == 20.f )
}

SCENARIO("Vectors cross product", "math")
{
    GIVEN( Tuple const v1 = Vector(1.f, 2.f, 3.f)
         , Tuple const v2 = Vector(2.f, 3.f, 4.f) )
    THEN( v1.Cross(v2) == Vector(-1.f, 2.f, -1.f)
        , v2.Cross(v1) == Vector(1.f, -2.f, 1.f) )
}

SCENARIO("Reflecting a vector approaching at 45 deg", "math")
{
    GIVEN( Tuple const v = Vector(1.f, -1.f, 0.f)
         , Tuple const n = Vector(0.f, 1.f, 0.f) )
    WHEN( auto const r = v.Reflect(n) )
    THEN( r == Vector(1.f, 1.f, 0.f) )
}

SCENARIO("Reflecting a vector off a slanted surface", "math")
{
    GIVEN( float const coord = SQRT2OVR2
         , Tuple const v = Vector(0.f, -1.f, 0.f)
         , Tuple const n = Vector(coord, coord, 0.f) )
    WHEN( auto const r = v.Reflect(n) )
    THEN( r == Vector(1.f, 0.f, 0.f) )
}
