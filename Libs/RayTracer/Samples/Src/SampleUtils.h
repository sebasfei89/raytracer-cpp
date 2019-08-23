#pragma once

#include "sampleutils_export.h"

#include <string>

class Camera;
class Canvas;
class World;

namespace SampleUtils
{

SAMPLEUTILS_EXPORT void SaveCanvas(Canvas const& canvas, std::string const& outputFile);
SAMPLEUTILS_EXPORT void RenderScene(Camera const& camera, World const& world, std::string const& outputFile = "");

}
