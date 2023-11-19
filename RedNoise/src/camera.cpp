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
		camera.cameraOrientation = {
			1, 0, 0,
			0, cos(theta), sin(theta),
			0, -sin(theta), cos(theta),
		};
		camera.cameraPosition = camera.cameraOrientation * camera.cameraPosition;
		break;
	case 2:
		camera.cameraOrientation = {
			cos(theta), 0, -sin(theta),
			0, 1, 0,
			sin(theta), 0, cos(theta),
		};
		camera.cameraPosition = camera.cameraOrientation * camera.cameraPosition;
		break;
	
	}
	return camera;
}
