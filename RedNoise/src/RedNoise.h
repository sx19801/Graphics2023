#ifndef REDNOISE_H
#define REDNOISE_H


#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include "Triangle.h"
#include "camera.h"
#include "objFuncs.h"
#include "rayTracing.h"
#include "lightShade.h"
#include "textureMapping.h"

//#define WIDTH 640
//#define HEIGHT 480
#define WIDTH 320
#define HEIGHT 240
//#define WIDTH 1080
//#define HEIGHT 920
#define BLACK (255 << 24) + (0 << 16) + (0 << 8) + 0
#define RED (255 << 24) + (255 << 16) + (0 << 8) + 0

#endif // !REDNOISE_H
