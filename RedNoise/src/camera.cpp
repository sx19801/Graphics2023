#include "camera.h"

Camera Camera::translate(Camera& camera, int& mode) {

	switch (mode)
	{
	case 1:
		camera.cameraPosition.x = camera.cameraPosition.x - 0.5;
		break;
	case 2:
		camera.cameraPosition.x = camera.cameraPosition.x + 0.5;
		break;
	case 3:
		camera.cameraPosition.y = camera.cameraPosition.y + 0.5;
		break;
	case 4:
		camera.cameraPosition.y = camera.cameraPosition.y - 0.5;
		break;
	case 5:
		camera.cameraPosition.z = camera.cameraPosition.z - 0.5;
		break;
	case 6:
		camera.cameraPosition.z = camera.cameraPosition.z + 0.5;
	}	

	return camera;
}

Camera Camera::rotate(Camera& camera, int& mode, float theta) {
	switch (mode)
	{
	case 1:
		camera.cameraOrientation = camera.rotationX * camera.cameraOrientation;
		break;
	case 2:
		camera.cameraOrientation = camera.rotationY * camera.cameraOrientation;
		break;
	case 3:
		camera.cameraPosition = camera.rotationX * camera.cameraPosition;
		break;
	case 4:
		camera.cameraPosition = camera.rotationY * camera.cameraPosition;
		break;

	}
	return camera;
}

Camera Camera::orbit(Camera& camera, float theta) {
	camera.cameraPosition = camera.rotationY * camera.cameraPosition;
	//camera.lookAt(camera, camera.lookAtPoint);
	return camera;
}

void Camera::lookAt(Camera& camera, glm::vec3 lookAtPoint) {
	
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 vertical = { 0.0, 1.0, 0.0 };

	glm::vec3 forward = lookAtPoint - camera.cameraPosition;

	//need to normalise forward 
	float len = sqrt((forward.x * forward.x) + (forward.y * forward.y) + (forward.z * forward.z));
	forward.x /= len;
	forward.y /= len;
	forward.z /= len;

	right = glm::cross(vertical, forward);
	up = glm::cross(forward, right);

	 camera.cameraOrientation = {
		right,
		-up,
		forward,
	};

}
