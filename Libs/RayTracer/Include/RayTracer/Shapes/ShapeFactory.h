#pragma once

#include "raytracer_export.h"

#include "Shape.h"
#include "../Types.h"

#include <nlohmann/json.hpp>

#include <functional>
#include <string>
#include <unordered_map>

using nlohmann::json;

class ShapeFactory
{
public:
    RAYTRACER_EXPORT static ShapeFactory& Get();

    using ShapeGenerator = std::function<ShapePtr(json const&, json const&, ArchetypeMap const&)>;
    bool Register(std::string const& name, ShapeGenerator generator);

    RAYTRACER_EXPORT ShapePtr Create(json const& data, json const& dataOverrides = {}, ArchetypeMap const& archetypes = {}) const;

private:
    ShapeFactory() = default;
    ShapeFactory(ShapeFactory const&) = delete;
    ~ShapeFactory() = default;

    std::unordered_map<std::string, ShapeGenerator> m_generators;
};

template <typename T>
class ShapeFactoryRegistration
{
public:
    ShapeFactoryRegistration(std::string const& type)
    {
        ShapeFactory::Get().Register(type, [](json const& data, json const& dataOverrides, ArchetypeMap const& archetypes)
        {
            auto shape = std::make_shared<T>();
            shape->Initialize(data, dataOverrides, archetypes);
            return shape;
        });
    }
};

#define REGISTER_SHAPE(T) namespace { static ShapeFactoryRegistration<T> s_##T(#T); }
