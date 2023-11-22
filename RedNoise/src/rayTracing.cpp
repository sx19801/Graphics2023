#include "rayTracing.h"


#define WIDTH 320
#define HEIGHT 240
#define PI 3.141592

RayTriangleIntersection getClosestValidIntersection(std::vector<ModelTriangle>& triangles, glm::vec3 cameraPosition, glm::vec3 rayDirection) {
	RayTriangleIntersection closestIntersection;
	float closestIntersectedPointT = 1000;

	for (size_t i = 0; i < triangles.size(); i++) {
		ModelTriangle triangle = triangles[i];

		glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
		glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
		glm::vec3 SPVector = cameraPosition - triangle.vertices[0];
		glm::mat3 DEMatrix(-rayDirection, e0, e1);
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

		float t = possibleSolution[0];
		float u = possibleSolution[1];
		float v = possibleSolution[2];
		//std::cout << t << " " << u << " " << v << '\n'; //all the u and vs are huge


		//u v check \/  and check that t (dist from camera to intersection) is positive   DO CHECKS REQUIRE NORMALISED POINTS?????
		if ((u >= 0.0) && (u <= 1.0) && (v >= 0.0) && (v <= 1.0) && ((u + v) <= 1.0)) {
			//std::cout << "t: " << t << " u: " << u << " v: " << v << std::endl;
			if (t > 0) {
				glm::vec3 normalisedDirection = normalise(rayDirection);
				//SMALLEST T VALUE CHECK \/
				if (t < closestIntersectedPointT) {
					closestIntersection.intersectionPoint = getTriangleIntersectionPointT(cameraPosition, normalisedDirection, t);
					
					closestIntersection.intersectedTriangle = triangles[i];
					closestIntersectedPointT = t;
					closestIntersection.valid = true;
					
				}
			}

		}
	}
	//You are looping through all the triangels so if the first one intersects valid is true but if the last one doesnt valid is false. Need 
	return closestIntersection;
	
}



glm::vec3 getTriangleIntersectionPointT(glm::vec3 cameraPosition, glm::vec3 rayDirection, float t) {
	glm::vec3 intersectionPoint = cameraPosition + rayDirection * t;
	return intersectionPoint;
}


glm::vec3 getPointInWorld(float u, float v, Camera& camera) {
	float scalingFactor = 80;
	float x = ((WIDTH - u) - (float(WIDTH)) / 2) / (scalingFactor * camera.focalLength);
	float y = (v - (float(HEIGHT)) / 2) / (scalingFactor * camera.focalLength);

	glm::vec3 distance = glm::vec3(x, y, camera.focalLength) * glm::inverse(camera.cameraOrientation);
	glm::vec3 vertexPosition = distance + camera.cameraPosition;
	return vertexPosition;
}

glm::vec3 getDirectionVector(glm::vec3 vertexPosition, Camera& camera) {
	glm::vec3 directionVector = normalise(camera.cameraPosition - vertexPosition);
	return directionVector;
}

//redundant
glm::vec3 directionVectorCalcs(Camera& camera) {
	CanvasPoint xy;
	xy = { 0 , 0 };
	float fov = atan(1 / camera.focalLength)*2;
	float scaling = tan(fov/2);
	float aspectRatio = float(WIDTH) / float(HEIGHT);
	std::cout << aspectRatio << '\n';
	//changing xy into screen space coords
	float xNorm = (2 * ((xy.x + 0.5) / WIDTH) - 1) * aspectRatio * tan(fov / 2);
	float yNorm = (1 - 2 * ((xy.y + 0.5) / HEIGHT)) * tan(fov / 2);
	//std::cout << glm::to_string(camera.cameraPosition);

	//float z = camera.cameraPosition.z + camera.focalLength;

	glm::vec3 rayOrigin = camera.cameraPosition * camera.cameraOrientation;
	glm::vec3 rayDirection = glm::vec3(xNorm, yNorm, -1) * camera.cameraOrientation;


	//wrong below
	glm::vec3 directionVector = rayDirection - rayOrigin;
	std::cout << glm::to_string(directionVector) << '\n';
	directionVector = normalise(directionVector);
	std::cout << glm::to_string(directionVector) << '\n';
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