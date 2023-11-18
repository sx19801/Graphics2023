#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <Colour.h>
#include <glm/ext.hpp>

std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues);

void draw(DrawingWindow& window);

void drawLine(CanvasPoint from, CanvasPoint to, Colour colour, DrawingWindow &window);

void drawStrokedTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow &window);

void drawFilledTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow &window);

CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition, float focalLength);

std::vector<CanvasPoint> orderByIncrY(CanvasTriangle triangle);

#endif // !TRIANGLE_H
