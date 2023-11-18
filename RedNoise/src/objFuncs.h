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



glm::vec3 convMtlToInt(glm::vec3 colourValues);

std::unordered_map<std::string, glm::vec3> loadMatOBJ();

std::vector<ModelTriangle> loadGeoOBJ(float scalingFactor);

#endif // !OBJFUNCS_H
