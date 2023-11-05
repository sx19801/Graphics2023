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


float zBuffer[960][720] = {};


std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues) {
	float total = 0;
	float spacing;
	int i = 0;
	std::vector<float> evenVector;
	total = to - from;
	spacing = total / (numberOfValues - 1);

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

void drawLine(DrawingWindow& window, CanvasPoint from, CanvasPoint to, Colour colour) {
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
 		xy.x = fromX + (xStepSize * i);
		xy.y = fromY + (yStepSize * i);


		xy.depth = zInterpolation[i];

		//if (zBuffer[int(xy.x)][int(xy.y)] < 1/xy.depth)
		//{
		//	zBuffer[int(xy.x)][int(xy.y)] = 1/xy.depth;
		//	window.setPixelColour(xy.x, xy.y, COLOUR);
		//}
		//else if (zBuffer[int(xy.x)][int(xy.y)] > 1/xy.depth) {
		//	//do nothing
		//}

		window.setPixelColour(xy.x, xy.y, COLOUR);
	}
}

void drawStrokedTriangle(DrawingWindow& window, CanvasTriangle triangle, Colour colour) {
	//std::cout << triangle.v0();
	drawLine(window, triangle.v0(), triangle.v1(), colour);
	drawLine(window, triangle.v1(), triangle.v2(), colour);
	drawLine(window, triangle.v2(), triangle.v0(), colour);
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

	std::cout << topPoint.depth << " <- this the depth of top point" << std::endl;

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
	float actualX2 = bottomPoint.x - changeInX*ratio;

	//use ratio to calulate the z of new middle point
	float actualZ = bottomPoint.depth - changeInZ * ratio;

	std::cout << "this the new one actualX2: " << actualX2 << std::endl;
	std::cout << "this the z new mid point: " << actualZ << std::endl;

	//


	
	//this old shit
	float midXPoint = (middlePoint.y - topPoint.y) / gradient;
	float actualX = midXPoint + topPoint.x;
	
	float leftMidPoint;
	float rightMidPoint;

	std::cout << "this the old one actualX: " << actualX << std::endl;

	//assigning left and right mid points
	if (actualX2 > middlePoint.x) {
		leftMidPoint = middlePoint.x;
		rightMidPoint = actualX2;
	}
	else {
		leftMidPoint = actualX2;
		rightMidPoint = middlePoint.x;
	}

	//THIS IS THE RIGHT BIT MAN PLEASE HOLY PLEASE
	int changeInYTop = middlePoint.y - topPoint.y;
	int changeInYBot = bottomPoint.y - middlePoint.y;

	std::vector<float> topLeftXArray = interpolateSingleFloats(topPoint.x, leftMidPoint, changeInYTop);
	std::vector<float> topRightXArray = interpolateSingleFloats(topPoint.x, rightMidPoint, changeInYTop);
	std::vector<float> botLeftXArray = interpolateSingleFloats(leftMidPoint, bottomPoint.x, changeInYBot);
	std::vector<float> botRightXArray = interpolateSingleFloats(rightMidPoint, bottomPoint.x, changeInYBot);

	std::vector<float> topLeftZArray = interpolateSingleFloats(topPoint.depth, middlePoint.depth, changeInYTop);
	std::vector<float> topRightZArray = interpolateSingleFloats(topPoint.depth, middlePoint.depth, changeInYTop);

	//FILL TOP TRIANGLE
	for (size_t i = 0; i < changeInYTop; i++) {

		CanvasPoint LineStart(round(topLeftXArray[i]), round((topPoint.y) + i));
		CanvasPoint LineEnd(round(topRightXArray[i]), round((topPoint.y) + i));

		

		drawLine(window, LineStart, LineEnd, colour);
	}

	//FILL BOT TRIANGLE
	for (size_t i = 0; i < changeInYBot; i++) {

		CanvasPoint LineStart(botLeftXArray[i], (middlePoint.y) + i);
		CanvasPoint LineEnd(botRightXArray[i], (middlePoint.y) + i);

		drawLine(window, LineStart, LineEnd, colour);
	}

	CanvasPoint testLineStart;
	CanvasPoint testLineEnd;
	testLineStart.x = actualX;
	testLineStart.y = middlePoint.y;
	testLineEnd.x = middlePoint.x;
	testLineEnd.y = middlePoint.y;

	std::cout << "mid x point: ";
	std::cout << actualX;
	Colour WHITE(255, 255, 255);

	/*drawLine(window, triangle.v0(), triangle.v1(), WHITE);
	drawLine(window, triangle.v1(), triangle.v2(), WHITE);
	drawLine(window, triangle.v2(), triangle.v0(), WHITE);*/
	//filling top triangle 

	float difInY = middlePoint.y - topPoint.y;

	//testing interpolation
	glm::vec3 interpTopPoint(topPoint.x, topPoint.y, 0);
	glm::vec3 interpLeftMidPoint(leftMidPoint, middlePoint.y, 0);
	glm::vec3 interpRightMidPoint(rightMidPoint, middlePoint.y, 0);

	std::vector<glm::vec3> leftLineResult = interpolateThreeElementValues(interpTopPoint, interpLeftMidPoint, difInY);
	std::vector<glm::vec3> rightLineResult = interpolateThreeElementValues(interpTopPoint, interpRightMidPoint, difInY);
	std::vector<glm::vec3> leftArrayNoDupes;
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
