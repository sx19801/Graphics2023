#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

class Camera
{
public:
//	glm::vec3 cameraPosition = {-2.721383, 0.000000, 2.931849};
	glm::vec3 cameraPosition = { 0.0, 0.0, 4.0 };
	//glm::vec3 cameraPosition = { -0.1, 0.5, 1.6 };//sphere

	glm::vec3 lookAtPoint = { 0.0, 0.0, 0.0 };
	//glm::mat3 cameraOrientation = { 0.732924, -0.000000, 0.680310, 0.000000, 1.000000, 0.000000, -0.680310, -0.000000, 0.732924 };
	glm::mat3 cameraOrientation;
	float focalLength = 2.0;
	float theta = 0.003;
//	glm::vec3 lightSource = { -0.3, 0.6, 1.1 };//specular lighting testing sphere
//	glm::vec3 lightSource = { 0.0, 0.0, 0.0 };
//	glm::vec3 lightSource = { 0.2, 1.1, 0.7 };
//	glm::vec3 lightSource = { 0.0,0.4,0.7 };
//	glm::vec3 lightSource = { 0.0,0.5,0.6 };
	glm::vec3 lightSource = { 0.5,0.6,-0.5 };
	bool lookAtToggle = false;
	float scalingFactor = 160;
	glm::mat3 rotationX = {
		1.0, 0.0, 0.0,
		0.0, cos(theta), sin(theta),
		0.0, -sin(theta), cos(theta),
	};

	glm::mat3 rotationY = {
		cos(theta), 0.0, -sin(theta),
		0.0, 1.0, 0.0,
		sin(theta), 0.0, cos(theta),
	};

	Camera translate(Camera& camera, int& mode);

	Camera rotate(Camera& camera, int& mode, float theta);

	Camera orbit(Camera& camera, float theta);

	void lookAt(Camera& camera, glm::vec3 lookAtPoint);
};


#endif // !CAMERA_H
