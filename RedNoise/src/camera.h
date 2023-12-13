#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>


class Camera
{
public:
//	glm::vec3 cameraPosition = {-2.721383, 0.000000, 2.931849};
	glm::vec3 cameraPosition = { 0.0, -0.5, 2.5 };
//	glm::vec3 cameraPosition = { -3.0, 0, -2.5 };
//	glm::vec3 cameraPosition = {-3.973935, 0.000000, 0.457460 };
	//glm::vec3 cameraPosition = { -0.1, 0.5, 1.6 };//sphere

	glm::vec3 lookAtPoint = { 0.0, 0.0, 0.0 };
	//glm::mat3 cameraOrientation = { 0.732924, -0.000000, 0.680310, 0.000000, 1.000000, 0.000000, -0.680310, -0.000000, 0.732924 };
	
	//glm::mat3 cameraOrientation = { 0.114360, -0.000000, 0.993439,0.000000, 1.000000, 0.000000, -0.993439, -0.000000, 0.114360 };
	glm::mat3 cameraOrientation;
	float focalLength = 2.0;
	float theta = 0.003;
//	glm::vec3 lightSource = { -0.3, 0.6, 1.1 };//specular lighting testing sphere
//	glm::vec3 lightSource = { 0.1, 0.0, 0.0 };
//	glm::vec3 lightSource = { 0.2, 1.1, 0.7 };
//	glm::vec3 lightSource = { 0.0,0.4,0.7 };
	
	glm::vec3 lightSource = { 0.7,0.7,-0.7 };
	
	//glm::vec3 lightSource = { 0.5,0.6,-0.5 };

	
	bool lookAtToggle = false;
	float scalingFactor = 160;

	//BEZIER
	glm::vec3 p0 = {-3.0, -1.0, 2.5};
	glm::vec3 p1 = { -1.5, -0.33, 0 };
	glm::vec3 p2 = { 1.5,0.33, 4.5};
	glm::vec3 p3 = { 3.0, 1.0, 2.5 };
	float t = 0;
	float speed = 0.01;

	//MULTI LIGHTSOURCES
	float radius = 0.25;
	glm::vec3 lightOrigin = { 0.7,0.7,-0.7 };


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

	glm::vec3 getPointBezier(Camera& camera);

	void moveAlongBezier(Camera& camera, glm::vec3& newPosition);

	void update(Camera& camera);

	glm::vec3 getLightSource(glm::vec3& lightOrigin, float& radius);
};


#endif // !CAMERA_H
