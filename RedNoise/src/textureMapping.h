#ifndef TEXTUREMAPPING_H
#define TEXTUREMAPPING_H

#include "RedNoise.h"
#include "TextureMap.h"
void loadTextureMap(CanvasTriangle& triangle, DrawingWindow& window, std::vector<std::vector<float>>& zBuffer);

float edgeFunction(CanvasPoint& a, CanvasPoint& b, CanvasPoint& c);

glm::vec3 barycentricCoords(CanvasPoint& xy, CanvasPoint& a, CanvasPoint& b, CanvasPoint& c);

#endif // !TEXTUREMAPPING_H
