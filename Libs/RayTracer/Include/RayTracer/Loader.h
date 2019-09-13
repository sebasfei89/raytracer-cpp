#pragma once

#include "Types.h"

#include <nlohmann/json.hpp>

#include <istream>

using nlohmann::json;

class World;

class Loader
{
public:
    static bool Load(World& world, std::istream& is);

    static ShapePtr LoadObject(World const& world, json const& data);

private:
    constexpr static char const* json_key_objects = "objects";
    constexpr static char const* json_key_lights = "lights";
    constexpr static char const* json_key_archetypes = "archetypes";
    constexpr static char const* json_key_archetype = "archetype";
    constexpr static char const* json_key_type = "type";
    constexpr static char const* json_key_position = "position";
    constexpr static char const* json_key_intensity = "intensity";
    constexpr static char const* json_key_name = "name";
    constexpr static char const* json_key_scaling = "scaling";

    static bool LoadArchetypes(World& world, std::vector<json> const& archetypes);
    static bool LoadLight(World& world, json const& data);
    static ShapePtr LoadShape(json const& data);
    static ShapePtr LoadArchetypeInstance(World const& world, json const& data);
};
