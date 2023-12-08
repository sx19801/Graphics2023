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

void resetDepthBuffer(DrawingWindow& window, std::vector<std::vector<float>>& zBuffer);

void drawLine(CanvasPoint from, CanvasPoint to, Colour colour, DrawingWindow &window, std::vector<std::vector<float>>& zBuffer);

void drawStrokedTriangle(CanvasTriangle& triangle, Colour colour, DrawingWindow &window, std::vector<std::vector<float>>& zBuffer);

void drawFilledTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow &window, std::vector<std::vector<float>>& zBuffer);

CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::mat3 cameraOrientation, glm::vec3 vertexPosition, float focalLength);

std::vector<CanvasPoint> orderByIncrY(CanvasTriangle triangle);

void zDepthCheck(CanvasPoint xy, float z, std::vector<std::vector<float>>& zBuffer, Uint32 colourUint32, DrawingWindow& window);

#endif // !TRIANGLE_H
