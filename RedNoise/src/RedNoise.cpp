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


#define WIDTH 960
#define HEIGHT 720

//#define WIDTH 320
//#define HEIGHT 240

CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition, float focalLength) {
	//function to calculate 2D canvaspoint on image plane
	//std::cout << "this the vertex pos x: " << vertexPosition.x << " this the vertex pos y: " << vertexPosition.y << std::endl;



	float distZ = cameraPosition.z - vertexPosition.z;

	CanvasPoint imagePlanePoint;
	imagePlanePoint.x = (focalLength * (vertexPosition.x / distZ));
	imagePlanePoint.y = (focalLength * ((-vertexPosition.y) / distZ));
	
	return imagePlanePoint;
}

void handleEvent(SDL_Event event, DrawingWindow& window) {
	//CanvasTriangle triangle(CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height));
	CanvasTriangle triangle(CanvasPoint(200, 300), CanvasPoint(600, 100), CanvasPoint(500, 300));
	//CanvasTriangle triangle(CanvasPoint(50,10),CanvasPoint(20,16),CanvasPoint(80,16));
	Colour COLOUR = Colour(rand() % 255, rand() % 255, rand() % 255);
	Colour WHITE = Colour(255, 255, 255);
	float scalingFactor = 0.35;
	float secScalingFactorY = 120;
	float secScalingFactorX = 360;
	glm::vec3 cameraPosition = {0.0, 0.0, 4.0};
	glm::vec3 vertexPosition;
	glm::vec3 rescaleVertexPosition; 
	float focalLength = 5.0;
	std::vector<ModelTriangle> triangles3D;
	std::vector<CanvasTriangle> triangles2D;
	CanvasPoint imagePlanePoint;
	CanvasPoint imagePlanePointScaled;
	CanvasTriangle triangle2D;
	std::vector<CanvasPoint> imagePlanePoints;

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
		else if (event.key.keysym.sym == SDLK_u) {
			std::cout << "u" << std::endl;
			drawStrokedTriangle(window, triangle, COLOUR);
		}
		else if (event.key.keysym.sym == SDLK_f) {
			drawFilledTriangle(window, triangle, COLOUR);
		}
		else if (event.key.keysym.sym == SDLK_t) {

		}
		else if (event.key.keysym.sym == SDLK_l) {
			loadGeoOBJ(scalingFactor);
		}
		else if (event.key.keysym.sym == SDLK_m) {
			loadMatOBJ();
		}
		else if (event.key.keysym.sym == SDLK_r){
			loadMatOBJ();
			triangles3D = loadGeoOBJ(scalingFactor);
			
			//JUST FOR PRINTING      \/				\/				\/				\/
			//for (size_t i = 0; i < triangles.size(); i++) { std::cout << glm::to_string(triangles[i].vertices[0]) << " " << glm::to_string(triangles[i].vertices[1]) << " " << glm::to_string(triangles[i].vertices[2]) << " this is i also -> " << i << std::endl; }

			for (size_t i = 0; i < triangles3D.size(); i++) {
				for (size_t j = 0; j < 3; j++) {
					vertexPosition = triangles3D[i].vertices[j];
					imagePlanePoint = getCanvasIntersectionPoint(cameraPosition, vertexPosition, focalLength);
					//rescaleVertexPosition = {vertexPosition.x * secScalingFactorY, vertexPosition.y *secScalingFactorY, vertexPosition.z};
					imagePlanePointScaled.x = round((imagePlanePoint.x * secScalingFactorY)) + (WIDTH / 2);
					imagePlanePointScaled.y = round((imagePlanePoint.y * secScalingFactorY)) + (HEIGHT / 2);

					triangle2D.vertices[j] = imagePlanePointScaled;
					
					imagePlanePoints.push_back(imagePlanePointScaled);
					_sleep(100);
					std::cout << "image plane points: " << imagePlanePointScaled.x << " " << imagePlanePointScaled.y << std::endl;
					//draw(window, imagePlanePointScaled.x, imagePlanePointScaled.y);
					//drawStrokedTriangle(window, );
					//std::cout << "this is the image plane point x: " << imagePlanePoint.x << " this the y: " << imagePlanePoint.y << std::endl;
					//std::cout << "just checking this shit works" << i << " <- thats i " << j << " <- and thats j" << std::endl;
				}
				
				triangles2D.push_back(triangle2D);
				//drawStrokedTriangle(window, triangles2D[i], WHITE);
				drawFilledTriangle(window, triangles2D[i], triangles3D[i].colour);
				window.renderFrame();
			}

			//for (size_t i = 0; i < imagePlanePoints.size(); i++) { std::cout << imagePlanePoints[i] << std::endl; }
			/*for (size_t i = 0; i < imagePlanePoints.size(); i++) { std::cout << "image plane points: " << imagePlanePoints[i] << std::endl; }*/
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char* argv[]) {
	//Ensuring randomness
	srand((unsigned int)time(NULL));
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	std::vector<float> result;
	std::vector<glm::vec3> result2;
	glm::vec3 from(1.0, 4.0, 9.2);
	glm::vec3 to(4.0, 1.0, 9.8);

	Colour BLACK = Colour(0, 0, 0);

	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		//draw(window);
		//drawStrokedTriangle(window, triangle, BLACK);
		/*drawLine(window, CanvasPoint(0.0, 0.0), CanvasPoint(window.width / 2, window.height / 2), BLACK);
		drawLine(window, CanvasPoint(window.width/3,window.height/2), CanvasPoint(window.width*2 / 3, window.height/2), BLACK);
		drawLine(window, CanvasPoint(window.width/2,window.height/2), CanvasPoint(window.width, 0), BLACK);
		drawLine(window, CanvasPoint(window.width/2,0), CanvasPoint(window.width / 2, window.height), BLACK);*/
		//Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
