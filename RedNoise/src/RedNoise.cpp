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
	float theta = 0.1;
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) { 
			mode = 1;
			std::cout << "LEFT" << std::endl; 
			resetDepthBuffer(window);
			camera.translate(camera, mode);
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
		else if (event.key.keysym.sym == SDLK_x){
			std::cout << "rotate in x" << std::endl;
			mode = 1;
			resetDepthBuffer(window);
			camera.rotate(camera, mode, theta);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

void draw(DrawingWindow& window, std::vector<ModelTriangle> triangles, Camera& camera) {
	CanvasPoint imagePlanePoint;
	CanvasTriangle canvasPointTriangle;
	CanvasPoint scaledImagePlanePoint;
	float scalingFactor = 180;


	for (size_t i = 0; i < triangles.size(); i++) {
		for (size_t j = 0; j < 3; j++) {
			glm::vec3 vertexPosition = triangles[i].vertices[j];
			//std::cout << "vertexposition not scaled: " << glm::to_string(triangles[i].vertices[j]) << std::endl;
			imagePlanePoint = getCanvasIntersectionPoint(camera.cameraPosition, vertexPosition, camera.focalLength);
			//std::cout << imagePlanePoint << std::endl;
			scaledImagePlanePoint.x = round(imagePlanePoint.x * scalingFactor) + (WIDTH / 2);
			scaledImagePlanePoint.y = round(imagePlanePoint.y * scalingFactor) + (HEIGHT / 2);

			scaledImagePlanePoint.depth = imagePlanePoint.depth;

			//std::cout << scaledImagePlanePoint.x << "  " << scaledImagePlanePoint.y << "  " << std::endl;

			canvasPointTriangle.vertices[j] = scaledImagePlanePoint;
		}
		//_sleep(500);
		//std::cout << "canvas point triangle " << canvasPointTriangle << std::endl;
		//drawStrokedTriangle(canvasPointTriangle, triangles[i].colour, window);
		drawFilledTriangle(canvasPointTriangle, triangles[i].colour, window);
		//window.renderFrame();
	}
	//draw is setting pixel colours
	// calling canvasintersectionpoint for each vertex of each triangle
	//window.clearPixels();
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
		// We MUST poll for events - otherwise the window will freeze !
		//draw(window, triangles, camera);
		if (window.pollForInputEvents(event)) handleEvent(event, window, camera);
		draw(window, triangles, camera);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
