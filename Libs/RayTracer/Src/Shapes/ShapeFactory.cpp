#include "Shapes/ShapeFactory.h"

#include "Archetype.h"
#include "Shapes/Shape.h"

namespace
{

constexpr static char const* json_key_type = "type";
constexpr static char const* json_key_archetype = "archetype";

}

// static
ShapeFactory& ShapeFactory::Get()
{
    static ShapeFactory s_factory;
    return s_factory;
}

bool ShapeFactory::Register(std::string const& name, ShapeGenerator generator)
{
    return m_generators.insert(std::make_pair(name, generator)).second;
}

ShapePtr ShapeFactory::Create(json const& data, json const& dataOverrides, ArchetypeMap const& archetypes /*= {}*/) const
{
    if (data.contains(json_key_type))
    {
        std::string const type = data.at(json_key_type);
        if (auto gen = m_generators.find(type); gen != m_generators.end())
        {
            return gen->second(data, dataOverrides, archetypes);
        }
    }
    else if (data.contains(json_key_archetype))
    {
        std::string const archetypeName = data.at(json_key_archetype);
        if (archetypes.count(archetypeName) == 0)
        {
            return nullptr;
        }

        auto archetype = archetypes.at(archetypeName);
        return Create(archetype->Template(), data, archetypes);
    }

    return nullptr;
}
