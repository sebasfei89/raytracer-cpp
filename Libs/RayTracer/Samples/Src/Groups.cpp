#include <RayTracer/Camera.h>
#include <RayTracer/Cylinder.h>
#include <RayTracer/Group.h>
#include <RayTracer/Sphere.h>
#include <RayTracer/Transformations.h>
#include <RayTracer/World.h>

#include <SampleUtils.h>

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace
{

std::string const scene = R"({
    "objects": [{
        "name": "Hexagon0"
        "archetpye": "Hexagon"
    },{
        "name": "sun",
        "type": "pointlight",
        "position": [0, 5, 0],
        "intensity: [1, 1, 1]"
    }],
    "archetypes": [{
        "name": "Hexagon",
        "type": "Group",
        "children": [{
            "name": "Side0"
            "archetpye": "HexagonSide"
        },{
            "name": "Side1"
            "archetpye": "HexagonSide",
            "rotation": [0, 1.04719755, 0]
        },{
            "name": "Side2"
            "archetpye": "HexagonSide",
            "rotation": [0, 2.09439510, 0]
        },{
            "name": "Side3"
            "archetpye": "HexagonSide",
            "rotation": [0, 3.14159265, 0]
        },{
            "name": "Side4"
            "archetpye": "HexagonSide",
            "rotation": [0, 4.18879020, 0]
        },{
            "name": "Side5"
            "archetpye": "HexagonSide",
            "rotation": [0, 5.23598776, 0]
        }]
    },{
        "name": "HexagonSide",
        "type": "Group",
        "children": [{
            "name": "Edge"
            "type": "Cylinder",
            "caps": [0, 1],
            "position": [0, 0, -1],
            "rotation": [0, -0.52359878, -1.57079633],
            "scaling": [0.25, 1.0, 0.25]
        },{
            "name": "Corner"
            "type": "Sphere",
            "position": [0, 0, -1],
            "scaling": [0.25, 0.25, 0.25]
        }]
    }]
})"_json;

}

ShapePtr HexagonCorner()
{
    auto corner = std::make_shared<Sphere>();
    corner->SetTransform( matrix::Translation(0.f, 0.f, -1.f) * matrix::Scaling(.25f, .25f, .25f) );
    return corner;
}

ShapePtr HexagonEdge()
{
    auto edge = std::make_shared<Cylinder>(0.f, 1.f);
    edge->SetTransform( matrix::Translation(0.f, 0.f, -1.f)
                      * matrix::RotationY(-PIOVR6)
                      * matrix::RotationZ(-PIOVR2)
                      * matrix::Scaling(.25f, 1.f, .25f) );
    return edge;
}

ShapePtr HexagonSide()
{
    auto side = std::make_shared<Group>();
    side->AddChild(HexagonCorner());
    side->AddChild(HexagonEdge());
    return side;
}

ShapePtr Hexagon()
{
    auto hex = std::make_shared<Group>();
    for (int i = 0; i < 6; i++)
    {
        auto side = HexagonSide();
        side->SetTransform(matrix::RotationY(i * PIOVR3));
        hex->AddChild(side);
    }
    return hex;
}

int main()
{
    auto world = World();

    world.Add(PointLight(Point(0.f, 5.f, 0.f), { 1.f, 1.f, 1.f }));
    world.Add(Hexagon());

    auto camera = Camera(800, 600, PI / 3.f);
    camera.SetTransform(matrix::View(Point(0.f, 3.f, -3.f), Point(0.f, -1.f, 1.f), Vector(0.f, 1.f, 0.f)));

    SampleUtils::RenderScene(camera, world, "Ch14_Groups.ppm");

    return 0;
}
