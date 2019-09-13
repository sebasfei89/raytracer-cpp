#include "Loader.h"

#include "Archetype.h"
#include "Cube.h"
#include "Group.h"
#include "Tuple.h"
#include "World.h"

#include <nlohmann/json.hpp>

using nlohmann::json;

void to_json(json& j, Tuple const& t)
{
    j = json::array({ t[0], t[1], t[2], t[3] });
}

void from_json(json const& j, Tuple& t)
{
    auto coords = j.get<std::vector<float>>();
    for (int i = 0; i < coords.size(); i++)
    {
        t[i] = coords[i];
    }
}

bool Loader::Load(World& world, std::istream& is)
{
    auto data = json::parse(is, nullptr, false);
    if ((data == json::value_t::discarded) || (data.type() != json::value_t::object))
    {
        return false;
    }

    if (data.contains(json_key_archetypes))
    {
        if (!LoadArchetypes(world, data.at(json_key_archetypes)))
        {
            return false;
        }
    }

    if (data.contains(json_key_lights))
    {
        std::vector<json> lights = data.at(json_key_lights);
        for (auto const& light : lights)
        {
            if (!LoadLight(world, light))
            {
                return false;
            }
        }
    }

    if (data.contains(json_key_objects))
    {
        std::vector<json> objects = data.at(json_key_objects);
        for (auto const& obj : objects)
        {
            auto shape = LoadObject(world, obj);
            if (shape)
            {
                world.Add(shape);
            }
            else
            {
                return false;
            }
        }
    }

    return true;
}

ShapePtr Loader::LoadObject(World const& world, json const& data)
{
    if (data.contains(json_key_type))
    {
        return LoadShape(data);
    }
    else if (data.contains(json_key_archetype))
    {
        return LoadArchetypeInstance(world, data);
    }

    return nullptr;
}

bool Loader::LoadArchetypes(World& world, std::vector<json> const& archetypes)
{
    for (auto const& arcTemplate : archetypes)
    {
        if (!arcTemplate.contains(json_key_name))
        {
            return false;
        }

        std::string name = arcTemplate.at(json_key_name);
        world.Add(std::make_shared<Archetype>(name, arcTemplate));
    }
    return true;
}

ShapePtr Loader::LoadArchetypeInstance(World const& world, json const& data)
{
    std::string const archetypeName = data.at(json_key_archetype);
    auto archetype = world.FindArchetype(archetypeName);
    if (!archetype)
    {
        return nullptr;
    }

    ShapePtr shape = Loader::LoadObject(world, archetype->Template());
    if (!shape)
    {
        return nullptr;
    }

    if (data.contains(json_key_scaling))
    {
        Tuple scaling = data.at(json_key_scaling);
        shape->SetTransform(matrix::Scaling(scaling.X(), scaling.Y(), scaling.Z()));
    }

    std::string name = "<unnamed>";
    if (data.contains("name"))
    {
        name = data.at("name");
    }
    shape->Name(name);

    return shape;
}

bool Loader::LoadLight(World& world, json const& data)
{
    Tuple pos;
    if (data.contains(json_key_position))
    {
        pos = data.at(json_key_position);
        pos[3] = 1.f;
    }

    Tuple intensity;
    if (data.contains(json_key_intensity))
    {
        intensity = data.at(json_key_intensity);
    }

    auto light = PointLight(pos, intensity);
    if (data.contains(json_key_name))
    {
        light.SetName(data.at(json_key_name));
    }
    world.Add(light);

    return true;
}

ShapePtr Loader::LoadShape(json const& data)
{
    ShapePtr shape;

    std::string type = data.at(json_key_type);
    if (type == "Cube")
    {
        shape = std::make_shared<Cube>();
    }
    else if(type == "Group")
    {
        shape = std::make_shared<Group>();
    }
    else
    {
        return nullptr;
    }

    if (data.contains(json_key_scaling))
    {
        Tuple scaling = data.at(json_key_scaling);
        shape->SetTransform(matrix::Scaling(scaling.X(), scaling.Y(), scaling.Z()));
    }

    shape->Name(data.contains(json_key_name) ? data.at(json_key_name) : "<unnamed>");
    return shape;
}
