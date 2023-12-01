#include "rayTracing.h"



RayTriangleIntersection getClosestValidIntersection(std::vector<ModelTriangle>& triangles, glm::vec3 originalPoint, glm::vec3 rayDirection, int ignoreIndex, Camera& camera) {
	RayTriangleIntersection closestIntersection;
	float closestIntersectedPointT = 1000;

	for (size_t i = 0; i < triangles.size(); i++) {
		ModelTriangle triangle = triangles[i];

		glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
		glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
		glm::vec3 SPVector = originalPoint - triangle.vertices[0];
		glm::mat3 DEMatrix(-rayDirection, e0, e1);
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

		float t = possibleSolution[0];
		float u = possibleSolution[1];
		float v = possibleSolution[2];
		//std::cout << t << " " << u << " " << v << '\n'; //all the u and vs are huge

	//	std::cout << glm::to_string(triangle.normal) << '\n';

		//u v check \/  and check that t (dist from camera to intersection) is positive
		if ((u >= 0.0) && (u <= 1.0) && (v >= 0.0) && (v <= 1.0) && ((u + v) <= 1.0)) {
			//std::cout << "t: " << t << " u: " << u << " v: " << v << std::endl;
//			if (t > 0 && i != ignoreIndex && t > 0.0001) {
			if (t > 0.04) {
				glm::vec3 normalisedDirection = normalise(rayDirection);
				//SMALLEST T VALUE CHECK \/
				if ((t < closestIntersectedPointT) && ignoreIndex != i) {
					closestIntersection.intersectionPoint = getTriangleIntersectionPointT(originalPoint, normalisedDirection, t);
					closestIntersection.distanceFromPoints = glm::distance(camera.lightSource, closestIntersection.intersectionPoint);
					closestIntersection.intersectedTriangle = triangles[i];
					closestIntersection.triangleIndex = i;
					closestIntersection.valid = true;
					closestIntersection.intersectedTriangle.normal = triangles[i].normal;
					closestIntersectedPointT = t;
						//std::cout << "i: " << i << " t: " << t << " distance from point to light source: " << closestIntersection.distanceFromPoints << " intersection point: " << glm::to_string(closestIntersection.intersectionPoint) << '\n';
					
				}
			}
		}
	}

	//CHECK FOR NO VALID INTERSECTIONS
	if (closestIntersection.valid == false) {
		closestIntersection.intersectionPoint = originalPoint;
		closestIntersection.distanceFromPoints = glm::distance(camera.lightSource, closestIntersection.intersectionPoint);
		closestIntersection.valid = true;
	}

	//std::cout << "shadow bool:   " << closestIntersection.shadow << '\n';
	//closestIntersection.shadow = true;
	//You are looping through all the triangels so if the first one intersects valid is true but if the last one doesnt valid is false. Need 
	return closestIntersection;
	
}



glm::vec3 getTriangleIntersectionPointT(glm::vec3 originalPoint, glm::vec3 rayDirection, float t) {
	glm::vec3 intersectionPoint = originalPoint + t*rayDirection ;
	return intersectionPoint;
}


glm::vec3 getPointInWorld(float u, float v, Camera& camera) {									
	float scalingFactor = camera.scalingFactor;
	float x = (u - (float(WIDTH)) / 2) / (scalingFactor * camera.focalLength);
	float y = (v - (float(HEIGHT)) / 2) / (scalingFactor * camera.focalLength);

	glm::vec3 distance = glm::vec3(x, -y, -1) * glm::inverse(camera.cameraOrientation); // THIS MIGHT NEED TO BE CHANGED IN FUTURE WITH CAMERA ROTATION/ORIENTATION
	glm::vec3 vertexPosition = distance + camera.cameraPosition;
	return vertexPosition;
}


glm::vec3 getDirectionVector(glm::vec3 vertexPositionTo, glm::vec3 vertexPositionFrom) {		
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