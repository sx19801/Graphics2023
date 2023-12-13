#ifndef LIGHTSHADE_H
#define LIGHTSHADE_H

#include"RedNoise.h"
#include"rayTracing.h"


float proximityLightIntensity(float& pointToLight);

float angleOfIncidence(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal);

float specularLighting(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal, glm::vec3 cameraToPoint);

std::array<glm::vec3, 3> gouraudShading(std::vector<ModelTriangle>& triangles, RayTriangleIntersection intersectedTriangle, Camera& camera);

glm::vec3 vectorOfReflection(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal);

#endif // !LIGHTSHADE_H
