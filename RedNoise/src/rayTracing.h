#ifndef RAYTRACING_H
#define RAYTRACING_H
#include "RayTriangleIntersection.h"
#include <ModelTriangle.h>
#include <utils.h>
#include <glm/ext.hpp>

void getClosestValidIntersection(std::vector<ModelTriangle> triangle, glm::vec3 position, glm::vec3 direction);

void directionVectorCalcs(glm::vec3 cameraPosition);

glm::vec3 getIntersectionPoint(glm::vec3 cameraPosition, glm::vec3 rayDirection, float t);

glm::vec3 normalise(glm::vec3 vector);

#endif // !RAYTRACING_H
