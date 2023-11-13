#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <CanvasPoint.h>
#include <Colour.h>
#include <CanvasTriangle.h>
#include <ModelTriangle.h>
#include <map>
#include"objFuncs.h"
#include"drawFuncs.h"


#define WIDTH 320
#define HEIGHT 240

//#define WIDTH 960
//#define HEIGHT 720


glm::vec3 cameraPosition = { 0.0, 0.0, 4.0 };
float normalisingScalingFactor = 0.35;
float secScalingFactorY = 60;
float secScalingFactorX = 360;
float focalLength = 2.0;
CanvasPoint resetFrom(0, 0, 10);
CanvasPoint resetTo(WIDTH - 1, 0, 10);
Colour COLOUR = Colour(rand() % 255, rand() % 255, rand() % 255);
Colour WHITE = Colour(255, 255, 255);
Colour BLACK(0, 0, 0);
glm::mat3 cameraRotation;

CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition, float focalLength) {
	//function to calculate 2D canvaspoint on image plane
	//std::cout << "this the vertex pos x: " << vertexPosition.x << " this the vertex pos y: " << vertexPosition.y << std::endl;

	float distZ = cameraPosition.z - vertexPosition.z;
	glm::vec3 distance = cameraPosition - vertexPosition;


	CanvasPoint imagePlanePoint;
	imagePlanePoint.x = (focalLength * (-distance.x) / distZ);
	imagePlanePoint.y = (focalLength * (distance.y / distZ));

	//assigning depth from z
	imagePlanePoint.depth = distZ;
	imagePlanePoint.x = imagePlanePoint.x;
	return imagePlanePoint;
}

void intersectionCalcsAndDrawing(glm::vec3 cameraPosition, std::vector<ModelTriangle> triangles3D, float focalLength, float secScalingFactorY, DrawingWindow& window) {
	CanvasPoint imagePlanePointScaled;
	CanvasTriangle triangle2D;
	std::vector<CanvasPoint> imagePlanePoints;
	std::vector<CanvasTriangle> triangles2D;
	for (size_t i = 0; i < triangles3D.size(); i++) {
		for (size_t j = 0; j < 3; j++) {
			glm::vec3 vertexPosition = triangles3D[i].vertices[j];

			CanvasPoint imagePlanePoint = getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);

			//these the ones i need to be checking
			imagePlanePointScaled.x = round((imagePlanePoint.x * secScalingFactorY)) + (WIDTH / 2);
			imagePlanePointScaled.y = round((imagePlanePoint.y * secScalingFactorY)) + (HEIGHT / 2);

			//+1 since some of the z values are negative so make all positive (instead of from -1 to 1 the values are now 0 to 2)
			imagePlanePointScaled.depth = imagePlanePoint.depth;

			triangle2D.vertices[j] = imagePlanePointScaled;

			imagePlanePoints.push_back(imagePlanePointScaled);
		}

		triangles2D.push_back(triangle2D);
		//drawStrokedTriangle(window, triangles2D[i], WHITE);
		//_sleep(80);
		drawFilledTriangle(window, triangles2D[i], triangles3D[i].colour);
		
	}
	window.renderFrame();
}

void resetZDepthBuffer(CanvasPoint resetFrom, CanvasPoint resetTo, Colour BLACK, bool reset, DrawingWindow& window) {
	for (size_t j = 0; j < HEIGHT; j++) {
		resetFrom.y = j;
		resetTo.y = j;
		drawLine(window, resetFrom, resetTo, BLACK, reset);
	}
	reset = false;
}

void handleEvent(SDL_Event event, DrawingWindow& window) {
	CanvasTriangle triangle(CanvasPoint(rand() % window.width, rand() % window.height, 0.5), CanvasPoint(rand() % window.width, rand() % window.height, 0.2), CanvasPoint(rand() % window.width, rand() % window.height, 0.7));
	//CanvasTriangle triangle(CanvasPoint(200, 300), CanvasPoint(600, 100), CanvasPoint(500, 300));
	//CanvasTriangle triangle(CanvasPoint(50,10),CanvasPoint(20,16),CanvasPoint(80,16));
	glm::vec3 vertexPosition;
	glm::vec3 rescaleVertexPosition; 
	std::vector<ModelTriangle> triangles3D;
	std::vector<CanvasTriangle> triangles2D;
	CanvasPoint imagePlanePoint;
	CanvasPoint imagePlanePointScaled;
	CanvasTriangle triangle2D;
	std::vector<CanvasPoint> imagePlanePoints;
	cameraRotation;
	float degrees = 15;
	float theta = degrees * (3.14159 / 180);

	triangles3D = loadGeoOBJ(normalisingScalingFactor);

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_RIGHT) {
			std::cout << "RIGHT" << std::endl;
			bool reset = true;
			resetZDepthBuffer(resetFrom, resetTo, BLACK, reset, window);

			cameraPosition.x = cameraPosition.x + 0.5;

			//getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);
			intersectionCalcsAndDrawing(cameraPosition, triangles3D, 2.0, secScalingFactorY, window);

			window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_LEFT) {
			std::cout << "LEFT" << std::endl;

			bool reset = true;
			resetZDepthBuffer(resetFrom, resetTo, BLACK, reset, window);

			cameraPosition.x = cameraPosition.x - 0.5;
			
			//getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);
			intersectionCalcsAndDrawing(cameraPosition, triangles3D, 2.0, secScalingFactorY, window);

			window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_DOWN) {
			std::cout << "DOWN" << std::endl;

			bool reset = true;
			resetZDepthBuffer(resetFrom, resetTo, BLACK, reset, window);

			cameraPosition.y = cameraPosition.y - 0.5;

			//getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);
			intersectionCalcsAndDrawing(cameraPosition, triangles3D, 2.0, secScalingFactorY, window);

			window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_UP) {
			std::cout << "UP" << std::endl;
			bool reset = true;
			resetZDepthBuffer(resetFrom, resetTo, BLACK, reset, window);

			cameraPosition.y = cameraPosition.y + 0.5;

			//getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);
			intersectionCalcsAndDrawing(cameraPosition, triangles3D, 2.0, secScalingFactorY, window);

			window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_z) {
			std::cout << "ZOOM IN" << std::endl;
			bool reset = true;
			resetZDepthBuffer(resetFrom, resetTo, BLACK, reset, window);

			cameraPosition.z = cameraPosition.z - 0.5;

			//getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);
			intersectionCalcsAndDrawing(cameraPosition, triangles3D, 2.0, secScalingFactorY, window);

			window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_x) {
			std::cout << "ZOOM OUT" << std::endl;
			bool reset = true;
			resetZDepthBuffer(resetFrom, resetTo, BLACK, reset, window);

			cameraPosition.z = cameraPosition.z + 0.5;
	
			//getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);
			intersectionCalcsAndDrawing(cameraPosition, triangles3D, 2.0, secScalingFactorY, window);

			window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_q) {
			cameraRotation = glm::mat3(
				1.0, 0.0, 0.0,
				0.0, cos(theta), sin(theta),
				0.0, -sin(theta), cos(theta)
			);

			bool reset = true;
			resetZDepthBuffer(resetFrom, resetTo, BLACK, reset, window);
			cameraPosition = cameraPosition * cameraRotation;
			
			std::cout << glm::to_string(cameraPosition);

			//getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);
			intersectionCalcsAndDrawing(cameraPosition, triangles3D, 2.0, secScalingFactorY, window);
			window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_e) {
			glm::mat3 cameraRotation2 = glm::mat3(
				cos(theta), 0.0, -sin(theta),
				0.0, 1.0, 0.0,
				sin(theta), 0.0, cos(theta)
			);

			bool reset = true;
			resetZDepthBuffer(resetFrom, resetTo, BLACK, reset, window);

			cameraPosition = cameraPosition * cameraRotation2;
			
			//getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);
			intersectionCalcsAndDrawing(cameraPosition, triangles3D, 2.0, secScalingFactorY, window);
		}
		else if (event.key.keysym.sym == SDLK_l) {
			loadGeoOBJ(normalisingScalingFactor);
		}
		else if (event.key.keysym.sym == SDLK_t) {
		}
		else if (event.key.keysym.sym == SDLK_m) {
		}
		else if (event.key.keysym.sym == SDLK_r){
			std::cout << "r" << std::endl;
			
			intersectionCalcsAndDrawing(cameraPosition, triangles3D, 2.0, secScalingFactorY, window);
		}
		else if (event.key.keysym.sym == SDLK_c) {
			std::cout << "c" << std::endl;

			bool reset = true;
			resetZDepthBuffer(resetFrom, resetTo, BLACK, reset, window);
			cameraPosition = {0.0, 0.0, 4.0};

		}
		else if (event.key.keysym.sym == SDLK_u) {
			std::cout << "u" << std::endl;
			drawStrokedTriangle(window, triangle, COLOUR);
		}
		else if (event.key.keysym.sym == SDLK_f) {
			std::cout << "f" << std::endl;
			drawFilledTriangle(window, triangle, COLOUR);
		}
		std::cout << glm::to_string(cameraPosition);
}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char* argv[]) {
	//Ensuring randomness
	srand((unsigned int)time(NULL));
	loadMatOBJ();
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	std::vector<float> result;
	std::vector<glm::vec3> result2;
	glm::vec3 from(1.0, 4.0, 9.2);
	glm::vec3 to(4.0, 1.0, 9.8);
	Colour BLACK = Colour(0, 0, 0);

	//std::cout << "hello.   make a triangle with 'u'.   make a 3 dimensional filled triangle with 'f'.    " << std::endl << std::endl << "make two boxes in a room with 'r'.   use 'c' to clear the window.   " << std::endl << std::endl;
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		//draw(window);
		//Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
