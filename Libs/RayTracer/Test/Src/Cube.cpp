#include <Beddev/Beddev.h>

#include <RayTracer/Cube.h>
#include <RayTracer/Ray.h>

struct ArgsT
{
    Tuple pos;
    Tuple dir;
    float t1;
    float t2;
};

PSCENARIO(ArgsT, "A ray intersects a cube", "shape,cube")
{
    PARAMS( ArgsT{ Point(5.f, .5f, 0.f), Vector(-1.f, 0.f, 0.f), 4.f, 6.f }
          , ArgsT{ Point(-5.f, .5f, 0.f), Vector(1.f, 0.f, 0.f), 4.f, 6.f }
          , ArgsT{ Point(.5f, 5.f, 0.f), Vector(0.f, -1.f, 0.f), 4.f, 6.f }
          , ArgsT{ Point(.5f, -5.f, 0.f), Vector(0.f, 1.f, 0.f), 4.f, 6.f }
          , ArgsT{ Point(.5f, 0.f, 5.f), Vector(0.f, 0.f, -1.f), 4.f, 6.f }
          , ArgsT{ Point(.5f, 0.f, -5.f), Vector(0.f, 0.f, 1.f), 4.f, 6.f }
          , ArgsT{ Point(0.f, .5f, 0.f), Vector(0.f, 0.f, 1.f), -1.f, 1.f } )
    GIVEN( auto const arg = GetParam()
         , auto const c = std::make_shared<Cube>()
         , auto const r = Ray(arg.pos, arg.dir)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(c, xs) )
    THEN( xs.size() == 2
        , xs[0].Distance() == arg.t1
        , xs[1].Distance() == arg.t2 )
}

PSCENARIO(ArgsT, "A ray misses a cube", "shape,cube")
{
    PARAMS( ArgsT{ Point(-2.f, 0.f, 0.f), Vector(0.2673f, 0.5345f, 0.8018f), 0.f, 0.f }
          , ArgsT{ Point(0.f,-2.f, 0.f), Vector(0.8018f, 0.2673f, 0.5345f), 0.f, 0.f }
          , ArgsT{ Point(0.f, 0.f,-2.f), Vector(0.5345f, 0.8018f, 0.2673f), 0.f, 0.f }
          , ArgsT{ Point(2.f, 0.f, 2.f), Vector(0.f, 0.f,-1.f), 0.f, 0.f }
          , ArgsT{ Point(0.f, 2.f, 2.f), Vector(0.f,-1.f, 0.f), 0.f, 0.f }
          , ArgsT{ Point(2.f, 2.f, 0.f), Vector(-1.f, 0.f, 0.f), 0.f, 0.f } )
    GIVEN( auto const c = std::make_shared<Cube>()
         , auto const arg = GetParam()
         , auto const r = Ray(arg.pos, arg.dir)
         , std::vector<Intersection> xs = {} )
    WHEN( r.Intersect(c, xs) )
    THEN( xs.size() == 0 )
}

PSCENARIO(ArgsT, "A ray intersects a cube", "shape,cube")
{
    PARAMS( ArgsT{ Point(1.f, .5f,-.8f), Vector(1.f, 0.f, 0.f), 0.f, 0.f }
          , ArgsT{ Point(-1.f,-.2f, .9f), Vector(-1.f, 0.f, 0.f), 0.f, 0.f }
          , ArgsT{ Point(-.4f, 1.f,-.1f), Vector(0.f, 1.f, 0.f), 0.f, 0.f }
          , ArgsT{ Point(.3f,-1.f,-.7f), Vector(0.f,-1.f, 0.f), 0.f, 0.f }
          , ArgsT{ Point(-.6f, .3f, 1.f), Vector(0.f, 0.f, 1.f), 0.f, 0.f }
          , ArgsT{ Point(.4f, .4f,-1.f), Vector(0.f, 0.f,-1.f), 0.f, 0.f }
          , ArgsT{ Point(1.f, 1.f, 1.f), Vector(1.f, 0.f, 0.f), 0.f, 0.f }
          , ArgsT{ Point(-1.f,-1.f,-1.f), Vector(-1.f, 0.f, 0.f), 0.f, 0.f } )
    GIVEN( auto const c = std::make_shared<Cube>()
         , auto const arg = GetParam()
         , auto const p = arg.pos )
    WHEN( auto const n = c->NormalAtLocal(p) )
    THEN( n == arg.dir )
}
