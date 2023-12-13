#ifndef RAYTRACING_H
#define RAYTRACING_H
#include "RayTriangleIntersection.h"
#include <ModelTriangle.h>
#include <Utils.h>
#include <glm/ext.hpp>
#include "camera.h"
#include <CanvasPoint.h>
#include "RedNoise.h"

RayTriangleIntersection getClosestValidIntersection(std::vector<ModelTriangle>& triangle, glm::vec3 position, glm::vec3 direction, int ignoreIndex, Camera& camera, bool& shadowCheck);

glm::vec3 directionVectorCalcs(Camera& camera);

glm::vec3 getPointInWorld(float u, float v, Camera& camera);

glm::vec3 getDirectionVector(glm::vec3 vertexPositionFrom, glm::vec3 vertexPositionTo);

glm::vec3 getTriangleIntersectionPointT(glm::vec3 cameraPosition, glm::vec3 rayDirection, float t);

glm::vec3 getTrianglePointUV(glm::vec3 startVertex, glm::vec3 edge0, glm::vec3 edge1, float u, float v);

glm::vec3 normalise(glm::vec3 vector);

void calcVertexNormalsPhong(std::vector<ModelTriangle>& triangles, RayTriangleIntersection intersectedTriangle);

void calcVertexNormals(std::vector<ModelTriangle>& triangles);

glm::vec3 calcAverageNormal(std::vector<ModelTriangle>& sharedTriangles);

#endif // !RAYTRACING_H
