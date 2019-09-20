#pragma once

#include <map>
#include <memory>

class Archetype;
using ArchetypePtr = std::shared_ptr<Archetype>;
using ArchetypeConstPtr = std::shared_ptr<Archetype const>;
using ArchetypeMap = std::map<std::string, ArchetypeConstPtr>;

class Shape;
using ShapePtr = std::shared_ptr<Shape>;
using ShapeConstPtr = std::shared_ptr<Shape const>;

class IPattern;
using PatternPtr = std::shared_ptr<IPattern>;
using PatternConstPtr = std::shared_ptr<IPattern const>;
