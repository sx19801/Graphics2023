#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <Colour.h>
#include <glm/ext.hpp>
#include <Colour.h>
#include <algorithm>
#include "RedNoise.h"



std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues);

void resetDepthBuffer(DrawingWindow& window, int**& zBuffer);

void drawLine(CanvasPoint from, CanvasPoint to, Colour colour, DrawingWindow &window, int**& zBuffer);

void drawStrokedTriangle(CanvasTriangle& triangle, Colour colour, DrawingWindow &window, int**& zBuffer);

void drawFilledTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow &window, int**& zBuffer);

CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::mat3 cameraOrientation, glm::vec3 vertexPosition, float focalLength);

std::vector<CanvasPoint> orderByIncrY(CanvasTriangle triangle);

bool zDepthCheck(float x, float y, float z, int**& zBuffer);

#endif // !TRIANGLE_H
