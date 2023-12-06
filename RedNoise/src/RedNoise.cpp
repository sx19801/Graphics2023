#include "RedNoise.h"



void drawRayTracing(DrawingWindow& window, std::vector<ModelTriangle>& triangles, Camera& camera, int& lightType) {
	Colour colour;
	float pointToLightDist;
	float distance2;
	float intensity = 1;
	float aoi1 = 1;
	float aoi2 = 1;
	float aoi3 = 1;

	float combine = 1;
	float specular = 0;
	bool shadowCheck = false;
	std::array<glm::vec3, 3> vertexNormals;
	uint32_t colourUint32 = colourUint32 = (255 << 24) + (int(round(colour.red * combine)) << 16) + (int(round(colour.green * combine)) << 8) + int(round(colour.blue * combine));
	glm::vec3 pointInSpace;
	glm::vec3 directionVector;
	RayTriangleIntersection closestValidIntersection;
	glm::vec3 lightDirectionVector;
	glm::vec3 lightDirectionVector1;
	glm::vec3 lightDirectionVector2;
	glm::vec3 lightDirectionVector3;
	RayTriangleIntersection checkShadowIntersectionPoint;

	for (size_t y = 0; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH; x++) {
			pointInSpace = getPointInWorld(x, y, camera);
			directionVector = getDirectionVector(pointInSpace, camera.cameraPosition);
			closestValidIntersection = getClosestValidIntersection(triangles, camera.cameraPosition, directionVector, -1, camera, shadowCheck);

			if (closestValidIntersection.valid == true) {

				colour = closestValidIntersection.intersectedTriangle.colour;
				closestValidIntersection.shadowCheck = true;
				lightDirectionVector = getDirectionVector(camera.lightSource, closestValidIntersection.intersectionPoint);
				checkShadowIntersectionPoint = getClosestValidIntersection(triangles, closestValidIntersection.intersectionPoint, lightDirectionVector, closestValidIntersection.triangleIndex, camera, closestValidIntersection.shadowCheck);

				pointToLightDist = glm::distance(closestValidIntersection.intersectionPoint, camera.lightSource);
				distance2 = glm::distance(closestValidIntersection.intersectionPoint, checkShadowIntersectionPoint.intersectionPoint);
			
				switch (lightType)
				{
				case 1:
					intensity = proximityLightIntensity(pointToLightDist, intensity);
					colourUint32 = (255 << 24) + (int(round(colour.red * intensity)) << 16) + (int(round(colour.green * intensity)) << 8) + int(round(colour.blue * intensity));
					break;

				case 2:
					aoi1 = angleOfIncidence(lightDirectionVector, closestValidIntersection.intersectedTriangle.normal);
					if (aoi1 < 0.1) aoi1 = 0.1;
					colourUint32 = (255 << 24) + (int(round(colour.red * aoi1)) << 16) + (int(round(colour.green * aoi1)) << 8) + int(round(colour.blue * aoi1));
					break;

				case 3:	
					specular = specularLighting(lightDirectionVector, closestValidIntersection.intersectedTriangle.normal, directionVector);
					colourUint32 = (255 << 24) + (int(round(255 * specular)) << 16) + (int(round(255 * specular)) << 8) + int(round(255 * specular));
					break;

				case 4:
					intensity = proximityLightIntensity(pointToLightDist, intensity);
					aoi1 = angleOfIncidence(lightDirectionVector, closestValidIntersection.intersectedTriangle.normal);
					specular = specularLighting(lightDirectionVector, closestValidIntersection.intersectedTriangle.normal, directionVector);
					if (aoi1 > 1 ) aoi1 = 1;
					if (aoi1 < 0.1) aoi1 = 0.1;
					if (specular > 0.5) std::cout << specular << '\n';
					specular = 255 * specular;
					combine = (1.7*intensity*(aoi1))+0.2;
					if (combine > 1) combine = 1;
					if (combine < 0) combine = 0;
					colourUint32 = (255 << 24) + (int(std::clamp(round(colour.red * combine + specular),0.0f,255.0f)) << 16) + (int(std::clamp(round(colour.green * combine + specular), 0.0f, 255.0f)) << 8) + std::clamp(round(colour.blue * combine + specular), 0.0f, 255.0f);
					break;

				case 5: //GOURAUD
					vertexNormals = gouraudShading(triangles, closestValidIntersection, camera);
					lightDirectionVector1 = getDirectionVector(camera.lightSource, closestValidIntersection.intersectedTriangle.vertices[0]);
					lightDirectionVector2 = getDirectionVector(camera.lightSource, closestValidIntersection.intersectedTriangle.vertices[1]);
					lightDirectionVector3 = getDirectionVector(camera.lightSource, closestValidIntersection.intersectedTriangle.vertices[2]);
					aoi1 = angleOfIncidence(lightDirectionVector1, vertexNormals[0]);
					aoi2 = angleOfIncidence(lightDirectionVector2, vertexNormals[1]);
					aoi3 = angleOfIncidence(lightDirectionVector3, vertexNormals[2]);
					glm::vec3 colour1 = { int(round(colour.red * aoi1)),int(round(colour.green * aoi1)),int(round(colour.blue * aoi1)) };
					glm::vec3 colour2 = { int(round(colour.red * aoi2)),int(round(colour.green * aoi2)),int(round(colour.blue * aoi2)) };
					glm::vec3 colour3 = { int(round(colour.red * aoi3)),int(round(colour.green * aoi3)),int(round(colour.blue * aoi3)) };
					
					glm::vec3 edge0 = colour2 - colour1;
					glm::vec3 edge1 = colour3 - colour1;
					glm::vec3 colourOfPoint = getTrianglePointUV(colour1, edge0, edge1, closestValidIntersection.u, closestValidIntersection.v);
					
					colourUint32 = (255 << 24) + (int(round(colourOfPoint.r)) << 16) + (int(round(colourOfPoint.g * combine)) << 8) + int(round(colourOfPoint.b * combine));

					break;
				}
			

				if ((distance2 < pointToLightDist) && (distance2 > 0) && (lightType == 4)) { //SHADOWS
					intensity = proximityLightIntensity(pointToLightDist, intensity);
					aoi1 = angleOfIncidence(lightDirectionVector, closestValidIntersection.intersectedTriangle.normal);
					if (aoi1 > 1) aoi1 = 1;
					if (aoi1 < 0.1) aoi1 = 0.1;
					combine = 1.9*(intensity/2*(aoi1))+0.15;
					if (combine < 0.03) combine = 0.03;
					if (combine > 1) combine = 1;

					colourUint32 = (255 << 24) + (int(round(colour.red * combine)) << 16) + (int(round(colour.green * combine)) << 8) + int(round(colour.blue * combine));
				}
				
				window.setPixelColour(x, y, colourUint32);
			}
			else { //NO VALID INTERSECTIONS 
				window.setPixelColour(x, y, BLACK);
			}
		}
	}
}


void draw(DrawingWindow& window, std::vector<ModelTriangle>& triangles, Camera& camera, int& renderType, int**& zBuffer) {
	resetDepthBuffer(window, zBuffer);
	if (camera.lookAtToggle == true) {
		camera.orbit(camera, camera.theta);
		camera.lookAt(camera, camera.lookAtPoint);
	}
	CanvasPoint imagePlanePoint;
	CanvasTriangle canvasPointTriangle;
	CanvasPoint scaledImagePlanePoint;
	float scalingFactor = camera.scalingFactor;

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
			drawFilledTriangle(canvasPointTriangle, triangles[i].colour, window,zBuffer);
		}

		if (renderType == 1) {
			drawStrokedTriangle(canvasPointTriangle, triangles[i].colour, window, zBuffer);
		}
		else if (renderType == 2) {
			drawFilledTriangle(canvasPointTriangle, triangles[i].colour, window, zBuffer);
		}

	}
}


void drawRefLines(DrawingWindow& window, int**& zBuffer) {	
	//drawLine(CanvasPoint(WIDTH / 2, 0), CanvasPoint(WIDTH / 2, 20), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(WIDTH / 2, HEIGHT), CanvasPoint(WIDTH / 2, HEIGHT-20), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(WIDTH / 4, 0), CanvasPoint(WIDTH / 4, 20), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(3*WIDTH / 4, 0), CanvasPoint(3*WIDTH / 4, 20), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(0, HEIGHT/4), CanvasPoint(20, (HEIGHT/4)), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(0, 3 * HEIGHT / 4), CanvasPoint(20,(3 * HEIGHT / 4)), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(0, HEIGHT/2), CanvasPoint(20, HEIGHT/2), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(WIDTH, HEIGHT/2), CanvasPoint(WIDTH-20,HEIGHT/2), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(WIDTH, HEIGHT / 4), CanvasPoint(WIDTH-20, (HEIGHT / 4)), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(WIDTH, 3 * HEIGHT / 4), CanvasPoint(WIDTH-20, (3 * HEIGHT / 4)), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(WIDTH/4, HEIGHT), CanvasPoint(WIDTH/4, HEIGHT-20), Colour(255, 0, 0), window);
	//drawLine(CanvasPoint(3*WIDTH/4, HEIGHT), CanvasPoint(3*WIDTH/4, (HEIGHT-20)), Colour(255, 0, 0), window);

	drawLine(CanvasPoint(0, HEIGHT / 2), CanvasPoint(WIDTH, HEIGHT / 2), Colour(255, 0, 0), window, zBuffer);
	drawLine(CanvasPoint(0, HEIGHT / 4), CanvasPoint(WIDTH, HEIGHT / 4), Colour(255, 0, 0), window, zBuffer);
	drawLine(CanvasPoint(0, 3 * HEIGHT / 4), CanvasPoint(WIDTH, (3 * HEIGHT / 4)), Colour(255, 0, 0), window, zBuffer);
	drawLine(CanvasPoint(WIDTH / 2, 0), CanvasPoint(WIDTH / 2, (HEIGHT)), Colour(255, 0, 0), window, zBuffer);
	drawLine(CanvasPoint(3 * WIDTH / 4, 0), CanvasPoint(3 * WIDTH / 4, HEIGHT), Colour(255, 0, 0), window, zBuffer);
	drawLine(CanvasPoint(WIDTH / 4, 0), CanvasPoint(WIDTH / 4, HEIGHT), Colour(255, 0, 0), window, zBuffer);
}


void handleEvent(SDL_Event event, DrawingWindow& window, Camera& camera, int& renderType, int& lightType, int**& zBuffer, bool& on, std::vector<ModelTriangle>& triangles) {
	int mode;
	float theta = 0.5;
	float scalingFactor = 0.35;
	float scalingFactor2 = 0.16;
	

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) {
			mode = 1;
			std::cout << "LEFT" << '\n';
			resetDepthBuffer(window, zBuffer);
			camera.translate(camera, mode);
			std::cout << glm::to_string(camera.cameraPosition) << '\n';
		}
		else if (event.key.keysym.sym == SDLK_RIGHT) {
			std::cout << "RIGHT" << '\n';
			mode = 2;
			resetDepthBuffer(window, zBuffer);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_UP) {
			std::cout << "UP" << '\n';
			mode = 3;
			resetDepthBuffer(window, zBuffer);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_DOWN) {
			std::cout << "DOWN" << '\n';
			mode = 4;
			resetDepthBuffer(window, zBuffer);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_z) {
			std::cout << "ZOOM" << '\n';
			mode = 5;
			resetDepthBuffer(window, zBuffer);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_x) {
			std::cout << "ZOOM OUT" << '\n';
			mode = 6;
			resetDepthBuffer(window, zBuffer);
			camera.translate(camera, mode);
		}
		else if (event.key.keysym.sym == SDLK_u) {
			std::cout << "U" << '\n';
			CanvasTriangle triangle(CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height));
			drawStrokedTriangle(triangle, Colour(rand() % 255, rand() % 255, rand() % 255), window, zBuffer);
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
			std::cout << "triangles2 " << triangles[26] << '\n';


		}
		else if (event.key.keysym.sym == SDLK_r) {
			std::cout << "pan in x" << '\n';
			mode = 1;
			resetDepthBuffer(window, zBuffer);
			camera.rotate(camera, mode, theta);

		}
		else if (event.key.keysym.sym == SDLK_t) {
			std::cout << "pan in y" << '\n';
			mode = 2;
			resetDepthBuffer(window, zBuffer);
			camera.rotate(camera, mode, theta);
		}
		else if (event.key.keysym.sym == SDLK_q) {
			std::cout << "rotate in x" << '\n';
			mode = 3;
			resetDepthBuffer(window, zBuffer);
			camera.rotate(camera, mode, theta);

		}
		else if (event.key.keysym.sym == SDLK_e) {
			std::cout << "rotate in y" << '\n';
			mode = 4;
			resetDepthBuffer(window, zBuffer);
			camera.rotate(camera, mode, theta);
		}
		else if (event.key.keysym.sym == SDLK_l) {
			std::cout << "look at" << '\n';
			if (camera.lookAtToggle == false) {
				camera.lookAtToggle = true;
			}
			else {
				camera.lookAtToggle = false;
			}
		}
		else if (event.key.keysym.sym == SDLK_k) {
			std::cout << glm::to_string(camera.cameraOrientation) << '\n';

			camera.lookAt(camera, camera.lookAtPoint);

			std::cout << glm::to_string(camera.cameraOrientation) << '\n';
		}
		else if (event.key.keysym.sym == SDLK_0) {

		}
		else if (event.key.keysym.sym == SDLK_m) {
			std::cout << "testing" << '\n';
			//directionVectorCalcs(camera);
			//window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_w) {
			std::cout << "move light source positive in y" << '\n';
			camera.lightSource.y = camera.lightSource.y + 0.1;
			std::cout << glm::to_string(camera.lightSource) << '\n';
		}
		else if (event.key.keysym.sym == SDLK_a) {
			std::cout << "move light source negative in x" << '\n';
			camera.lightSource.x = camera.lightSource.x - 0.1;
			std::cout << glm::to_string(camera.lightSource) << '\n';
		}
		else if (event.key.keysym.sym == SDLK_s) {
			std::cout << "move light source negative in y" << '\n';
			camera.lightSource.y = camera.lightSource.y - 0.1;
			std::cout << glm::to_string(camera.lightSource) << '\n';
		}
		else if (event.key.keysym.sym == SDLK_d) {
			std::cout << "move light source positive in x" << '\n';
			camera.lightSource.x = camera.lightSource.x + 0.1;
			std::cout << glm::to_string(camera.lightSource) << '\n';
		}
		else if (event.key.keysym.sym == SDLK_LCTRL) {
			std::cout << "move light source positive in z" << '\n';
			camera.lightSource.z = camera.lightSource.z + 0.1;
			std::cout << glm::to_string(camera.lightSource) << '\n';
		}
		else if (event.key.keysym.sym == SDLK_LSHIFT) {
			std::cout << "move light source negative in z" << '\n';
			camera.lightSource.z = camera.lightSource.z - 0.1;
			std::cout << glm::to_string(camera.lightSource) << '\n';
		}
		else if (event.key.keysym.sym == SDLK_1) {
			std::cout << "wireframe" << '\n';
			renderType = 1;
		}
		else if (event.key.keysym.sym == SDLK_2) {
			std::cout << "rasterised" << '\n';
			renderType = 2;
		}
		else if (event.key.keysym.sym == SDLK_3) {
			std::cout << "raytrace" << '\n';
			renderType = 3;
			drawRayTracing(window, triangles, camera, lightType);

		}
		else if (event.key.keysym.sym == SDLK_4) {
			std::cout << "proximity" << '\n';
			lightType = 1;
		}
		else if (event.key.keysym.sym == SDLK_5) {
			std::cout << "aoi" << '\n';
			lightType = 2;
		}
		else if (event.key.keysym.sym == SDLK_6) {
			std::cout << "specular" << '\n';
			lightType = 3;
		}
		else if (event.key.keysym.sym == SDLK_7) {
			std::cout << "combined" << '\n';
			lightType = 4;
		}
		else if (event.key.keysym.sym == SDLK_8) {
			std::cout << "gouraud" << '\n';
			lightType = 5;
			}
		else if (event.key.keysym.sym == SDLK_p) {
			std::cout << "delete zBuffer" << '\n';
			for (int i = 0; i < HEIGHT; ++i) {
				delete[] zBuffer[i];
			}
			delete[] zBuffer;
			std::cout << "yoo" << '\n';
		}


	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
	else {
		//std::cout << mouseX << "'" << mouseY << '\n';
	}
}


int main(int argc, char* argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	srand((unsigned int)time(NULL));
	float scalingFactor = 0.35;
	Camera camera;
	int renderType = 1;
	int lightType = 1;

	std::vector<ModelTriangle>& triangles = loadGeoOBJ(scalingFactor);
	
	bool on = true;

	//ZBUFFER
	int rows = HEIGHT;
	int columns = WIDTH;
	int** zBuffer = new int* [rows];
	for (size_t i = 0; i < rows; ++i) zBuffer[i] = new int[columns];
	resetDepthBuffer(window, zBuffer); //INITIALISE EVERY ELEMENT ZBUFFER WITH 0
	while (true) {
		if (window.pollForInputEvents(event)) handleEvent(event, window, camera, renderType, lightType, zBuffer, on, triangles);

		if (renderType == 1) {
			draw(window, triangles, camera, renderType, zBuffer);
		}
		else if (renderType == 2) {
			draw(window, triangles, camera, renderType, zBuffer);
		}
		else if (renderType == 3) {
			//drawRayTracing(window, triangles, camera, lightType);
		}

		//drawRefLines(window, zBuffer);
		window.renderFrame();
	}
}
