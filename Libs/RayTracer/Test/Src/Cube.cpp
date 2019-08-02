#include <Beddev/Beddev.h>

#include <RayTracer/Cube.h>
#include <RayTracer/Ray.h>

#define MAKE_SCENARIO(DESC, CAT, POS, DIR, T1, T2)\
    SCENARIO(DESC, CAT)\
    {\
        GIVEN( auto const c = std::make_shared<Cube>()\
             , auto const r = Ray(POS, DIR)\
             , std::vector<Intersection> xs = {} )\
        WHEN( r.Intersect(c, xs) )\
        THEN( xs.size() == 2\
            , xs[0].Distance() == T1\
            , xs[1].Distance() == T2 )\
    }

MAKE_SCENARIO("A ray intersects a cube on +x side", "shape,cube", Point(5.f, .5f, 0.f), Vector(-1.f, 0.f, 0.f), 4.f, 6.f)
MAKE_SCENARIO("A ray intersects a cube on -x side", "shape,cube", Point(-5.f, .5f, 0.f), Vector(1.f, 0.f, 0.f), 4.f, 6.f)
MAKE_SCENARIO("A ray intersects a cube on +y side", "shape,cube", Point(.5f, 5.f, 0.f), Vector(0.f, -1.f, 0.f), 4.f, 6.f)
MAKE_SCENARIO("A ray intersects a cube on -y side", "shape,cube", Point(.5f, -5.f, 0.f), Vector(0.f, 1.f, 0.f), 4.f, 6.f)
MAKE_SCENARIO("A ray intersects a cube on +z side", "shape,cube", Point(.5f, 0.f, 5.f), Vector(0.f, 0.f, -1.f), 4.f, 6.f)
MAKE_SCENARIO("A ray intersects a cube on -z side", "shape,cube", Point(.5f, 0.f, -5.f), Vector(0.f, 0.f, 1.f), 4.f, 6.f)
MAKE_SCENARIO("A ray intersects a cube from inside", "shape,cube", Point(0.f, .5f, 0.f), Vector(0.f, 0.f, 1.f), -1.f, 1.f)

#define MAKE_SCENARIO2(DESC, CAT, POS, DIR)\
    SCENARIO(DESC, CAT)\
    {\
        GIVEN( auto const c = std::make_shared<Cube>()\
             , auto const r = Ray(POS, DIR)\
             , std::vector<Intersection> xs = {} )\
        WHEN( r.Intersect(c, xs) )\
        THEN( xs.size() == 0 )\
    }

MAKE_SCENARIO2("A ray misses a cube 1", "shape,cube", Point(-2.f, 0.f, 0.f), Vector(0.2673f, 0.5345f, 0.8018f));
MAKE_SCENARIO2("A ray misses a cube 2", "shape,cube", Point( 0.f,-2.f, 0.f), Vector(0.8018f, 0.2673f, 0.5345f));
MAKE_SCENARIO2("A ray misses a cube 3", "shape,cube", Point( 0.f, 0.f,-2.f), Vector(0.5345f, 0.8018f, 0.2673f));
MAKE_SCENARIO2("A ray misses a cube 4", "shape,cube", Point( 2.f, 0.f, 2.f), Vector( 0.f, 0.f,-1.f));
MAKE_SCENARIO2("A ray misses a cube 5", "shape,cube", Point( 0.f, 2.f, 2.f), Vector( 0.f,-1.f, 0.f));
MAKE_SCENARIO2("A ray misses a cube 6", "shape,cube", Point( 2.f, 2.f, 0.f), Vector(-1.f, 0.f, 0.f));

#define MAKE_SCENARIO3(DESC, CAT, POINT, NORMAL)\
    SCENARIO(DESC, CAT)\
    {\
        GIVEN( auto const c = std::make_shared<Cube>()\
             , auto const p = POINT )\
        WHEN( auto const n = c->NormalAtLocal(p) )\
        THEN( n == NORMAL )\
    }

MAKE_SCENARIO3("The normal on the surface of a cube 1", "shape,cube", Point( 1.f, .5f,-.8f), Vector( 1.f, 0.f, 0.f));
MAKE_SCENARIO3("The normal on the surface of a cube 2", "shape,cube", Point(-1.f,-.2f, .9f), Vector(-1.f, 0.f, 0.f));
MAKE_SCENARIO3("The normal on the surface of a cube 3", "shape,cube", Point(-.4f, 1.f,-.1f), Vector( 0.f, 1.f, 0.f));
MAKE_SCENARIO3("The normal on the surface of a cube 4", "shape,cube", Point( .3f,-1.f,-.7f), Vector( 0.f,-1.f, 0.f));
MAKE_SCENARIO3("The normal on the surface of a cube 5", "shape,cube", Point(-.6f, .3f, 1.f), Vector( 0.f, 0.f, 1.f));
MAKE_SCENARIO3("The normal on the surface of a cube 6", "shape,cube", Point( .4f, .4f,-1.f), Vector( 0.f, 0.f,-1.f));
MAKE_SCENARIO3("The normal on the surface of a cube 7", "shape,cube", Point( 1.f, 1.f, 1.f), Vector( 1.f, 0.f, 0.f));
MAKE_SCENARIO3("The normal on the surface of a cube 8", "shape,cube", Point(-1.f,-1.f,-1.f), Vector(-1.f, 0.f, 0.f));
