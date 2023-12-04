#ifndef LIGHTSHADE_H
#define LIGHTSHADE_H

#include"RedNoise.h"


float proximityLightIntensity(float& pointToLight, float intensity);

float angleOfIncidence(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal);

float specularLighting(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal, glm::vec3 cameraToPoint);

#endif // !LIGHTSHADE_H
