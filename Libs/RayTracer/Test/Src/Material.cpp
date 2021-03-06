#include <RayTracer/Material.h>

#include <Beddev/Beddev.h>

SCENARIO("The default material", "[Materials]")
{
    GIVEN( auto const m = Material() )
    THEN( m.GetColor() == Color(1.f, 1.f, 1.f)
        , m.Ambient() == 0.1f
        , m.Diffuse() == 0.9f
        , m.Specular() == 0.9f
        , m.Reflective() == 0.f
        , m.Transparency() == 0.f
        , m.RefractiveIndex() == 1.f
        , m.Shininess() == 200.f )
}
