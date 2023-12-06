#include "lightShade.h"



float proximityLightIntensity(float& pointToLight, float intensity) {
	intensity = (3 / (4 * M_PI * pow(pointToLight, 2)));
	//glm::clamp(0.0f, 1.0f, intensity);
	if (intensity > 1) {
		intensity = 1;
	}
	return intensity;
}

float angleOfIncidence(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal) {
	float aoi = glm::dot(lightDirectionVector, triangleNormal);
	//float aoi = glm::dot(triangleNormal, lightDirectionVector);
	if (aoi < 0) aoi = 0;
	if (aoi > 1) aoi = 1;
	return aoi;
}

float specularLighting(glm::vec3 lightDirectionVector, glm::vec3 triangleNormal, glm::vec3 cameraToPoint) {
	float specular;
	int power = 128;
	glm::vec3 reflectionDirectionVector = (lightDirectionVector - 2 * triangleNormal * (dot(lightDirectionVector, triangleNormal)));
	specular = (pow(dot(cameraToPoint, reflectionDirectionVector), power));
	//if (specular > 0.5) std::cout << specular << '\n';
	//_sleep(50);

	return specular;
}

std::array<glm::vec3, 3> gouraudShading(std::vector<ModelTriangle>& triangles, RayTriangleIntersection intersectedTriangle, Camera& camera) {
	intersectedTriangle.intersectedTriangle.vertexNormals = calcVertexNormals(triangles, intersectedTriangle);
	//calc colour at each vertex
	return intersectedTriangle.intersectedTriangle.vertexNormals;

}
	
void phongShading(std::vector<ModelTriangle>& triangles, RayTriangleIntersection intersectedTriangle, Camera& camera) {
	intersectedTriangle.intersectedTriangle.vertexNormals = calcVertexNormals(triangles, intersectedTriangle);
	glm::vec3 edge0 = intersectedTriangle.intersectedTriangle.vertexNormals[1] - intersectedTriangle.intersectedTriangle.vertexNormals[0];
	glm::vec3 edge1 = intersectedTriangle.intersectedTriangle.vertexNormals[2] - intersectedTriangle.intersectedTriangle.vertexNormals[0];
	glm::vec3 normalOfPoint = getTrianglePointUV(intersectedTriangle.intersectedTriangle.vertexNormals[0], edge0, edge1, intersectedTriangle.u, intersectedTriangle.v);
	//barycentric stuff here

}
