#ifndef RAYTRACING_H
#define RAYTRACING_H
#include "RayTriangleIntersection.h"
#include <ModelTriangle.h>
#include <utils.h>
#include <glm/ext.hpp>
#include "camera.h"
#include <CanvasPoint.h>

RayTriangleIntersection getClosestValidIntersection(std::vector<ModelTriangle>& triangle, glm::vec3 position, glm::vec3 direction);

glm::vec3 directionVectorCalcs(Camera& camera);

glm::vec3 getPointInWorld(float u, float v, Camera& camera);

glm::vec3 getDirectionVector(glm::vec3 vertexPosition, Camera& camera);

glm::vec3 getTriangleIntersectionPointT(glm::vec3 cameraPosition, glm::vec3 rayDirection, float t);

glm::vec3 normalise(glm::vec3 vector);

#endif // !RAYTRACING_H
