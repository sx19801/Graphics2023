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

	//IDK WHY BUT IT WORKS
	 camera.cameraOrientation = {
		-right,
		up,
		-forward,
	};

	// camera.lookAtToggle = false;
}

glm::vec3 Camera::getPointBezier(Camera& camera) {
	float t = camera.t;
	
	float u = 1 - t;
	float tt = t * t;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * t;

	glm::vec3 point = (camera.p0 * uuu);		// First term
	point += (camera.p1 * 3.0f * uu * t);		// Second term
	point += (camera.p2 * 3.0f * u * tt);		// Third term
	point += (camera.p3 * ttt);					// Fourth term

	return point;
}

glm::vec3 Camera::getPointBezierLight(Camera& camera) {
	float tLight = camera.tLight;
	
	float uL = 1 - tLight;
	float ttL = tLight * tLight;
	float uuL = uL * uL;
	float uuuL = uuL * uL;
	float tttL = ttL * tLight;

	glm::vec3 point = (camera.pLight0 * uuuL);		// First term
	point += (camera.pLight1 * 3.0f * uuL * tLight);		// Second term
	point += (camera.pLight2 * 3.0f * uL * ttL);		// Third term
	point += (camera.pLight3 * tttL);					// Fourth term

	return point;
}


void Camera::moveAlongBezier(Camera& camera, glm::vec3& newPosition) {
	camera.cameraPosition = newPosition;					//for camera movement
	//camera.lookAtPoint = newPosition;						//for lookatpoint movement


	camera.lookAt(camera, camera.lookAtPoint);
}

void Camera::moveAlongBezierLight(Camera& camera, glm::vec3& newPosition) {
	//camera.cameraPosition = newPosition;					//for camera movement
	camera.lightSource = newPosition;						//for lookatpoint movement
	//camera.lightOrigin = newPosition;

	//camera.lookAt(camera, camera.lookAtPoint);
}

void Camera::update(Camera& camera) {
	camera.t += camera.speed;
	if (camera.t > 1.0) camera.t = 1.0;
	glm::vec3 newPosition = camera.getPointBezier(camera);
	camera.moveAlongBezier(camera, newPosition);
}

void Camera::updateLight(Camera& camera) {
	camera.tLight += camera.speedLight;
	if (camera.tLight > 1.0) camera.tLight = 1.0;
	glm::vec3 newPosition = camera.getPointBezierLight(camera);
	camera.moveAlongBezierLight(camera, newPosition);
}



glm::vec3 Camera::getLightSource(glm::vec3& lightOrigin, float& radius) {
	float d;
	double x, y, z;
	do {
	x = ((double)rand() / RAND_MAX) * radius * 2.0 - radius + lightOrigin.x;
	y = ((double)rand() / RAND_MAX) * radius * 2.0 - radius + lightOrigin.y;
	z = ((double)rand() / RAND_MAX) * radius * 2.0 - radius + lightOrigin.z;
	d = (x - lightOrigin.x) * (x - lightOrigin.x) + (y - lightOrigin.y) * (y- lightOrigin.y) + (z- lightOrigin.z) * (z - lightOrigin.z);
	} while (d > radius*radius);
	return { x, y, z };	
}