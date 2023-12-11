#ifndef OBJFUNCS_H
#define OBJFUNCS_H
#include <iostream>
#include <fstream>
#include <Utils.h>
#include <Colour.h>
#include <string>
#include <sstream>
#include <ModelTriangle.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/ext.hpp>
#include "RedNoise.h"



glm::vec3 convMtlToInt(glm::vec3 colourValues);

std::unordered_map<std::string, glm::vec4> loadMatOBJ();

std::vector<ModelTriangle> loadGeoOBJ(float scalingFactor, DrawingWindow& window);

glm::vec3 calcSurfaceNormal(ModelTriangle);

Uint32 convColourToUint32(Colour colour);

CanvasTriangle convertModelTriToCanvas(ModelTriangle modelTriangle);

#endif // !OBJFUNCS_H
