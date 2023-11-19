#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

class Camera
{
public:
	glm::vec3 cameraPosition = { 0.0, 0.0, 4.0 };
	glm::mat3 cameraOrientation;
	float focalLength = 2.0;

	Camera translate(Camera& camera, int& mode);

	Camera rotate(Camera& camera, int& mode, float theta);

};

#endif // !CAMERA_H
