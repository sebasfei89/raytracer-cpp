#include <RayTracer/Archetype.h>
#include <RayTracer/Material.h>
#include <RayTracer/Pattern.h>
#include <RayTracer/Shapes/Cone.h>
#include <RayTracer/Shapes/Cube.h>
#include <RayTracer/Shapes/Group.h>
#include <RayTracer/Shapes/Plane.h>
#include <RayTracer/Shapes/ShapeFactory.h>
#include <RayTracer/Shapes/Sphere.h>
#include <RayTracer/Transformations.h>

#include <Beddev/Beddev.h>

#include <memory>

SCENARIO("Loading a default Sphere", "ShapeFactory")
{
    GIVEN( auto sphere = std::dynamic_pointer_cast<Sphere>(ShapeFactory::Get().Create({{"type", "Sphere"}})) )
    THEN( sphere != nullptr
        , sphere->CastShadows()
        , sphere->GetBounds().Min() == Point(-1.f, -1.f, -1.f)
        , sphere->GetBounds().Max() == Point(1.f, 1.f, 1.f)
        , sphere->GetMaterial() == Material()
        , sphere->Name() == "<unnamed>"
        , sphere->Parent() == nullptr
        , sphere->Transform() == Mat44::Identity() )
}

SCENARIO("Loading a non-default Sphere", "ShapeFactory")
{
    GIVEN( auto j = R"({
            "name": "TestSphere", "type": "Sphere", "cast_shadows": false,
            "position": [1,0,0], "rotation": [0,0.785398,0], "scaling":[1,2,1],
            "material": {
                "ambient": 0.2, "diffuse": 0.8, "specular": 0.7, "shininess": 100,
                "reflective": 0.5, "transparency": 0.2, "refractiveIndex": 0.75,
                "pattern": { "type": "Solid", "color": [1,0,0] }
            }})"_json
        , auto m = Material(.2f, .8f, .7f, 100.f, .5f, .2f, .75f)
        , m.Pattern(std::make_shared<SolidPattern>(Color(1.f, 0.f, 0.f))) )
    WHEN( auto sphere = std::dynamic_pointer_cast<Sphere>(ShapeFactory::Get().Create(j)) )
    THEN( sphere != nullptr
        , !sphere->CastShadows()
        , sphere->GetBounds().Min() == Point(-1.f, -1.f, -1.f)
        , sphere->GetBounds().Max() == Point(1.f, 1.f, 1.f)
        , sphere->GetMaterial() == m
        , sphere->Name() == "TestSphere"
        , sphere->Parent() == nullptr
        , sphere->Transform() == matrix::Translation(1.f, 0.f, 0.f) * matrix::RotationY(PIOVR4) * matrix::Scaling(1.f, 2.f, 1.f) )
}

SCENARIO("Loading a default Cone", "ShapeFactory")
{
    GIVEN( auto cone = std::dynamic_pointer_cast<Cone>(ShapeFactory::Get().Create({ {"type", "Cone"} })) )
    THEN( cone != nullptr
        , cone->GetBounds().Min() == Point(-INF, -INF, -INF)
        , cone->GetBounds().Max() == Point(INF, INF, INF)
        , !cone->Closed()
        , cone->GetMaterial() == Material()
        , cone->Name() == "<unnamed>"
        , cone->Parent() == nullptr
        , cone->Transform() == Mat44::Identity() )
}

SCENARIO("Loading a non-default Cone", "ShapeFactory")
{
    GIVEN( auto j = R"({ "name": "TestCone", "type": "Cone"
                       , "position": [1,0,0], "closed": true, "min": 2, "max": 4
                       , "material": { "pattern": { "type": "PerlinNoise", "pattern": { "type": "Solid", "color": [0,1,0] } } }
                       })"_json
         , auto m = Material()
         , auto solid = std::make_shared<SolidPattern>(Color(0.f, 1.f, 0.f))
         , m.Pattern(std::make_shared<PerlinNoisePattern>(solid)) )
    WHEN( auto cone = std::dynamic_pointer_cast<Cone>(ShapeFactory::Get().Create(j)) )
    THEN( cone != nullptr
        , cone->GetBounds().Min() == Point(-4, 2, -4)
        , cone->GetBounds().Max() == Point(4, 4, 4)
        , cone->Closed()
        , cone->GetMaterial() == m
        , cone->Name() == "TestCone"
        , cone->Parent() == nullptr
        , cone->Transform() == matrix::Translation(1.f, 0.f, 0.f) )
}

SCENARIO("Loading a default Cylinder", "ShapeFactory")
{
    GIVEN( auto cyl = std::dynamic_pointer_cast<Cylinder>(ShapeFactory::Get().Create({ {"type", "Cylinder"} })) )
    THEN( cyl != nullptr
        , cyl->GetBounds().Min() == Point(-1, -INF, -1)
        , cyl->GetBounds().Max() == Point(1, INF, 1)
        , !cyl->Closed()
        , cyl->GetMaterial() == Material()
        , cyl->Name() == "<unnamed>"
        , cyl->Parent() == nullptr
        , cyl->Transform() == Mat44::Identity() )
}

SCENARIO("Loading a non-default Cylinder", "ShapeFactory")
{
    GIVEN( auto j = R"({ "name": "TestCylinder", "type": "Cylinder"
                       , "position": [1,0,0], "closed": true, "min": 2, "max": 4
                       , "material": { "pattern": { "type": "Strip", "pattern_a": { "type": "Solid", "color": [1,0,0] }, "pattern_b": { "type": "Solid", "color": [0,1,0] } } }
                       })"_json
         , auto m = Material()
         , m.Pattern(std::make_shared<StripPattern>(Color(1.f, 0.f, 0.f), Color(0.f, 1.f, 0.f))) )
    WHEN( auto cyl = std::dynamic_pointer_cast<Cylinder>(ShapeFactory::Get().Create(j)) )
    THEN( cyl != nullptr
        , cyl->GetBounds().Min() == Point(-1, 2, -1)
        , cyl->GetBounds().Max() == Point(1, 4, 1)
        , cyl->Closed()
        , cyl->GetMaterial() == m
        , cyl->Name() == "TestCylinder"
        , cyl->Parent() == nullptr
        , cyl->Transform() == matrix::Translation(1.f, 0.f, 0.f) )
}

SCENARIO("Loading a default Cube", "ShapeFactory")
{
    GIVEN( auto const cube = std::dynamic_pointer_cast<Cube>(ShapeFactory::Get().Create({ {"type", "Cube"} })) )
    THEN( cube != nullptr
        , cube->GetBounds().Min() == Point(-1, -1, -1)
        , cube->GetBounds().Max() == Point(1, 1, 1)
        , cube->GetMaterial() == Material()
        , cube->Name() == "<unnamed>"
        , cube->Parent() == nullptr
        , cube->Transform() == Mat44::Identity())
}

SCENARIO("Loading a non-default Cube", "ShapeFactory")
{
    GIVEN( auto j = R"({ "name": "TestCube", "type": "Cube"
                       , "position": [1,0,0], "scaling": [1, 2, 3]
                       , "material": { "pattern": { "type": "Gradient", "pattern_a": { "type": "Solid", "color": [1,0,0] }, "pattern_b": { "type": "Solid", "color": [0,1,0] } } }
                       })"_json
         , auto m = Material()
         , m.Pattern(std::make_shared<GradientPattern>(Color(1.f, 0.f, 0.f), Color(0.f, 1.f, 0.f))) )
    WHEN( auto cube = std::dynamic_pointer_cast<Cube>(ShapeFactory::Get().Create(j)) )
    THEN( cube != nullptr
        , cube->GetBounds().Min() == Point(-1, -1, -1)
        , cube->GetBounds().Max() == Point(1, 1, 1)
        , cube->GetMaterial() == m
        , cube->Name() == "TestCube"
        , cube->Parent() == nullptr
        , cube->Transform() == matrix::Translation(1.f, 0.f, 0.f) * matrix::Scaling(1.f, 2.f, 3.f) )
}

SCENARIO("Loading a default Plane", "ShapeFactory")
{
    GIVEN( auto const plane = std::dynamic_pointer_cast<Plane>(ShapeFactory::Get().Create({ {"type", "Plane"} })) )
    THEN( plane != nullptr
        , plane->GetBounds().Min() == Point(-INF, 0, -INF)
        , plane->GetBounds().Max() == Point(INF, 0, INF)
        , plane->GetMaterial() == Material()
        , plane->Name() == "<unnamed>"
        , plane->Parent() == nullptr
        , plane->Transform() == Mat44::Identity())
}

SCENARIO("Loading a non-default Plane", "ShapeFactory")
{
    GIVEN( auto j = R"({ "name": "TestPlane", "type": "Plane"
                       , "position": [1,0,0]
                       , "material": { "pattern": { "type": "Ring"
                                     , "pattern_a": { "type": "Checker", "pattern_a": { "type": "Solid", "color": [1,0,0] }, "pattern_b": { "type": "Solid", "color": [0,1,0] } }
                                     , "pattern_b": { "type": "RadialGradient", "pattern_a": { "type": "Solid", "color": [1,0,0] }, "pattern_b": { "type": "Solid", "color": [0,1,0] } } } }
                       })"_json
         , auto m = Material()
         , m.Pattern(std::make_shared<RingPattern>(std::make_shared<CheckerPattern>(Color(1.f, 0.f, 0.f), Color(0.f, 1.f, 0.f)), std::make_shared<RadialGradientPattern>(Color(1.f, 0.f, 0.f), Color(0.f, 1.f, 0.f)))) )
    WHEN( auto plane = std::dynamic_pointer_cast<Plane>(ShapeFactory::Get().Create(j)) )
    THEN( plane != nullptr
        , plane->GetBounds().Min() == Point(-INF, 0.f, -INF)
        , plane->GetBounds().Max() == Point(INF, 0.f, INF)
        , plane->GetMaterial() == m
        , plane->Name() == "TestPlane"
        , plane->Parent() == nullptr
        , plane->Transform() == matrix::Translation(1.f, 0.f, 0.f) )
}

SCENARIO("Loading a default empty Group", "ShapeFactory")
{
    GIVEN( auto const group = std::dynamic_pointer_cast<Group>(ShapeFactory::Get().Create({ {"type", "Group"} })) )
    THEN( group != nullptr
        , group->Children().size() == 0
        , group->GetBounds().Min() == Point(INF, INF, INF)
        , group->GetBounds().Max() == Point(-INF, -INF, -INF)
        , group->GetMaterial() == Material()
        , group->Name() == "<unnamed>"
        , group->Parent() == nullptr
        , group->Transform() == Mat44::Identity())
}

SCENARIO("Loading a non-default non-empty Group", "ShapeFactory")
{
    GIVEN( auto j = R"({ "name": "TestGroup", "type": "Group", "position": [1,0,0]
                       , "material": { "pattern": { "type": "Blend", "pattern_a": { "type": "Solid", "color": [1,0,0] }, "pattern_b": { "type": "Solid", "color": [0,1,0] } } }
                       , "children": [{ "name": "ChildA", "type": "Cube", "position": [1,0,0]}]
                       })"_json
         , auto m = Material()
         , m.Pattern(std::make_shared<BlendPattern>(Color(1.f, 0.f, 0.f), Color(0.f, 1.f, 0.f))) )
    WHEN( auto const group = std::dynamic_pointer_cast<Group>(ShapeFactory::Get().Create(j)) )
    THEN( group != nullptr
        , group->Children().size() == 1
        , group->Children()[0]->Name() == "ChildA"
        , std::dynamic_pointer_cast<Cube>(group->Children()[0]) != nullptr
        , group->GetBounds().Min() == Point(0, -1, -1)
        , group->GetBounds().Max() == Point(2, 1, 1)
        , group->GetMaterial() == m
        , group->Name() == "TestGroup"
        , group->Parent() == nullptr
        , group->Transform() == matrix::Translation(1.f, 0.f, 0.f) )
}

SCENARIO("Trying to load an invalid archetype", "ShapeFactory")
{
    GIVEN( auto j = R"({ "name": "TestArchetypeA"
                       , "archetype": "ArchetypeB"
                       , "position": [1,0,0]
                       })"_json
         , ArchetypeMap archetypes
         , archetypes["ArchetypeA"] = std::make_shared<Archetype>("ArchetypeA", R"({ "name": "ArchetypeA", "type": "Sphere", "scaling": [2,2,2] })"_json) )
    WHEN( auto const instance = std::dynamic_pointer_cast<Sphere>(ShapeFactory::Get().Create(j, {}, archetypes)) )
    THEN( instance == nullptr )
}

SCENARIO("Loading an archetype instance", "ShapeFactory")
{
    GIVEN( auto j = R"({ "name": "TestArchetypeA"
                       , "archetype": "ArchetypeA"
                       , "position": [1,0,0]
                       })"_json
         , ArchetypeMap archetypes
         , archetypes["ArchetypeA"] = std::make_shared<Archetype>("ArchetypeA", R"({ "name": "ArchetypeA", "type": "Sphere", "scaling": [2,2,2] })"_json) )
    WHEN( auto const instance = std::dynamic_pointer_cast<Sphere>(ShapeFactory::Get().Create(j, {}, archetypes)) )
    THEN( instance != nullptr
        , instance->GetBounds().Min() == Point(-1, -1, -1)
        , instance->GetBounds().Max() == Point(1, 1, 1)
        , instance->GetMaterial() == Material()
        , instance->Name() == "TestArchetypeA"
        , instance->Parent() == nullptr
        , instance->Transform() == matrix::Translation(1.f, 0.f, 0.f) * matrix::Scaling(2.f, 2.f, 2.f) )
}

SCENARIO("Loading a Group with an archetype instance", "ShapeFactory")
{
    GIVEN( auto j = R"({ "name": "GroupWithArchetype"
                       , "type": "Group"
                       , "children": [{ "name": "ChildA", "archetype": "Arch1", "position": [1,0,0]}]
                       })"_json
         , ArchetypeMap archetypes
         , archetypes["Arch1"] = std::make_shared<Archetype>("Arch1", R"({ "name": "Arch1", "type": "Sphere", "scaling": [2,2,2] })"_json) )
    WHEN( auto const group = std::dynamic_pointer_cast<Group>(ShapeFactory::Get().Create(j, {}, archetypes)) )
    THEN( group != nullptr
        , group->GetBounds().Min() == Point(-1, -2, -2)
        , group->GetBounds().Max() == Point(3, 2, 2)
        , group->GetMaterial() == Material()
        , group->Name() == "GroupWithArchetype"
        , group->Parent() == nullptr
        , group->Transform() == Mat44::Identity()
        , group->Children().size() == 1
        , group->Children()[0]->Name() == "ChildA"
        , group->Children()[0]->Transform() == matrix::Translation(1.f, 0.f, 0.f) * matrix::Scaling(2.f, 2.f, 2.f)
        , std::dynamic_pointer_cast<Sphere>(group->Children()[0]) != nullptr )
}
