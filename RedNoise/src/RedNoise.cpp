#include "RedNoise.h"

bool intersects(glm::vec3 origin, glm::vec3 ray, glm::vec3 minBox, glm::vec3 maxBox) {
	float tmin = (minBox.x - origin.x) / ray.x;
	float tmax = (maxBox.x - origin.x) / ray.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (minBox.y - origin.y) / ray.y;
	float tymax = (maxBox.y - origin.y) / ray.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (minBox.z - origin.z) / ray.z;
	float tzmax = (maxBox.z - origin.z) / ray.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	return true;
}

std::vector<glm::vec3> calculateBoundingBox(std::vector<glm::vec3>& trianglesVertices) {
	glm::vec3 min = trianglesVertices[0];
	glm::vec3 max = trianglesVertices[0];

	for (const auto& vertex : trianglesVertices) {
		min.x = std::min(min.x, vertex.x);
		min.y = std::min(min.y, vertex.y);
		min.z = std::min(min.z, vertex.z);

		max.x = std::max(max.x, vertex.x);
		max.y = std::max(max.y, vertex.y);
		max.z = std::max(max.z, vertex.z);
	}

	return std::vector<glm::vec3> { min, max };
}

std::vector<glm::vec3> returnAllVertices(std::vector<ModelTriangle>& triangles) {
	std::vector<glm::vec3> vertices;
	//triangles[0].vertices[0].
	for (auto& triangle : triangles) {
		vertices.push_back(triangle.vertices[0]);
		vertices.push_back(triangle.vertices[1]);
		vertices.push_back(triangle.vertices[2]);
	}
	return vertices;
}

void drawRayTracing(DrawingWindow& window, std::vector<ModelTriangle>& triangles, Camera& camera, int& lightType, std::vector<std::vector<float>>& shadowMap, std::vector<std::vector<float>>& shadowMap2, std::vector<std::vector<glm::vec3>>& colourMap) {
	glm::vec3 colour;
	float pointToLightDist;
	float distance2;
	float intensity = 0;
	float intensity1 = 0;
	float intensity2 = 0;
	float intensity3 = 0;
	float aoi1 = 0;
	float aoi2 = 1;
	float aoi3 = 1;
	float specular1 = 0;
	float specular2 = 0;
	float specular3 = 0;
	float proximity = 0;
	float ambient = 0.04;
	float colourOfVertex0;
	float colourOfVertex1;
	float colourOfVertex2;
	float colourOfVertexAOI0;
	float colourOfVertexAOI1;
	float colourOfVertexAOI2;
	float combine1;
	float combine2;
	float combine3;
	float combine = 1;
	float colourOfPoint;
	float colourOfPointAOI;
	float pointToLightDistance1;
	float pointToLightDistance2;
	float pointToLightDistance3;


	glm::vec3 combined = {};
	float specular = 0;
	bool shadowCheck = false;
	std::array<glm::vec3, 3> vertexNormals;
	uint32_t colourUint32 = colourUint32 = (255 << 24) + (int(round(colour.r)) << 16) + (int(round(colour.g)) << 8) + int(round(colour.b));
	glm::vec3 pointInSpace;
	glm::vec3 directionVector;
	RayTriangleIntersection closestValidIntersection;
	glm::vec3 lightDirectionVector;
	glm::vec3 lightDirectionVector1;
	glm::vec3 lightDirectionVector2;
	glm::vec3 lightDirectionVector3;
	glm::vec3 directionVector1;
	glm::vec3 directionVector2;
	glm::vec3 directionVector3;
	glm::vec3 lightDirectionVectorAOI1;
	glm::vec3 lightDirectionVectorAOI2;
	glm::vec3 lightDirectionVectorAOI3;
	RayTriangleIntersection checkShadowIntersectionPoint;
	glm::vec3 pointNormal;

	int colourR;
	int colourG;
	int colourB;

	int i = 0;
	std::vector<glm::vec3> triangleVertices = returnAllVertices(triangles);
	std::vector<glm::vec3> boundingBox = calculateBoundingBox(triangleVertices);
	calcVertexNormals(triangles);


	for (size_t y = 0; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH; x++) {
			pointInSpace = getPointInWorld(x, y, camera);
			directionVector = getDirectionVector(camera.cameraPosition, pointInSpace);

			//bounding box here

			bool intersectionWithBBox = intersects(camera.cameraPosition, directionVector, boundingBox[0], boundingBox[1]);

			//std::cout << "x: " << x << " y: " << y << " bool: " << intersectionWithBBox << '\n';

			if (!(intersectionWithBBox == 0)) {
				closestValidIntersection = getClosestValidIntersection(triangles, camera.cameraPosition, directionVector, -1, camera, shadowCheck);

				if (closestValidIntersection.valid == true) {

					colour.r = closestValidIntersection.intersectedTriangle.colour.red;
					colour.g = closestValidIntersection.intersectedTriangle.colour.green;
					colour.b = closestValidIntersection.intersectedTriangle.colour.blue;

					

					/*colourR = int(std::clamp(round((colour.r)), 0.0f, 255.0f));
					colourG = int(std::clamp(round((colour.g)), 0.0f, 255.0f));
					colourB = int(std::clamp(round((colour.b)), 0.0f, 255.0f));


					colourMap[x][y] = { colourR,colourG,colourB };*/

					//directionVector = getDirectionVector(camera.cameraPosition, pointInSpace);
					pointToLightDist = glm::distance(closestValidIntersection.intersectionPoint, camera.lightSource);
					closestValidIntersection.shadowCheck = true;




					switch (lightType)
					{
					case 1:
						intensity = proximityLightIntensity(pointToLightDist);
						//colourUint32 = (255 << 24) + (int(round(colour.red * intensity)) << 16) + (int(round(colour.green * intensity)) << 8) + int(round(colour.blue * intensity));
						break;

					case 2:
						aoi1 = angleOfIncidence(lightDirectionVector, closestValidIntersection.intersectedTriangle.normal);
						//colourUint32 = (255 << 24) + (int(round(colour.red * aoi1)) << 16) + (int(round(colour.green * aoi1)) << 8) + int(round(colour.blue * aoi1));
						break;

					case 3:
						directionVector = getDirectionVector(closestValidIntersection.intersectionPoint, camera.cameraPosition);
						lightDirectionVector = getDirectionVector(camera.lightSource, closestValidIntersection.intersectionPoint);
						specular = specularLighting(lightDirectionVector, closestValidIntersection.intersectedTriangle.normal, directionVector);
						specular = specular * 255;
						//std::cout << specular << '\n';
						//colourUint32 = (255 << 24) + (int(round(255 * specular)) << 16) + (int(round(255 * specular)) << 8) + int(round(255 * specular));
						break;

					case 4:
						intensity = proximityLightIntensity(pointToLightDist);
						aoi1 = angleOfIncidence(lightDirectionVector, closestValidIntersection.intersectedTriangle.normal);
						specular = specularLighting(lightDirectionVector, closestValidIntersection.intersectedTriangle.normal, directionVector);
						specular *= 255;
						//specular = 255 * specular;
						//combine = intensity+aoi1+0.2;
						//if (combine > 1) combine = 1;
						//if (combine < 0) combine = 0;
						//colourUint32 = (255 << 24) + (int(std::clamp(round(colour.r * combine + specular),0.0f,255.0f)) << 16) + (int(std::clamp(round(colour.g * combine + specular), 0.0f, 255.0f)) << 8) + std::clamp(round(colour.b * combine + specular), 0.0f, 255.0f);
						break;

					case 5: //GOURAUD
						//calculate aoi of each  vertex
						lightDirectionVectorAOI1 = getDirectionVector(closestValidIntersection.intersectedTriangle.vertices[0], camera.lightSource);
						lightDirectionVectorAOI2 = getDirectionVector(closestValidIntersection.intersectedTriangle.vertices[1], camera.lightSource);
						lightDirectionVectorAOI3 = getDirectionVector(closestValidIntersection.intersectedTriangle.vertices[2], camera.lightSource);

						//directionVector = getDirectionVector(closestValidIntersection.intersectionPoint, camera.cameraPosition);
						//calculate specular of each vertex
						lightDirectionVector1 = getDirectionVector(camera.lightSource, closestValidIntersection.intersectedTriangle.vertices[0]);
						directionVector1 = getDirectionVector(closestValidIntersection.intersectedTriangle.vertices[0], camera.cameraPosition);
						specular1 = specularLighting(lightDirectionVector1, closestValidIntersection.intersectedTriangle.vertexNormals[0], directionVector1);

						lightDirectionVector2 = getDirectionVector(camera.lightSource, closestValidIntersection.intersectedTriangle.vertices[1]);
						directionVector2 = getDirectionVector(closestValidIntersection.intersectedTriangle.vertices[1], camera.cameraPosition);
						specular2 = specularLighting(lightDirectionVector2, closestValidIntersection.intersectedTriangle.vertexNormals[1], directionVector2);

						lightDirectionVector3 = getDirectionVector(camera.lightSource, closestValidIntersection.intersectedTriangle.vertices[2]);
						directionVector3 = getDirectionVector(closestValidIntersection.intersectedTriangle.vertices[2], camera.cameraPosition);
						specular3 = specularLighting(lightDirectionVector3, closestValidIntersection.intersectedTriangle.vertexNormals[2], directionVector3);

						//if ((specular1 > 0.2 || specular2 > 0.2 || specular3 > 0.2)) std::cout << specular1 << " " << specular2 << " " << specular3 << '\n';

						aoi1 = angleOfIncidence(lightDirectionVectorAOI1, closestValidIntersection.intersectedTriangle.vertexNormals[0]);
						aoi2 = angleOfIncidence(lightDirectionVectorAOI2, closestValidIntersection.intersectedTriangle.vertexNormals[1]);
						aoi3 = angleOfIncidence(lightDirectionVectorAOI3, closestValidIntersection.intersectedTriangle.vertexNormals[2]);

						pointToLightDistance1 = glm::distance(closestValidIntersection.intersectedTriangle.vertexNormals[0], camera.lightSource);
						pointToLightDistance2 = glm::distance(closestValidIntersection.intersectedTriangle.vertexNormals[1], camera.lightSource);
						pointToLightDistance3 = glm::distance(closestValidIntersection.intersectedTriangle.vertexNormals[2], camera.lightSource);

						intensity1 = proximityLightIntensity(pointToLightDistance1);
						intensity2 = proximityLightIntensity(pointToLightDistance1);
						intensity3 = proximityLightIntensity(pointToLightDistance1);

						intensity1 = (1 - closestValidIntersection.u - closestValidIntersection.v) * intensity1;
						colourOfVertex1 = closestValidIntersection.u * intensity2;
						colourOfVertex2 = closestValidIntersection.v * intensity3;

						colourOfVertex0 = (1 - closestValidIntersection.u - closestValidIntersection.v) * 255 * specular1;
						colourOfVertex1 = closestValidIntersection.u * 255 * specular2;
						colourOfVertex2 = closestValidIntersection.v * 255 * specular3;

						colourOfVertexAOI0 = (1 - closestValidIntersection.u - closestValidIntersection.v) * aoi1;
						colourOfVertexAOI1 = closestValidIntersection.u * aoi2;
						colourOfVertexAOI2 = closestValidIntersection.v * aoi3;

						colourOfPoint = colourOfVertex0 + colourOfVertex1 + colourOfVertex2;
						colourOfPointAOI = colourOfVertexAOI0 + colourOfVertexAOI1 + colourOfVertexAOI2;

						combine1 = std::clamp((colour.r * colourOfPointAOI) + colourOfPoint, 0.0f, 255.0f);
						combine2 = std::clamp((colour.g * colourOfPointAOI) + colourOfPoint, 0.0f, 255.0f);
						combine3 = std::clamp((colour.b * colourOfPointAOI) + colourOfPoint, 0.0f, 255.0f);


						//std::cout << combine1 << " " << combine2 << " " << combine3 << " " << '\n';

						//if (colourOfPoint > 1) std::cout << "shit";

						//colourUint32 = (255 << 24) + (int(combine1) << 16) + (int(combine2) << 8) + int(combine3);

						break;

					case 6://Phong
						glm::vec3 point1 = (1 - closestValidIntersection.u - closestValidIntersection.v) * closestValidIntersection.intersectedTriangle.vertexNormals[0];
						glm::vec3 point2 = closestValidIntersection.u * closestValidIntersection.intersectedTriangle.vertexNormals[1];
						glm::vec3 point3 = closestValidIntersection.v * closestValidIntersection.intersectedTriangle.vertexNormals[2];

						pointNormal = point1 + point2 + point3;
						pointNormal = normalise(pointNormal);



						aoi1 = angleOfIncidence(lightDirectionVector, pointNormal);

						directionVector2 = getDirectionVector(closestValidIntersection.intersectionPoint, camera.cameraPosition);
						lightDirectionVector2 = getDirectionVector(camera.lightSource, closestValidIntersection.intersectionPoint);
						specular = specularLighting(lightDirectionVector2, pointNormal, directionVector2);
						specular = specular * 255;
						//std::cout << specular << '\n';

						intensity = proximityLightIntensity(pointToLightDist);
						//combine = (1.7 * intensity * (aoi1));
						ambient = 0.15;
						//colourUint32 = (255 << 24) + (int(std::clamp(round((colour.red * combine) + specular), 0.0f, 255.0f)) << 16) + (int(std::clamp(round((colour.green * combine) + specular), 0.0f, 255.0f)) << 8) + std::clamp(round((colour.blue * combine) + specular), 0.0f, 255.0f);


						break;

					}


					//combine = aoi1*0.2+ambient*0.2+intensity*5;
					//combine = 4 * intensity* (aoi1) + aoi1*0.4 + ambient ;
					//std::cout << combine << '\n';

					//REFLECTIVE
					if (closestValidIntersection.intersectedTriangle.colour.name == "Grey") { //reflective blue box
						glm::vec3 point1 = (1 - closestValidIntersection.u - closestValidIntersection.v) * closestValidIntersection.intersectedTriangle.vertexNormals[0];
						glm::vec3 point2 = closestValidIntersection.u * closestValidIntersection.intersectedTriangle.vertexNormals[1];
						glm::vec3 point3 = closestValidIntersection.v * closestValidIntersection.intersectedTriangle.vertexNormals[2];

						pointNormal = point1 + point2 + point3;
						pointNormal = normalise(pointNormal);
						pointNormal = closestValidIntersection.intersectedTriangle.normal;
						glm::vec3 viewDirectionVectorToPoint = getDirectionVector(camera.cameraPosition, closestValidIntersection.intersectionPoint);
						glm::vec3 reflectionDirectionVector = vectorOfReflection(viewDirectionVectorToPoint, pointNormal);
						reflectionDirectionVector = normalise(reflectionDirectionVector);
						std::vector<ModelTriangle> triangle;

						//triangle.push_back(closestValidIntersection.intersectedTriangle);
						RayTriangleIntersection reflectedIntersection = getClosestValidIntersection(triangles, closestValidIntersection.intersectionPoint, reflectionDirectionVector, closestValidIntersection.triangleIndex, camera, shadowCheck);

						if (reflectedIntersection.valid == false && reflectedIntersection.intersectedTriangle.colour.name == "Magenta") {
							colour.r = (reflectedIntersection.intersectedTriangle.colour.red);
							colour.g = (reflectedIntersection.intersectedTriangle.colour.green);
							colour.b = (reflectedIntersection.intersectedTriangle.colour.blue);

							//std::cout << closestValidIntersection.intersectedTriangle.colour.name << '\n';
						}
						else {
							combine = ((intensity)) + (aoi1 * (intensity / 2)) + ambient;
							//combine = 1;
							colour.r = (reflectedIntersection.intersectedTriangle.colour.red * combine);
							colour.g = (reflectedIntersection.intersectedTriangle.colour.green * combine);
							colour.b = (reflectedIntersection.intersectedTriangle.colour.blue * combine);
						}
						i++;

					}

					//STORE COLOUR INTO COLOUR MAP
					
						
					



					//MULTI LIGHTSOURCES
					float radius = 0.25;
					int numberOfLights = 8;
					std::vector<glm::vec3> lights;
					for (size_t i = 0; i < numberOfLights; i++) {
						lights.push_back(camera.getLightSource(camera.lightSource, radius));
						lightDirectionVector = getDirectionVector(closestValidIntersection.intersectionPoint, lights[i]);
						checkShadowIntersectionPoint = getClosestValidIntersection(triangles, closestValidIntersection.intersectionPoint, lightDirectionVector, closestValidIntersection.triangleIndex, camera, closestValidIntersection.shadowCheck);
						distance2 = glm::distance(closestValidIntersection.intersectionPoint, checkShadowIntersectionPoint.intersectionPoint);

						if ((distance2 < pointToLightDist) && distance2 > 0) {
							intensity = 0;
							aoi1 = 0;
							specular = 0;
							//ambient = 0.07;

							if (shadowMap[x][y] == 0) {
								shadowMap[x][y] = 0.15;
							}
							else {
								shadowMap[x][y] = shadowMap[x][y] - 0.05;
							}
							//ambient = shadowMap[x][y];
						}
						

					}


					//				combine = ((intensity)) + (aoi1*(intensity*0.83)) + ambient;
					combine = ((intensity)) + (aoi1 * (intensity)) + ambient;


					if (combine > 1) combine = 1;
					if (combine < 0) combine = 0;
					//combine = aoi1*intensity + intensity;

					//specular = specular * 0.7;

					colourR = int(std::clamp(round((colour.r * combine) + specular), 0.0f, 255.0f));
					colourG = int(std::clamp(round((colour.g * combine) + specular), 0.0f, 255.0f));
					colourB = int(std::clamp(round((colour.b * combine) + specular), 0.0f, 255.0f));

					//if (combine < 0.1) combine = 0.1;
					colourUint32 = (255 << 24) + (colourR << 16) + (colourG << 8) + colourB;

					if (lightType == 5) {
						colourUint32 = (255 << 24) + (int(combine1) << 16) + (int(combine2) << 8) + int(combine3);
					}
					window.setPixelColour(x, y, colourUint32);

					lights.clear();
				}
				else { //NO VALID INTERSECTIONS 
					window.setPixelColour(x, y, BLACK);
				}
				
			}
		}
	}



//	for (int i = 0; i < WIDTH; i++) {
//		for (int j = 0; j < HEIGHT; j++) {
//		
////			std::cout << shadowMap[i][j] << " ";
//
//			if (!shadowMap[i][j] == 0) {
//				shadowMap2[i][j] = shadowMap[i][j] + shadowMap[std::clamp(i - 1, 0, WIDTH)][std::clamp(j - 1, 0, HEIGHT - 1)] + shadowMap[std::clamp(i - 1, 0, WIDTH - 1)][j] + shadowMap[std::clamp(i - 1, 0, WIDTH - 1)][std::clamp(j + 1, 0, HEIGHT - 1)] + shadowMap[i][std::clamp(j - 1, 0, HEIGHT - 1)] + shadowMap[std::clamp(i + 1, 0, WIDTH - 1)][std::clamp(j - 1, 0, HEIGHT - 1)] + shadowMap[std::clamp(i + 1, 0, WIDTH - 1)][j] + shadowMap[i][std::clamp(j + 1, 0, HEIGHT - 1)] + shadowMap[std::clamp(i + 1, 0, WIDTH - 1)][std::clamp(j + 1, 0, HEIGHT - 1)];
//				shadowMap2[i][j] = shadowMap2[i][j] / 9;
//
//				ambient = shadowMap2[i][j];
//
//				glm::vec3 colourFromMap = colourMap[i][j];
//
//				combine = 1;
//
//				int colourR1 = int(std::clamp(round((colourFromMap.r * combine)), 0.0f, 255.0f));
//				int colourG1 = int(std::clamp(round((colourFromMap.g * combine)), 0.0f, 255.0f));
//				int colourB1 = int(std::clamp(round((colourFromMap.b * combine)), 0.0f, 255.0f));
//
//
//				colourUint32 = (255 << 24) + (colourR1 << 16) + (colourG1 << 8) + colourB1;
//				//window.setPixelColour(i, j, colourUint32);
//			}
//
//		}
//		//std::cout << '\n';
//	}
	
}


void draw(DrawingWindow& window, std::vector<ModelTriangle>& triangles, Camera& camera, int& renderType, int& texture, std::vector<std::vector<float>>& zBuffer) {
//	resetDepthBuffer(window, zBuffer);
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
		/*if (!(triangles[i].colour.name == "Cobbles")) {*/
			
			for (size_t j = 0; j < 3; j++) {
				glm::vec3 vertexPosition = triangles[i].vertices[j];

				if (triangles[i].colour.name == "White") {
					vertexPosition.y = vertexPosition.y - 0.02;
				}
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

				if (triangles[i].colour.name == "Cobbles") {
					canvasPointTriangle.v0().texturePoint = triangles[i].texturePoints[0];
					canvasPointTriangle.v1().texturePoint = triangles[i].texturePoints[1];
					canvasPointTriangle.v2().texturePoint = triangles[i].texturePoints[2];
					
				
				}
			}

			
			
			//_sleep(800);

			if (triangles[i].colour.name == "Cobbles" && texture == 1) {
				//CanvasTriangle triangle = convertModelTriToCanvas(triangles[i]);
				loadTextureMap(canvasPointTriangle, window, zBuffer);
			}
			else if (triangles.size() == 1) {
				drawFilledTriangle(canvasPointTriangle, triangles[i].colour, window,zBuffer);
			}
			else if (renderType == 1) {
				drawStrokedTriangle(canvasPointTriangle, triangles[i].colour, window, zBuffer);
			}
			else if (renderType == 2) {
			//	std::cout << "yo" << '\n';
				drawFilledTriangle(canvasPointTriangle, triangles[i].colour, window, zBuffer);
				//window.renderFrame();
			}
		//}
	}
}


void drawRefLines(DrawingWindow& window, std::vector<std::vector<float>>& zBuffer) {
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


void handleEvent(SDL_Event event, DrawingWindow& window, Camera& camera, int& renderType, int& lightType, int& texture, std::vector<std::vector<float>>& zBuffer, bool& on, std::vector<ModelTriangle>& triangles, std::vector<std::vector<float>>& shadowMap, std::vector<std::vector<float>>& shadowMap2, std::vector<std::vector<glm::vec3>>& colourMap) {
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
			std::vector<ModelTriangle>& triangles = loadGeoOBJ(1, window);
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
		else if (event.key.keysym.sym == SDLK_f) {
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
			resetDepthBuffer(window, zBuffer);
			drawRayTracing(window, triangles, camera, lightType, shadowMap, shadowMap2, colourMap);
			window.renderFrame();
		}
		else if (event.key.keysym.sym == SDLK_t) {
			std::cout << "textured floor" << '\n';
			texture = 1;
			
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
		else if (event.key.keysym.sym == SDLK_9) {
			std::cout << "phong" << '\n';
			lightType = 6;
		}
		else if (event.key.keysym.sym == SDLK_p) {
			std::cout << glm::to_string(camera.cameraOrientation) << " " << glm::to_string(camera.cameraPosition) << '\n';
		}


	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		/*window.savePPM("output.ppm");
		window.saveBMP("output.bmp");*/
		int x, y;
		SDL_GetMouseState(&x, &y);
		window.setPixelColour(x, y, RED);
		window.renderFrame();
		std::cout << x << " " << y << '\n';
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
	int texture = 0;

	std::vector<ModelTriangle>& triangles = loadGeoOBJ(scalingFactor, window);

	bool on = true;

	//ZBUFFER
	/*int rows = WIDTH;
	int columns = HEIGHT;
	int** zBuffer = new int* [columns];
	for (size_t i = 0; i < rows; ++i) zBuffer[i] = new int[rows];*/
	Colour WHITE = { 255, 255, 255 };

	std::vector<std::vector<float>> zBuffer(WIDTH, std::vector<float>(HEIGHT));
	std::vector<std::vector<float>> shadowMap(WIDTH, std::vector<float>(HEIGHT));
	std::vector<std::vector<float>> shadowMap2(WIDTH, std::vector<float>(HEIGHT));
	std::vector<std::vector<glm::vec3>> colourMap(WIDTH, std::vector<glm::vec3>(HEIGHT));

	CanvasPoint A = { 160, 10 };
	CanvasPoint B = { 300, 230 };
	CanvasPoint C = { 10, 150 };

	float widthTexture = 480;
	float heightTexture = 395;

	int numberOfLights = 1;
	std::vector<glm::vec3> lights;

	A.texturePoint = {float(195.0f/480.0f),float(5.0f/395.0f) };
	B.texturePoint = {float(395.0f/480.0f), float(380.0f/395.0f)};
	C.texturePoint = {float(65.0f/480.0f), float(330.0f/395.0f)};

	std::cout << A.texturePoint << " " << B.texturePoint << " " << C.texturePoint << '\n';
	CanvasTriangle triangle = { A, B, C };
	//loadTextureMap(triangle, window);
	//resetDepthBuffer(window, zBuffer); //INITIALISE EVERY ELEMENT ZBUFFER WITH 0
	//draw(window, triangles, camera, renderType, zBuffer);
	//drawRayTracing(window, triangles, camera, lightType);

	float radius = 0.25;
	glm::vec3 lightOrigin = { 0.7,0.7,-0.7 };
	while (true) {
		//std::cout << glm::to_string(camera.getLightSource(lightOrigin, radius)) << '\n';
	
		if (window.pollForInputEvents(event)) handleEvent(event, window, camera, renderType, lightType, texture, zBuffer, on, triangles, shadowMap, shadowMap2, colourMap);

		if (renderType == 1) {
			resetDepthBuffer(window, zBuffer);
			draw(window, triangles, camera, renderType, texture, zBuffer);
		}
		else if (renderType == 2) {
			resetDepthBuffer(window, zBuffer);
			draw(window, triangles, camera, renderType, texture, zBuffer);
		}
		else if (renderType == 3) {
			//drawRayTracing(window, triangles, camera, lightType, shadowMap, shadowMap2);
		}

		//drawStrokedTriangle(triangle, WHITE, window, zBuffer);
		
		//BEZIER CURVE
		//camera.update(camera);															//uncomment for curve movement


		//drawRefLines(window, zBuffer);

		lights.clear();
		window.renderFrame();
		//_sleep(50);
		//std::cout << "yo " << '\n';
	}
}
