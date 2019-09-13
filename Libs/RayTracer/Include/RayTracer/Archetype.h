#pragma once

#include <nlohmann/json.hpp>

#include <string>

using nlohmann::json;

class World;

class Archetype
{
public:
    Archetype(std::string const& name, json const& tpl)
        : m_name(name), m_template(tpl) {}

    std::string const& Name() const { return m_name; }
    json const& Template() const { return m_template; }

private:
    std::string m_name;
    json m_template;
};
