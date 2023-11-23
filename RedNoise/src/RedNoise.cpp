#include "RedNoise.h"

void drawRayTracing(DrawingWindow& window, std::vector<ModelTriangle>& triangles, Camera& camera) {
	for (size_t y = 0; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH; x++) {
			glm::vec3 pointInSpace = getPointInWorld(x, y, camera);
			glm::vec3 directionVector = getDirectionVector(pointInSpace, camera.cameraPosition);
			RayTriangleIntersection closestValidIntersection = getClosestValidIntersection(triangles, camera.cameraPosition, directionVector, -1);

			if (closestValidIntersection.valid == true) {
				//glm::vec3 offsetPoint = 1.0f * 
				glm::vec3 directionVector2 = getDirectionVector(LIGHTSOURCE, closestValidIntersection.intersectionPoint);
				RayTriangleIntersection checkShadowIntersectionPoint = getClosestValidIntersection(triangles, closestValidIntersection.intersectionPoint, directionVector2, closestValidIntersection.triangleIndex);

				float pointToLight = glm::distance(closestValidIntersection.intersectionPoint, glm::vec3(LIGHTSOURCE));
				float distance2 = glm::distance(closestValidIntersection.intersectionPoint, checkShadowIntersectionPoint.intersectionPoint);
				
				Colour colour = closestValidIntersection.intersectedTriangle.colour;
				uint32_t colourUint32 = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
				if ((distance2 < pointToLight) && (distance2 > 0)) {
					window.setPixelColour(x, y, BLACK);
				}
				else {
					window.setPixelColour(x, y, colourUint32);
					//_sleep(50);
					//window.renderFrame();
				}
				//if no shadows then colour as regular colour if shadow then use black
			}



		}

	}
	
}

void draw(DrawingWindow& window, std::vector<ModelTriangle>& triangles, Camera& camera) {
	resetDepthBuffer(window);
	if (camera.lookAtToggle == true){
		camera.orbit(camera, camera.theta);
		camera.lookAt(camera, camera.lookAtPoint);
	}
	CanvasPoint imagePlanePoint;
	CanvasTriangle canvasPointTriangle;
	CanvasPoint scaledImagePlanePoint;
	float scalingFactor = 80;

	float maxZImagePlanePoint = -10;
	float minZImagePlanePoint = 10;

	for (size_t i = 0; i < triangles.size(); i++) {
		for (size_t j = 0; j < 3; j++) {
			glm::vec3 vertexPosition = triangles[i].vertices[j];
			
			imagePlanePoint = getCanvasIntersectionPoint(camera.cameraPosition, camera.cameraOrientation, vertexPosition, camera.focalLength);
			
			scaledImagePlanePoint.x = round(imagePlanePoint.x * scalingFactor) + (WIDTH / 2);
			scaledImagePlanePoint.y = round(imagePlanePoint.y * scalingFactor) + (HEIGHT / 2);

			scaledImagePlanePoint.depth = imagePlanePoint.depth;


			if (vertexPosition.z > maxZImagePlanePoint) {
				maxZImagePlanePoint = vertexPosition.z;
				
			}
			if (vertexPosition.z < minZImagePlanePoint) {
				minZImagePlanePoint = vertexPosition.z;
				
			}

			// std::cout << scaledImagePlanePoint.x << " " << scaledImagePlanePoint.y << " " << std::endl;
			canvasPointTriangle.vertices[j] = scaledImagePlanePoint;
		}
		
		//_sleep(800);

		if (triangles.size() == 1) {
			drawFilledTriangle(canvasPointTriangle, triangles[i].colour, window);
		}

		//drawStrokedTriangle(canvasPointTriangle, triangles[i].colour, window);
		drawStrokedTriangle(canvasPointTriangle, triangles[i].colour, window);
		/*if ((camera.cameraPosition.z > maxZImagePlanePoint)) {
			drawFilledTriangle(canvasPointTriangle, triangles[i].colour, window);
		}*/
		
		//uint32_t red = (255 << 24) + (255 << 16) + (0 << 8) + 0;
		//window.setPixelColour(WIDTH / 2, HEIGHT / 2, red);
		//window.renderFrame();
		
	}
	//draw is setting pixel colours
	// calling canvasintersectionpoint for each vertex of each triangle
	//resetDepthBuffer(window);
}
void drawRefLines(DrawingWindow& window, int mode) {
	drawLine(CanvasPoint(WIDTH / 2, 0), CanvasPoint(WIDTH / 2, 20), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(WIDTH / 2, HEIGHT), CanvasPoint(WIDTH / 2, HEIGHT-20), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(WIDTH / 4, 0), CanvasPoint(WIDTH / 4, 20), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(3*WIDTH / 4, 0), CanvasPoint(3*WIDTH / 4, 20), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(0, HEIGHT/4), CanvasPoint(20, (HEIGHT/4)), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(0, 3 * HEIGHT / 4), CanvasPoint(20,(3 * HEIGHT / 4)), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(0, HEIGHT/2), CanvasPoint(20, HEIGHT/2), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(WIDTH, HEIGHT/2), CanvasPoint(WIDTH-20,HEIGHT/2), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(WIDTH, HEIGHT / 4), CanvasPoint(WIDTH-20, (HEIGHT / 4)), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(WIDTH, 3 * HEIGHT / 4), CanvasPoint(WIDTH-20, (3 * HEIGHT / 4)), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(WIDTH/4, HEIGHT), CanvasPoint(WIDTH/4, HEIGHT-20), Colour(255, 0, 0), window);
	drawLine(CanvasPoint(3*WIDTH/4, HEIGHT), CanvasPoint(3*WIDTH/4, (HEIGHT-20)), Colour(255, 0, 0), window);

	if (mode == 1){
		drawLine(CanvasPoint(0, HEIGHT / 2), CanvasPoint(WIDTH, HEIGHT / 2), Colour(255, 0, 0), window);
		drawLine(CanvasPoint(0, HEIGHT / 4), CanvasPoint(WIDTH, HEIGHT/4), Colour(255, 0, 0), window);
		drawLine(CanvasPoint(0, 3*HEIGHT / 4), CanvasPoint(WIDTH, (3*HEIGHT / 4)), Colour(255, 0, 0), window);
		drawLine(CanvasPoint(WIDTH/2, 0), CanvasPoint(WIDTH /2, (HEIGHT)), Colour(255, 0, 0), window);
		drawLine(CanvasPoint(3*WIDTH / 4, 0), CanvasPoint(3*WIDTH / 4, HEIGHT), Colour(255, 0, 0), window);
		drawLine(CanvasPoint(WIDTH / 4, 0), CanvasPoint(WIDTH / 4, HEIGHT), Colour(255, 0, 0), window);
	}

}

void handleEvent(SDL_Event event, DrawingWindow& window, Camera& camera, int& renderType) {
	int mode;
	float theta = 0.5;
	float scalingFactor = 0.35;
	std::vector<ModelTriangle>& triangles = loadGeoOBJ(scalingFactor);
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
			std::vector<ModelTriangle>& triangles = loadGeoOBJ(1);
			std::vector<ModelTriangle> triangles2;
			triangles2.push_back(triangles[26]);
			//std::cout << "triangles size " << triangles.size() << std::endl;
			//for (size_t i = 0; i < triangles.size(); i++) { std::cout << "triangle: " << triangles[i] << " i: " << i << std::endl; }
			std::cout << "triangles2 " << triangles[26] << std::endl;

			
		}
		else if (event.key.keysym.sym == SDLK_r){
			std::cout << "pan in x" << std::endl;
			mode = 1;
			resetDepthBuffer(window);
			camera.rotate(camera, mode, theta);
			
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
			
		}
		else if (event.key.keysym.sym == SDLK_e) {
			std::cout << "rotate in y" << std::endl;
			mode = 4;
			resetDepthBuffer(window);
			camera.rotate(camera, mode, theta);
		}
		else if (event.key.keysym.sym == SDLK_l) {
			std::cout << "look at" << std::endl;
			if (camera.lookAtToggle == false) {
				camera.lookAtToggle = true;
			}
			else {
				camera.lookAtToggle = false;
			}
		}
		else if (event.key.keysym.sym == SDLK_k) {
			std::cout << glm::to_string(camera.cameraOrientation) << std::endl;

			camera.lookAt(camera, camera.lookAtPoint);

			std::cout << glm::to_string(camera.cameraOrientation) << std::endl;
		}
		else if (event.key.keysym.sym == SDLK_0) {
			std::cout << "ray tracing" << std::endl;
			glm::vec3 p0 = { 0.9,0.9,0 };
			glm::vec3 p1 = { 0.9, -0.9, 0 };
			glm::vec3 p2 = { -0.9, 0, 0 };
			std::vector<ModelTriangle>& triangles = loadGeoOBJ(1);
			ModelTriangle triangle;
			triangle.vertices = { p0, p1, p2 };

			std::vector<ModelTriangle> triangles2 = { triangle };
			
			//glm::vec3 direction = { 0.0,0.0,1 };
			drawRayTracing(window, triangles, camera);
			window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_m) {
			std::cout << "testing" << std::endl;
			//directionVectorCalcs(camera);
			float scalingFactor = 0.35;
			;
			drawRayTracing(window, triangles, camera);
			int mode = 1;
			//drawRefLines(window, mode);

		}
		else if (event.key.keysym.sym == SDLK_s) {
			std::cout << "wireframe" << std::endl;
			std::vector<ModelTriangle>& triangles = loadGeoOBJ(0.35);
			resetDepthBuffer(window);
			draw(window, triangles, camera);
		}
		else if (event.key.keysym.sym == SDLK_1) {
			std::cout << "wireframe" << std::endl;
			renderType = 1;
		}
		else if (event.key.keysym.sym == SDLK_2) {
			std::cout << "rasterised" << std::endl;
			renderType = 2;
		}
		else if (event.key.keysym.sym == SDLK_3) {
			std::cout << "raytrace" << std::endl;
			renderType = 3;
			
		}

	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}



int main(int argc, char* argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	srand((unsigned int)time(NULL));
	float scalingFactor = 0.35;
	Camera camera;
	int renderType = 1;
	//drawRayTracing(window, triangles, camera);
	std::vector<ModelTriangle>& triangles = loadGeoOBJ(scalingFactor);
	while (true) {
		if (window.pollForInputEvents(event)) handleEvent(event, window, camera, renderType);

		if (renderType == 1) {
			draw(window, triangles, camera);
		}
		else if (renderType == 2) {

		}
		else if (renderType == 3) {
			drawRayTracing(window, triangles, camera);
		}

		//draw(window, triangles, camera);
		//drawRayTracing(window, triangles, camera);
		window.renderFrame();
		//std::cout << "done";
	}
}
