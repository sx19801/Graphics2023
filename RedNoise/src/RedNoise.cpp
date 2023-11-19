#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include "Triangle.h"
#include "camera.h"
#include "objFuncs.h"

#define WIDTH 480
#define HEIGHT 360

//#define WIDTH 960
//#define HEIGHT 720

void handleEvent(SDL_Event event, DrawingWindow& window, Camera& camera) {
	int mode;
	float theta = 0.5;
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) { 
			mode = 1;
			std::cout << "LEFT" << std::endl; 
			resetDepthBuffer(window);
			camera.translate(camera, mode);
			std::cout << glm::to_string(camera.cameraPosition) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_RIGHT) {
			std::cout << "RIGHT" << std::endl;
			mode = 2;
			resetDepthBuffer(window);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_UP) { 
			std::cout << "UP" << std::endl; 
			mode = 3;
			resetDepthBuffer(window);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_DOWN) {
			std::cout << "DOWN" << std::endl;
			mode = 4;
			resetDepthBuffer(window);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_z) {
			std::cout << "ZOOM" << std::endl;
			mode = 5;
			resetDepthBuffer(window);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_x) {
			std::cout << "ZOOM OUT" << std::endl;
			mode = 6;
			resetDepthBuffer(window);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_u) {
			std::cout << "U" << std::endl;
			CanvasTriangle triangle(CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height));
			drawStrokedTriangle(triangle, Colour(rand() % 255, rand() % 255, rand() % 255), window);
		}
		else if (event.key.keysym.sym == SDLK_f) {
			//CanvasTriangle triangle(CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height));
			//std::cout << triangle << '\n';
			//drawFilledTriangle(triangle, Colour(rand() % 255, rand() % 255, rand() % 255), window);
		}
		else if (event.key.keysym.sym == SDLK_r){
			std::cout << "pan in x" << std::endl;
			mode = 1;
			resetDepthBuffer(window);
			camera.rotate(camera, mode, theta);
			std::cout << glm::to_string(camera.cameraPosition) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_t) {
			std::cout << "pan in y" << std::endl;
			mode = 2;
			resetDepthBuffer(window);
			camera.rotate(camera, mode, theta);
		}
		else if (event.key.keysym.sym == SDLK_q) {
			std::cout << "rotate in x" << std::endl;
			mode = 3;
			resetDepthBuffer(window);
			camera.rotate(camera, mode, theta);
			//std::cout << glm::to_string(camera.cameraPosition) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_e) {
			std::cout << "rotate in y" << std::endl;
			mode = 4;
			resetDepthBuffer(window);
			camera.rotate(camera, mode, theta);
		}
		else if (event.key.keysym.sym == SDLK_l) {
			std::cout << "look at" << std::endl;
			//camera.lookAt(camera, camera.lookAtPoint);
			glm::vec3 lookAtPoint = { 0.0, 0.0, 0.0 };
			glm::vec3 right;
			glm::vec3 up;
			glm::vec3 vertical = { 0.0, -1.0, 0.0 };

			right = glm::cross(vertical, lookAtPoint);

			up = glm::cross(lookAtPoint, right);

			glm::mat3 LOOKATME = {
				right,
				up,
				lookAtPoint,
			};

			std::cout << glm::to_string(right) << " " << glm::to_string(up) << " " << glm::to_string(lookAtPoint) <<  std::endl;
		}
		else if (event.key.keysym.sym == SDLK_k) {
			std::cout << glm::to_string(camera.cameraOrientation) << std::endl;

			camera.lookAt(camera, camera.lookAtPoint);

			std::cout << glm::to_string(camera.cameraOrientation) << std::endl;
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

void draw(DrawingWindow& window, std::vector<ModelTriangle> triangles, Camera& camera) {
	resetDepthBuffer(window);
	camera.orbit(camera, camera.theta);
	camera.lookAt(camera, camera.lookAtPoint);
	CanvasPoint imagePlanePoint;
	CanvasTriangle canvasPointTriangle;
	CanvasPoint scaledImagePlanePoint;
	float scalingFactor = 80;

	float maxZImagePlanePoint = -10;
	float minZImagePlanePoint = 10;

	for (size_t i = 0; i < triangles.size(); i++) {
		for (size_t j = 0; j < 3; j++) {
			glm::vec3 vertexPosition = triangles[i].vertices[j];
			//std::cout << "vertexposition not scaled: " << glm::to_string(triangles[i].vertices[j]) << std::endl;
			imagePlanePoint = getCanvasIntersectionPoint(camera.cameraPosition, camera.cameraOrientation, vertexPosition, camera.focalLength);
			//std::cout << imagePlanePoint << std::endl;
			scaledImagePlanePoint.x = round(imagePlanePoint.x * scalingFactor) + (WIDTH / 2);
			scaledImagePlanePoint.y = round(imagePlanePoint.y * scalingFactor) + (HEIGHT / 2);

			scaledImagePlanePoint.depth = imagePlanePoint.depth;


			if (vertexPosition.z > maxZImagePlanePoint) {
				maxZImagePlanePoint = vertexPosition.z;
				//	std::cout << "max z: " << maxZImagePlanePoint << std::endl;
			}
			if (vertexPosition.z < minZImagePlanePoint) {
				minZImagePlanePoint = vertexPosition.z;
				//std::cout << "min z: " << minZImagePlanePoint << std::endl;
			}

			//std::cout << scaledImagePlanePoint.depth << std::endl;
			//std::cout << scaledImagePlanePoint.x << "  " << scaledImagePlanePoint.y << "  " << std::endl;

			canvasPointTriangle.vertices[j] = scaledImagePlanePoint;
		}
		
		
		if ((camera.cameraPosition.z > maxZImagePlanePoint)) {
			drawFilledTriangle(canvasPointTriangle, triangles[i].colour, window);
		}
		
			//window.renderFrame();
		
	}
	//draw is setting pixel colours
	// calling canvasintersectionpoint for each vertex of each triangle
	//resetDepthBuffer(window);
}

//float zBuffer[WIDTH][HEIGHT] = {};
int main(int argc, char* argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	srand((unsigned int)time(NULL));
	float scalingFactor = 0.35;
	Camera camera;

	std::vector<ModelTriangle> triangles = loadGeoOBJ(scalingFactor);
	while (true) {
		//window.clearPixels();
		// We MUST poll for events - otherwise the window will freeze !
		//draw(window, triangles, camera);
		if (window.pollForInputEvents(event)) handleEvent(event, window, camera);
		draw(window, triangles, camera);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
