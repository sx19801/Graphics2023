#include "rayTracing.h"


void getClosestValidIntersection(std::vector<ModelTriangle> triangles, glm::vec3 cameraPosition, glm::vec3 rayDirection) {
	RayTriangleIntersection closestIntersection;
	float closestIntersectedPointT = 1000;
	
	for (size_t i = 0; i < triangles.size(); i++) {
		
		//first check closest then check valid
		closestIntersection.intersectedTriangle = triangles[i];
		ModelTriangle triangle = triangles[i];


		//std::cout << glm::to_string(triangle.vertices[0]) << " " << glm::to_string(triangle.vertices[1]) << " " << glm::to_string(triangle.vertices[2]) << " " << std::endl;

		glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
		glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
		glm::vec3 SPVector = cameraPosition - triangle.vertices[0];
		glm::mat3 DEMatrix(-rayDirection, e0, e1);
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

		float t;
		float u;
		float v;

	//	std::cout << "e0 : " << glm::to_string(e0) << " e1 : " << glm::to_string(e1) << std::endl;

		t = possibleSolution[0];
		u = possibleSolution[1];
		v = possibleSolution[2];


		//u v check \/  and check that t (dist from camera to intersection) is positive   DO CHECKS REQUIRE NORMALISED POINTS?????
		if ((u >= 0.0) && (u <= 1.0) && (v >= 0.0) && (v <= 1.0) && ((u + v) <= 1.0)) {
			//std::cout << "t: " << t << " u: " << u << " v: " << v << std::endl;
			if (t > 0) {
				glm::vec3 normalisedDirection = normalise(rayDirection);
				//SMALLEST T VALUE CHECK \/
				if (t < closestIntersectedPointT) {
					std::cout << triangle.colour << std::endl;
					//std::cout << "closestintersectionpointT :" << closestIntersectedPointT << std::endl;
					closestIntersection.intersectionPoint = getIntersectionPoint(cameraPosition, normalisedDirection, t);
					closestIntersectedPointT = t;
					std::cout << "t :" << t << std::endl;
					std::cout << closestIntersection.intersectedTriangle.colour << " " << closestIntersection.intersectedTriangle << " " << glm::to_string(closestIntersection.intersectionPoint) << std::endl;
				}
			}

		}
	}

	
}

glm::vec3 getIntersectionPoint(glm::vec3 cameraPosition, glm::vec3 rayDirection, float t) {
	glm::vec3 intersectionPoint = cameraPosition + rayDirection * t;
	return intersectionPoint;
}

void directionVectorCalcs(glm::vec3 cameraPosition) {
	/////////DIRECTION CALCS/////////
}

glm::vec3 normalise(glm::vec3 vector) {
	glm::vec3 normalisedVec;
	float magnitude	= sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	if (magnitude > 0) {
		normalisedVec = { vector.x / magnitude, vector.y / magnitude, vector.z / magnitude };
	}
	return normalisedVec;

}