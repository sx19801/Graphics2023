#include "lightShade.h"



float proximityLightIntensity(float& pointToLight) {
	float intensity = (1 / (5*pointToLight* pow(pointToLight, 2)));
	//std::cout << pointToLight << " ";
	//glm::clamp(0.0f, 1.0f, intensity);
	if (intensity > 1) {
		intensity = 1;
	}
	return intensity;
}

float angleOfIncidence(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal) {
	float aoi = glm::dot(lightDirectionVector, triangleNormal);
	//float aoi = glm::dot(triangleNormal, lightDirectionVector);
	//if (aoi < 0) std::cout << aoi << '\n';
	if (aoi < 0) aoi = 0;
	if (aoi > 1) aoi = 1;
	return aoi;
}

float specularLighting(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal, glm::vec3 cameraToPoint) {
	float specular;
	int power =32;
	glm::vec3 reflectionDirectionVector = lightDirectionVector - ((2 * triangleNormal) * (dot(lightDirectionVector, triangleNormal)));
	reflectionDirectionVector = normalise(reflectionDirectionVector);
	specular = pow(std::clamp(dot(cameraToPoint, reflectionDirectionVector), 0.0f,1.0f), power);
	//if (specular < 0) specular = 0;
	//specular = pow(specular, power);
	//_sleep(50);

	return specular;
}

glm::vec3 vectorOfReflection(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal) {
	glm::vec3 reflectionVector = lightDirectionVector - (2 * triangleNormal * dot(lightDirectionVector, triangleNormal));
	//reflectionVector = normalise(reflectionVector);
	return reflectionVector;
}

std::array<glm::vec3, 3> gouraudShading(std::vector<ModelTriangle>& triangles, RayTriangleIntersection intersectedTriangle, Camera& camera) {

	return intersectedTriangle.intersectedTriangle.vertexNormals;

}
	
void phongShading(std::vector<ModelTriangle>& triangles, RayTriangleIntersection intersectedTriangle, Camera& camera) {
	calcVertexNormalsPhong(triangles, intersectedTriangle);

	//barycentric stuff here

}
