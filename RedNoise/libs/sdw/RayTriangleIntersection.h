#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "ModelTriangle.h"

struct RayTriangleIntersection {
	glm::vec3 intersectionPoint;
	float distanceFromPoints;
	float totalDistFromLight;
	ModelTriangle intersectedTriangle;
	size_t triangleIndex;
	bool valid = false;
	bool shadowCheck = false;
	bool shadow = false;
	float u = 0;
	float v = 0;


	RayTriangleIntersection();
	RayTriangleIntersection(const glm::vec3 &point, float distance, const ModelTriangle &triangle, size_t index);
	friend std::ostream &operator<<(std::ostream &os, const RayTriangleIntersection &intersection);
};
