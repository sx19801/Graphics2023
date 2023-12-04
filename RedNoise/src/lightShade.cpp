#include "lightShade.h"



float proximityLightIntensity(float& pointToLight, float intensity) {
	intensity = (5 / (4 * M_PI * pow(pointToLight, 2)));
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

float specularLighting(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal, glm::vec3 cameraToPoint) {
	float specular;
	int power = 256;
	glm::vec3 reflectionDirectionVector = (lightDirectionVector - 2 * triangleNormal * (dot(lightDirectionVector, triangleNormal)));
	specular = (pow(dot(cameraToPoint, reflectionDirectionVector), power));
	//if (specular > 0.5) std::cout << specular << '\n';
	//_sleep(50);

	return specular;
}