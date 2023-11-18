#include"Triangle.h"
#include<Colour.h>
#include<algorithm>
#include <glm/ext.hpp>

#define WIDTH 320
#define HEIGHT 240

std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues) {
	float total = 0;
	float spacing;
	int i = 0;
	std::vector<float> evenlySpacedVec;
	total = to - from;
	spacing = total / (numberOfValues - 1);

	if (numberOfValues == 1) {
		evenlySpacedVec.push_back(from);
		return evenlySpacedVec;
	}

	for (int i = 0; i < numberOfValues; i++) {
		evenlySpacedVec.push_back(round(from + spacing * i));
	}

	return evenlySpacedVec;
}


bool compareY(CanvasPoint u, CanvasPoint v) { return (u.y < v.y); }
std::vector<CanvasPoint> orderByIncrY(CanvasTriangle triangle) {

	std::vector<CanvasPoint> points = { triangle.v0(), triangle.v1(), triangle.v2() };
	
	std::sort(points.begin(), points.end(), compareY);

	return points;

}

void drawLine(CanvasPoint from, CanvasPoint to, Colour colour, DrawingWindow &window) {
	float changeInY = round(to.y) - round(from.y);
	float changeInX = round(to.x) - round(from.x);
	float numberOfSteps = std::max(std::abs(changeInY), std::abs(changeInX));
	float toX = round(to.x);
	float fromX = round(from.x);
	float toY = round(to.y);
	float fromY = round(from.y);

	uint32_t colourUint32 = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
	std::vector<float> xInterpolation = interpolateSingleFloats(fromX, toX, numberOfSteps+1);
	std::vector<float> yInterpolation = interpolateSingleFloats(fromY, toY, numberOfSteps+1);


	float xStepSize = changeInX / numberOfSteps;
	float yStepSize = changeInY / numberOfSteps;


	float x;
	float y;

	if (numberOfSteps == 0) {
		x = fromX;
		y = fromY;
	}
	for (size_t i = 0; i < numberOfSteps; i++) {
		x = from.x + (xStepSize * i);
		y = from.y + (yStepSize * i);
		window.setPixelColour(x, y, colourUint32);
	}
}

void drawStrokedTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow &window) {
	drawLine(triangle.v0(),triangle.v1(), colour, window);
	drawLine(triangle.v1(), triangle.v2(), colour, window);
	drawLine(triangle.v2(), triangle.v0(), colour, window);
}

void drawFilledTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow &window){
	std::vector<CanvasPoint> incrYPoints = orderByIncrY(triangle);
	
	
	float changeInY = incrYPoints[2].y - incrYPoints[0].y;
	float changeInX = incrYPoints[2].x - incrYPoints[0].x;
	float ratio = (incrYPoints[2].y - incrYPoints[1].y) / changeInY;

	CanvasPoint newMidPoint;
	newMidPoint.y = incrYPoints[1].y;
	newMidPoint.x = incrYPoints[2].x - changeInX * ratio;

	CanvasPoint leftMidPoint;
	CanvasPoint rightMidPoint;
	//if (newMidPoint.x < 0) std::cout << newMidPoint << '\n';

	if (newMidPoint.x > incrYPoints[1].x) {
		leftMidPoint = incrYPoints[1];
		rightMidPoint = newMidPoint;
	}
	else {
		leftMidPoint = newMidPoint;
		rightMidPoint = incrYPoints[1];
	}

	float changeInYTop = incrYPoints[1].y - incrYPoints[0].y;
	float changeInYBot = incrYPoints[2].y - incrYPoints[1].y;

	std::vector<float> topLeftXArray = interpolateSingleFloats(incrYPoints[0].x, leftMidPoint.x, changeInYTop);
	std::vector<float> topRightXArray = interpolateSingleFloats(incrYPoints[0].x, rightMidPoint.x, changeInYTop);
	std::vector<float> botLeftXArray = interpolateSingleFloats(leftMidPoint.x, incrYPoints[2].x, changeInYBot);
	std::vector<float> botRightXArray = interpolateSingleFloats(rightMidPoint.x, incrYPoints[2].x, changeInYBot);

	for (size_t i = 0; i < changeInYTop; i++) {
		//std::cout << topLeftXArray.size()<<" "<<topRightXArray.size()<<" " <<i<<" "<<changeInYTop<< std::endl;
		
		CanvasPoint lineStart(topLeftXArray[i],incrYPoints[0].y + i);
		CanvasPoint lineEnd(topRightXArray[i], incrYPoints[0].y + i);

		drawLine(lineStart, lineEnd, colour, window);
	}

	for (size_t i = 0; i < changeInYBot; i++) {
	//	std::cout << " this the bot triangle " << botLeftXArray.size() << " " << botRightXArray.size() << " " << i << " " << changeInYBot << std::endl;

		CanvasPoint lineStart(botLeftXArray[i], incrYPoints[1].y + i);
		CanvasPoint lineEnd(botRightXArray[i], incrYPoints[1].y + i);

		drawLine(lineStart, lineEnd, colour, window);
	}
}

CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::vec3 vertexPosition, float focalLength) {
	CanvasPoint intersectPoint;
	float scalingFactor = 120;

	//std::cout << "vertex position" << glm::to_string(vertexPosition) << std::endl;

	glm::vec3 dist = cameraPosition - vertexPosition;

	intersectPoint.x = round((focalLength * ((-dist.x) / dist.z)) * scalingFactor + (WIDTH/2));
	intersectPoint.y = round((focalLength * ((dist.y) / dist.z)) * scalingFactor + (HEIGHT/2));

	//std::cout << " intersection point" << intersectPoint << std::endl;

	return intersectPoint;
};

