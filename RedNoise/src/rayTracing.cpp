#include "rayTracing.h"


RayTriangleIntersection getClosestValidIntersection(std::vector<ModelTriangle>& triangles, glm::vec3 originalPoint, glm::vec3 rayDirection, int ignoreIndex, Camera& camera, bool& shadowCheck) {
	float closestIntersectedPointT = 1000;
	RayTriangleIntersection closestIntersection;
	float t;
	float u;
	float v;
	closestIntersection.t = -1;
	for (size_t i = 0; i < triangles.size(); i++) {

		glm::vec3 e0 = triangles[i].vertices[1] - triangles[i].vertices[0];
		glm::vec3 e1 = triangles[i].vertices[2] - triangles[i].vertices[0];
		glm::vec3 SPVector = originalPoint - triangles[i].vertices[0];
		glm::mat3 DEMatrix(-rayDirection, e0, e1);
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

		t = possibleSolution[0];
		u = possibleSolution[1];
		v = possibleSolution[2];
		//std::cout << t << " " << u << " " << v << '\n'; //all the u and vs are huge

	//	std::cout << glm::to_string(triangle.normal) << '\n';

		//u v check \/  and check that t (dist from camera to intersection) is positive
		if ((u >= 0.0) && (u <= 1.0) && (v >= 0.0) && (v <= 1.0) && ((u + v) <= 1.0)) {
			//std::cout << "t: " << t << " u: " << u << " v: " << v << std::endl;
			if (i != ignoreIndex && t > 0.015 && t < closestIntersectedPointT) {
				closestIntersection.intersectionPoint = getTriangleIntersectionPointT(originalPoint, normalise(rayDirection), t);
				closestIntersection.distanceFromPoints = glm::distance(camera.lightSource, closestIntersection.intersectionPoint);
				closestIntersection.intersectedTriangle = triangles[i];
				closestIntersection.triangleIndex = i;
				closestIntersection.valid = true;
				closestIntersection.intersectedTriangle.normal = triangles[i].normal;
				closestIntersectedPointT = t;
				closestIntersection.t = t;
				closestIntersection.u = u;
				closestIntersection.v = v;
				//if (t < 0.2) std::cout << t << '\n';
				//std::cout << "i: " << i << " t: " << t << " distance from point to light source: " << closestIntersection.distanceFromPoints << " intersection point: " << glm::to_string(closestIntersection.intersectionPoint) << '\n';

			}
			else if ((t > 0 && t <= 0.015) && i != ignoreIndex) {
				closestIntersection.t = 66;
				closestIntersection.intersectedTriangle.colour.name = triangles[i].colour.name;
				closestIntersection.intersectedTriangle.colour = triangles[i].colour;
				
			}
		}
		if (shadowCheck == true && closestIntersection.valid == true) {
			closestIntersection.shadow = true;
			closestIntersection.totalDistFromLight = glm::distance(camera.lightSource, originalPoint);
		}
	}

	if ((closestIntersectedPointT == 1000)) {
		closestIntersection.shadow = false;
		closestIntersection.intersectionPoint = originalPoint;
		closestIntersection.valid = false;
		closestIntersection.t = 1000;
	}

	

	//CHECK FOR SHADOWS

	//std::cout << "shadow bool:   " << closestIntersection.shadow << '\n';
	//closestIntersection.shadow = true;
	//You are looping through all the triangels so if the first one intersects valid is true but if the last one doesnt valid is false. Need 
	return closestIntersection;
	
}



glm::vec3 getTriangleIntersectionPointT(glm::vec3 originalPoint, glm::vec3 rayDirection, float t) {
	return originalPoint + t * rayDirection;
}

glm::vec3 getTrianglePointUV(glm::vec3 startVertex ,glm::vec3 edge0, glm::vec3 edge1, float u, float v) {
	return startVertex + (u * edge0) + (v * edge1);
}

glm::vec3 getPointInWorld(float u, float v, Camera& camera) {									
	float x = (u - (float(WIDTH)) / 2) / (camera.scalingFactor * camera.focalLength);
	float y = (v - (float(HEIGHT)) / 2) / (camera.scalingFactor * camera.focalLength);

	glm::vec3 distance = glm::vec3(x, -y, -1) * glm::inverse(camera.cameraOrientation); // THIS MIGHT NEED TO BE CHANGED IN FUTURE WITH CAMERA ROTATION/ORIENTATION
	glm::vec3 vertexPosition = distance + camera.cameraPosition;
	return vertexPosition;
}


glm::vec3 getDirectionVector(glm::vec3 vertexPositionFrom, glm::vec3 vertexPositionTo) {		
	glm::vec3 directionVector = normalise( vertexPositionTo - vertexPositionFrom);
	return directionVector;
}

//redundant
glm::vec3 directionVectorCalcs(Camera& camera) {
	CanvasPoint xy;
	xy = { 0 , 0 };
	float fov = atan(1 / camera.focalLength)*2;
	float scaling = tan(fov/2);
	float aspectRatio = float(WIDTH) / float(HEIGHT);
	//std::cout << aspectRatio << '\n';
	//changing xy into screen space coords
	float xNorm = (2 * ((xy.x + 0.5) / WIDTH) - 1) * aspectRatio * tan(fov / 2);
	float yNorm = (1 - 2 * ((xy.y + 0.5) / HEIGHT)) * tan(fov / 2);
	//std::cout << glm::to_string(camera.cameraPosition);

	//float z = camera.cameraPosition.z + camera.focalLength;

	glm::vec3 rayOrigin = camera.cameraPosition * camera.cameraOrientation;
	glm::vec3 rayDirection = glm::vec3(xNorm, yNorm, -1) * camera.cameraOrientation;


	//wrong below
	glm::vec3 directionVector = rayDirection - rayOrigin;
	//std::cout << glm::to_string(directionVector) << '\n';
	directionVector = normalise(directionVector);
	//std::cout << glm::to_string(directionVector) << '\n';
	return directionVector;
		
	
}

glm::vec3 normalise(glm::vec3 vector) {
	glm::vec3 normalisedVec;
	float magnitude	= sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	if (magnitude > 0) {
		normalisedVec = { vector.x / magnitude, vector.y / magnitude, vector.z / magnitude };
	}
	return normalisedVec;

}

void calcVertexNormalsPhong(std::vector<ModelTriangle>& triangles, RayTriangleIntersection intersectedTriangle) {
	std::array<glm::vec3, 3> vertexNormals;
	std::vector<ModelTriangle> sharedTriangles;
	//std::cout << triangles.size() << '\n';

	//consider one vertex
	//for (size_t i = 0; i < 3; i++) { //check single vertex
	//	for (size_t j = 0; j < triangles.size(); j++) { //loop through all triangles
	//		for (size_t k = 0; k < 3; k++) { //loop through each vertex on triangle
	//			if ((intersectedTriangle.intersectedTriangle.vertices[i] == triangles[j].vertices[k])) {
	//				sharedTriangles.push_back(triangles[j]);
	//			}
	//		}
	//	}
	//	//std::cout << sharedTriangles.size() << '\n';
	//	//average normal for vertex here
	//	vertexNormals[i] = calcAverageNormal(sharedTriangles);
	//	sharedTriangles = {};
	//}



	for (size_t i = 0; i < triangles.size(); i++) {//loop through all triangles
		for (size_t j = 0; j < 3; j++) {//loop through every vertex of a triangle
			for (size_t k = 0; k < triangles.size(); k++) {//check if vertex is shared by other triangles
				for (size_t l = 0; l < 3; l++) {//loop through every vertex of all triangles
					if ((triangles[i].vertices[j] == triangles[k].vertices[l])) {
						sharedTriangles.push_back(triangles[k]);
					}
				}
			}

			triangles[i].vertexNormals[j] = calcAverageNormal(sharedTriangles);//set triangle vertex normal to the average of all shared triangle normals
			sharedTriangles = {};//reset shared triangles

		}
	}
}

void calcVertexNormals(std::vector<ModelTriangle>& triangles) {
	
	std::vector<ModelTriangle> sharedTriangles;
	//std::cout << triangles.size() << '\n';


	
		for (size_t i = 0; i < triangles.size(); i++) {//loop through all triangles
			
				for (size_t j = 0; j < 3; j++) {//loop through every vertex of a triangle
					for (size_t k = 0; k < triangles.size(); k++) {//check if vertex is shared by other triangles
						for (size_t l = 0; l < 3; l++) {//loop through every vertex of all triangles
							if ((triangles[i].vertices[j] == triangles[k].vertices[l])) {
								sharedTriangles.push_back(triangles[k]);
							}
						}
					}

					triangles[i].vertexNormals[j] = calcAverageNormal(sharedTriangles);//set triangle vertex normal to the average of all shared triangle normals
					sharedTriangles = {};//reset shared triangles

				}
		
		}
	
}

glm::vec3 calcAverageNormal(std::vector<ModelTriangle>& sharedTriangles) {
	glm::vec3 averageNormal = {0,0,0};
	for (size_t i = 0; i < sharedTriangles.size(); i++) {
		averageNormal = sharedTriangles[i].normal + averageNormal;
	}
	averageNormal = averageNormal / sharedTriangles.size();
	//std::cout << glm::to_string(averageNormal);
	return averageNormal;
}