#include "lightShade.h"



float proximityLightIntensity(float& pointToLight, float& intensity) {
	intensity = (4 / (5 * M_PI * pow(pointToLight, 2)));
	//glm::clamp(0.0f, 1.0f, intensity);
	if (intensity > 1) {
		intensity = 1;
	}
	return intensity;
}

float angleOfIncidence(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal) {
	float aoi = glm::dot(lightDirectionVector, triangleNormal);
	//float aoi = glm::dot(triangleNormal, lightDirectionVector);
	//if (aoi < 0) aoi = 1;
	return aoi;
}