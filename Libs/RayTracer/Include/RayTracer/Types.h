#pragma once

#include <memory>

class Shape;
using ShapePtr = std::shared_ptr<Shape>;
using ShapeConstPtr = std::shared_ptr<Shape const>;

class IPattern;
using PatternPtr = std::shared_ptr<IPattern>;
using PatternConstPtr = std::shared_ptr<IPattern const>;
