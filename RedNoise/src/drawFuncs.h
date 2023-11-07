#ifndef DRAWFUNCS_H
#define DRAWFUNCS_H
#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <CanvasPoint.h>
#include <Colour.h>
#include <CanvasTriangle.h>
#include <ModelTriangle.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>


//initalising zbuffer with height and width of screen
float zBuffer[960][720] = {};


std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues) {
	float total = 0;
	float spacing;
	int i = 0;
	std::vector<float> evenVector;
	total = to - from;
	spacing = total / (numberOfValues - 1);
	
	//avoiding fringe case of single point
	if (numberOfValues == 1) {
		evenVector.push_back(from);
		return evenVector;
	}

	for (int i = 0; i < numberOfValues; i++) {
		evenVector.push_back(from + spacing * i);
	}

	return evenVector;
}

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues) {
	float total1 = 0;
	float spacing1;

	std::vector<glm::vec3> vecOfVecs;
	glm::vec3 vec;
	for (int i = 0; i < numberOfValues; i++) {
		for (int j = 0; j < 3; j++) {
			total1 = to[0 + j] - from[0 + j];
			spacing1 = total1 / (numberOfValues - 1);
			vec[0 + j] = from[0 + j] + spacing1 * i;

		}
		vecOfVecs.push_back(vec);
	}

	return vecOfVecs;
}


void zDepthBufferInterpolations(CanvasPoint from, CanvasPoint to, Colour colour) {
	
	ModelTriangle currentTriangle3D;

	/*for (size_t i = 0; i < triangles3D.size(); i++) {
		currentTriangle3D = triangles3D[i];
		currentTriangle3D.vertices[i][2];
		;
		
	}*/
}

void triangleTextureMap() {

}

void drawLine(DrawingWindow& window, CanvasPoint from, CanvasPoint to, Colour colour, bool reset) {
	std::vector<float> zInterpolation;
	float toX = round(to.x);
	float fromX = round(from.x);
	float toY = round(to.y);
	float fromY = round(from.y);
	int xDiff = toX - fromX;
	int yDiff = toY - fromY;
	float numberOfSteps = std::max(std::abs(xDiff), std::abs(yDiff));
	float xStepSize = xDiff / numberOfSteps;
	float yStepSize = yDiff / numberOfSteps;
	uint32_t COLOUR = (0 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
	
	zInterpolation = interpolateSingleFloats(from.depth, to.depth, numberOfSteps+1);
	
	for (int i = 0; i <= numberOfSteps; i++) {
		CanvasPoint xy;

		if (numberOfSteps == 0) {
			xy.x = fromX;
			xy.y = fromY;
		}
		else {
			xy.x = fromX + (xStepSize * i);
			xy.y = fromY + (yStepSize * i);
		}

		xy.depth = zInterpolation[i];
		//std::cout << "z depth in drawLine func" << zInterpolation[i] << std::endl;

		
		if ((zBuffer[int(xy.x)][int(xy.y)] != 0) && (zBuffer[int(xy.x)][int(xy.y)] > 1/xy.depth) && (reset == false))
		{
			zBuffer[int(xy.x)][int(xy.y)] = 1/xy.depth;
			window.setPixelColour(xy.x, xy.y, COLOUR);
		} 
		else if ((zBuffer[int(xy.x)][int(xy.y)] == 0) && (reset == false)){
			zBuffer[int(xy.x)][int(xy.y)] = 1 / xy.depth;
			window.setPixelColour(xy.x, xy.y, COLOUR);
		}
		else if ((zBuffer[int(xy.x)][int(xy.y)] < 1/xy.depth) && (reset == false)) {
			//do nothing
		}
		else if (reset == true) {
			uint32_t BLACK = (255 << 24) + (0 << 16) + (0 << 8) + 0;
			window.setPixelColour(xy.x, xy.y, BLACK);
			zBuffer[int(xy.x)][int(xy.y)] = 0;
			
		}

		//window.setPixelColour(xy.x, xy.y, COLOUR);
	}
}

void drawStrokedTriangle(DrawingWindow& window, CanvasTriangle triangle, Colour colour) {
	//std::cout << triangle.v0();
	bool reset = false;
	drawLine(window, triangle.v0(), triangle.v1(), colour, reset);
	drawLine(window, triangle.v1(), triangle.v2(), colour, reset);
	drawLine(window, triangle.v2(), triangle.v0(), colour, reset);
}


void drawFilledTriangle(DrawingWindow& window, CanvasTriangle triangle, Colour colour) {
	/*drawLine(window, triangle.v0(), triangle.v1(), colour);
	drawLine(window, triangle.v1(), triangle.v2(), colour);
	drawLine(window, triangle.v2(), triangle.v0(), colour);*/

	//Step 1: segment triangle into two

	float maxYPoint = std::max({ triangle.v0().y,triangle.v1().y,triangle.v2().y });
	float minYPoint = std::min({ triangle.v0().y,triangle.v1().y,triangle.v2().y });

	
	CanvasPoint topPoint;
	CanvasPoint bottomPoint;
	CanvasPoint middlePoint;


	//finding middle then allocating top, middle and bottom points 
	if (triangle.v0().y != minYPoint && triangle.v0().y != maxYPoint) {
		middlePoint = triangle.v0();
		if (triangle.v1().y == maxYPoint) {
			bottomPoint = triangle.v1();
			topPoint = triangle.v2();
		}
		else {
			bottomPoint = triangle.v2();
			topPoint = triangle.v1();
		}
	}

	else if (triangle.v1().y != minYPoint && triangle.v1().y != maxYPoint) {
		middlePoint = triangle.v1();
		if (triangle.v0().y == maxYPoint) {
			bottomPoint = triangle.v0();
			topPoint = triangle.v2();
		}
		else {
			bottomPoint = triangle.v2();
			topPoint = triangle.v0();
		}
	}

	else if (triangle.v2().y != minYPoint && triangle.v2().y != maxYPoint) {
		middlePoint = triangle.v2();
		if (triangle.v0().y == maxYPoint) {
			bottomPoint = triangle.v0();
			topPoint = triangle.v1();
		}
		else if (triangle.v1().y == maxYPoint) {
			bottomPoint = triangle.v1();
			topPoint = triangle.v0();
		}
	}

	else {
		if (triangle.v0().y == triangle.v1().y ) { 
			middlePoint = triangle.v0();
			if (triangle.v2().y > triangle.v0().y) {
				bottomPoint = triangle.v2();
				topPoint = triangle.v1();
			}
			else {
				bottomPoint = triangle.v1();
				topPoint = triangle.v2();
			}
		}
		else if (triangle.v0().y == triangle.v2().y) {
			middlePoint = triangle.v0();
			if (triangle.v1().y > triangle.v0().y) {
				bottomPoint = triangle.v1();
				topPoint = triangle.v2();
			}
			else {
				bottomPoint = triangle.v2();
				topPoint = triangle.v1();
			}
		}
		else {
			middlePoint = triangle.v1();
			if (triangle.v1().y > triangle.v0().y) {
				topPoint = triangle.v0();
				bottomPoint = triangle.v2();
			}
			else {
				topPoint = triangle.v2();
				bottomPoint = triangle.v0();
			}
		}
	};

	/*std::cout << topPoint << std::endl;
	std::cout << middlePoint << std::endl;
	std::cout << bottomPoint << std::endl;*/

	//std::cout << topPoint.depth << " <- this the depth of top point" << std::endl;

	//Step 1.5: find gradient from top to bottom points to find x coordinate
	float gradient;
	float changeInX = bottomPoint.x - topPoint.x;
	gradient = (bottomPoint.y - topPoint.y) / (changeInX);
	float changeInY = bottomPoint.y - topPoint.y;
	float changeInZ = bottomPoint.depth - topPoint.depth;

	//prevents bugging out if grad = 0
	if (gradient == 0) {
		return;
	}

	//trying ratios instead
	float ratio = (bottomPoint.y - middlePoint.y)/changeInY;
	float newMidPointX = bottomPoint.x - changeInX*ratio;
	float newMidPointZ = bottomPoint.depth - changeInZ*ratio;
	//std::cout << "this the new one actualX2: " << newMidPointX << std::endl;
	
	float leftMidPoint;
	float rightMidPoint;
	float leftMidPointDepth;
	float rightMidPointDepth;

	//assigning left and right mid points
	if (newMidPointX > middlePoint.x) {
		leftMidPoint = middlePoint.x;
		rightMidPoint = newMidPointX;
		leftMidPointDepth = middlePoint.depth;
		rightMidPointDepth = newMidPointZ;
	}
	else {
		leftMidPoint = newMidPointX;
		rightMidPoint = middlePoint.x;
		leftMidPointDepth = newMidPointZ;
		rightMidPointDepth = middlePoint.depth;
	}

	//THIS IS THE RIGHT BIT MAN PLEASE HOLY PLEASE
	int changeInYTop = middlePoint.y - topPoint.y;
	int changeInYBot = bottomPoint.y - middlePoint.y;

	std::vector<float> topLeftXArray = interpolateSingleFloats(topPoint.x, leftMidPoint, changeInYTop);
	std::vector<float> topRightXArray = interpolateSingleFloats(topPoint.x, rightMidPoint, changeInYTop);
	std::vector<float> botLeftXArray = interpolateSingleFloats(leftMidPoint, bottomPoint.x, changeInYBot);
	std::vector<float> botRightXArray = interpolateSingleFloats(rightMidPoint, bottomPoint.x, changeInYBot);

	//maybe make the z arrays global? \/ \/ \/ 
	std::vector<float> topLeftZArray = interpolateSingleFloats(topPoint.depth, leftMidPointDepth, changeInYTop);
	std::vector<float> topRightZArray = interpolateSingleFloats(topPoint.depth, rightMidPointDepth, changeInYTop);
	std::vector<float> botLeftZArray = interpolateSingleFloats(leftMidPointDepth, bottomPoint.depth, changeInYBot);
	std::vector<float> botRightZArray = interpolateSingleFloats(rightMidPointDepth, bottomPoint.depth, changeInYBot);

	//for (size_t i = 0; i < topLeftZArray.size(); i++) { std::cout << "this the top left z array all elements: " << topLeftZArray[i] << std::endl; }

	//FILL TOP TRIANGLE
	bool reset = false;
	for (size_t i = 0; i < changeInYTop; i++) {

		CanvasPoint LineStart(round(topLeftXArray[i]), round((topPoint.y) + i), topLeftZArray[i]);
		CanvasPoint LineEnd(round(topRightXArray[i]), round((topPoint.y) + i), topRightZArray[i]);

		//zBuffer[][] = 

		
		drawLine(window, LineStart, LineEnd, colour, reset);
	}

	//FILL BOT TRIANGLE
	for (size_t i = 0; i < changeInYBot; i++) {

		CanvasPoint LineStart(botLeftXArray[i], (middlePoint.y) + i, botLeftZArray[i]);
		CanvasPoint LineEnd(botRightXArray[i], (middlePoint.y) + i, botRightZArray[i]);

		drawLine(window, LineStart, LineEnd, colour, reset);
	}

	CanvasPoint testLineStart;
	CanvasPoint testLineEnd;
	testLineStart.x = newMidPointX;
	testLineStart.y = middlePoint.y;
	testLineEnd.x = middlePoint.x;
	testLineEnd.y = middlePoint.y;

	//std::cout << "mid x point: ";
	//std::cout << newMidPointX;
}

int counter = 0;

void draw(DrawingWindow& window, float x, float y) {
	//window.clearPixels();
	size_t i = 0;
	glm::vec3 topLeft(255, 0, 0);        // red 
	glm::vec3 topRight(0, 0, 255);       // blue 
	glm::vec3 bottomRight(0, 255, 0);    // green 
	glm::vec3 bottomLeft(255, 255, 0);   // yellow

	CanvasPoint pointA;
	CanvasPoint pointB;
	pointA.x = 1;
	pointA.y = 5;
	pointB.x = 5;
	pointB.y = 10;

	//SET COLOUR HERE \/   \/  \/
	Colour colour = { 255,255,255 };
	uint32_t intColour = (255 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
	counter++;
	//std::cout << "somethings been drawn innit " << counter << std::endl;
	

	window.setPixelColour(x, y, intColour);

	//std::vector<float> gradient = interpolateSingleFloats(255, 0, window.width);									for black and white gradient
	std::vector<glm::vec3> leftColumn = interpolateThreeElementValues(topLeft, bottomLeft, window.height);
	std::vector<glm::vec3> rightColumn = interpolateThreeElementValues(topRight, bottomRight, window.height);

	//for (size_t y = 0; y < window.height; y++) {
	//	for (size_t x = 0; x < window.width; x++) {
	//		std::vector<glm::vec3> rowRGB = interpolateThreeElementValues(leftColumn[y], rightColumn[y], window.width);
	//		glm::vec3 currentRGB = rowRGB[x];
	//		float red = currentRGB[0];
	//		float green = currentRGB[1];
	//		float blue = currentRGB[2];
	//		uint32_t colour = (0 << 24) + (int(0) << 16) + (int(0) << 8) + int(0);
	//		/*uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);*/

	//		window.setPixelColour(x, y, colour);
	//	}
	//}
}


#endif 
