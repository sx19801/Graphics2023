#ifndef LIGHTSHADE_H
#define LIGHTSHADE_H

#include"RedNoise.h"


float proximityLightIntensity(float& pointToLight, float& intensity);

float angleOfIncidence(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal);

#endif // !LIGHTSHADE_H
