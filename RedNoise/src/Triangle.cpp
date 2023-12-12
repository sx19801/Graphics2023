#include"Triangle.h"
#include<Colour.h>

#include <glm/ext.hpp>

//#define WIDTH 960
//#define HEIGHT 720

//float zBuffer[WIDTH][HEIGHT] = {}; //actually -1 on both the row and column since starts at 0

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

	for (int i = 0; i <= numberOfValues; i++) {
		evenlySpacedVec.push_back((from + spacing * i));
	}

	return evenlySpacedVec;
}

std::vector<CanvasPoint> interpolateSinglePoints(CanvasPoint from, CanvasPoint to, int numberOfValues) {
	float totalX;
	float spacingX;
	float totalZ;
	float spacingZ;
	int i = 0;
	CanvasPoint newPoint;
	std::vector<CanvasPoint> evenlySpacedVec;
	totalX = to.x - from.x;
	totalZ = to.depth - from.depth;
	spacingX = totalX / (numberOfValues - 1);
	spacingZ = totalZ / (numberOfValues - 1);

	if (numberOfValues == 1) {
		evenlySpacedVec.push_back(from);
		return evenlySpacedVec;
	}

	for (int i = 0; i <= numberOfValues; i++) {
		newPoint = { round(from.x + spacingX * i), from.y, from.depth + spacingZ * i };
		evenlySpacedVec.push_back(newPoint);
	}

	return evenlySpacedVec;
}

bool compareY(CanvasPoint u, CanvasPoint v) { return (u.y < v.y); }

bool compareX(CanvasPoint u, CanvasPoint v) { return (u.x < v.x); }

std::vector<CanvasPoint> orderByIncrY(CanvasTriangle triangle) {

	std::vector<CanvasPoint> points = { triangle.v0(), triangle.v1(), triangle.v2() };

	std::sort(points.begin(), points.end(), compareY);

	return points;

}

std::vector<CanvasPoint> orderByIncrX(CanvasTriangle triangle) {

	std::vector<CanvasPoint> points = { triangle.v0(), triangle.v1(), triangle.v2() };

	std::sort(points.begin(), points.end(), compareX);

	return points;

}

void zDepthCheck(CanvasPoint xy, float z, std::vector<std::vector<float>>& zBuffer, Uint32 colourUint32, DrawingWindow& window) {
	
	if (zBuffer[xy.x][xy.y] == 0) {
		zBuffer[xy.x][xy.y] = z;
		window.setPixelColour((xy.x), (xy.y), colourUint32);
		//window.renderFrame();
	}
	else {
		if (zBuffer[xy.x][xy.y] < z) {
			zBuffer[xy.x][xy.y] = z;
			window.setPixelColour((xy.x), (xy.y), colourUint32);
			//window.renderFrame();
		}
		else {
			
		}
	}
}

void resetDepthBuffer(DrawingWindow& window, std::vector<std::vector<float>>& zBuffer) {
	window.clearPixels();
	//window.renderFrame();
	for (size_t i = 0; i < WIDTH; i++) {
		for (size_t j = 0; j < HEIGHT; j++) {
			zBuffer[i][j] = 0; 			
		}
	}
}

bool compareOnScreen(float u, float v) { 
	if ((u < WIDTH-1) && (v < HEIGHT-1) && (u >= 0) && (v >= 0)) {
		return true;
	}
	else { return false; }
}

void drawLine(CanvasPoint from, CanvasPoint to, Colour colour, DrawingWindow& window, std::vector<std::vector<float>>& zBuffer) {
	//if (!(colour.name == "Red" || colour.name == "Yellow")) return;
	
	float toX = (to.x);
	float fromX = (from.x);
	float toY = (to.y);
	float fromY = (from.y);
	float changeInY = toY - fromY;
	float changeInX = toX - fromX;
	float changeInZ = (to.depth) - (from.depth);
	float numberOfSteps = std::max(std::abs(changeInY), std::abs(changeInX));

	float toZ = to.depth;
	float fromZ = from.depth;
	//std::cout << "z : " << toZ << std::endl;

	uint32_t colourUint32 = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
	//std::vector<float> xInterpolation = interpolateSingleFloats(fromX, toX, numberOfSteps+1);
	//std::vector<float> yInterpolation = interpolateSingleFloats(fromY, toY, numberOfSteps+1);
	//std::vector<float> zInterpolation = interpolateSingleFloats(fromZ, toZ, numberOfSteps + 1);



	float xStepSize = changeInX / numberOfSteps;
	float yStepSize = changeInY / numberOfSteps;
	float zStepSize = changeInZ / numberOfSteps;

	CanvasPoint xy;
	
	if (numberOfSteps == 0) {
		xy.x = fromX;
		xy.y = fromY;
		xy.depth = fromZ;
	}
	else {
		for (size_t i = 0; i <= numberOfSteps+1; i++) {
			
			
			xy.x = from.x + (xStepSize * i);
			xy.y = from.y + (yStepSize * i);
			xy.depth = from.depth + (zStepSize * i);

			


			//std::cout << "depth per step : " << xy.depth << " at i : "<< i <<  std::endl;
			
			if (compareOnScreen(xy.x, xy.y) == true){
				zDepthCheck(xy,xy.depth, zBuffer, colourUint32, window);
				
				//window.setPixelColour(round(xy.x), round(xy.y), colourUint32);
					//window.renderFrame();
				
			}


			//window.setPixelColour(xy.x, xy.y, colourUint32);

		}

		//if (compareOnScreen(xy.x,xy.y)){
		//	if (zDepthCheck(round(xy.x), round(xy.y), xy.depth) == true) {
		//		window.setPixelColour(round(xy.x), round(xy.y), colourUint32);
		//		//window.renderFrame();
		//	}
		//}

	}
}

void drawStrokedTriangle(CanvasTriangle& triangle, Colour colour, DrawingWindow& window, std::vector<std::vector<float>>& zBuffer) {
	drawLine(triangle.v0(), triangle.v1(), colour, window, zBuffer);
	drawLine(triangle.v1(), triangle.v2(), colour, window, zBuffer);
	drawLine(triangle.v2(), triangle.v0(), colour, window, zBuffer);
	//std::cout << triangle.v0() << " " << triangle.v1() << " " << triangle.v2() << std::endl;
}

void drawFilledTriangle(CanvasTriangle triangle, Colour colour, DrawingWindow& window, std::vector<std::vector<float>>& zBuffer) {
	std::vector<CanvasPoint> incrYPoints = orderByIncrY(triangle);

	
	/*for (size_t i = 0; i < incrYPoints.size(); i++) {
		std::cout << incrYPoints[0].y << " " << incrYPoints[1].y << " " << incrYPoints[2].y << std::endl;
	}*/

	/*drawLine(triangle.v0(), triangle.v1(), colour, window);
	drawLine(triangle.v1(), triangle.v2(), colour, window);
	drawLine(triangle.v2(), triangle.v0(), colour, window);*/

	

	////std::cout << "z : " << triangle.v1().depth << std::endl;
	//float changeInY = incrYPoints[2].y - incrYPoints[0].y;
	//float changeInX = incrYPoints[2].x - incrYPoints[0].x;
	//float changeInZ = incrYPoints[2].depth - incrYPoints[0].depth;

	//float ratio = (incrYPoints[2].y - incrYPoints[1].y) / changeInY;

	//CanvasPoint newMidPoint;
	//newMidPoint.y = incrYPoints[1].y;
	//newMidPoint.x = incrYPoints[2].x - changeInX * ratio;
	//newMidPoint.depth = incrYPoints[2].depth - changeInZ * ratio;

	//CanvasPoint leftMidPoint;
	//CanvasPoint rightMidPoint;
	////if (newMidPoint.x < 0) std::cout << newMidPoint << '\n';




	//if (newMidPoint.x > incrYPoints[1].x) {
	//	leftMidPoint = incrYPoints[1];
	//	rightMidPoint = newMidPoint;
	//}
	//else {
	//	leftMidPoint = newMidPoint;
	//	rightMidPoint = incrYPoints[1];
	//}

	//float changeInYTop = (incrYPoints[1].y - incrYPoints[0].y);
	//float changeInYBot = (incrYPoints[2].y - incrYPoints[1].y);


	//std::vector<CanvasPoint> topLeftArray = interpolateSinglePoints(incrYPoints[0], leftMidPoint, changeInYTop);
	//std::vector<CanvasPoint> topRightArray = interpolateSinglePoints(incrYPoints[0], rightMidPoint, changeInYTop);
	//std::vector<CanvasPoint> botLeftArray = interpolateSinglePoints(leftMidPoint, incrYPoints[2], changeInYBot);
	//std::vector<CanvasPoint> botRightArray = interpolateSinglePoints(rightMidPoint, incrYPoints[2], changeInYBot);

	//for (size_t i = 0; i < changeInYTop; i++) {
	//	//std::cout << topLeftXArray.size()<<" "<<topRightXArray.size()<<" " <<i<<" "<<changeInYTop<< std::endl;

	//	CanvasPoint lineStart((topLeftArray[i].x), (incrYPoints[0].y + i), topLeftArray[i].depth);
	//	CanvasPoint lineEnd((topRightArray[i].x), (incrYPoints[0].y + i), topRightArray[i].depth);

	//	drawLine(lineStart, lineEnd, colour, window, zBuffer);
	//	//window.renderFrame();
	//}

	//for (size_t i = 0; i < changeInYBot; i++) {
	//	//	std::cout << " this the bot triangle " << botLeftXArray.size() << " " << botRightXArray.size() << " " << i << " " << changeInYBot << std::endl;

	//	CanvasPoint lineStart((botLeftArray[i].x), (incrYPoints[1].y) + i, botLeftArray[i].depth);
	//	CanvasPoint lineEnd((botRightArray[i].x), (incrYPoints[1].y) + i, botRightArray[i].depth);

	//	drawLine(lineStart, lineEnd, colour, window, zBuffer);
	//	//window.renderFrame();
	//}


	

	//USING BARYCENTRIC COORDS
	
	int minX = std::min({ triangle.v0().x,triangle.v1().x, triangle.v2().x});
	int minY = std::min({ triangle.v0().y,triangle.v1().y, triangle.v2().y });
	int maxX = std::max({ triangle.v0().x,triangle.v1().x, triangle.v2().x });
	int maxY = std::max({ triangle.v0().y,triangle.v1().y, triangle.v2().y });

	//std::cout << minX << " " << minY << " " << maxX << " " << maxY << '\n';

	////bounding box
	//Uint32 colourUint32 = (255 << 24) + (255 << 16) + (255 << 8) + 255;
	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			//calculate barycentric of pixel 
			CanvasPoint XY(x, y);
			glm::vec3 bary = barycentricCoords(XY, triangle.v0(), triangle.v1(), triangle.v2());
			if (bary.x >= 0 && bary.y >= 0 && bary.z >= 0) {
				//pixel inside triangle 
				float u = (triangle.v0().x * bary.x + triangle.v1().x * bary.y + triangle.v2().x * bary.z);
				float v = (triangle.v0().y * bary.x + triangle.v1().y * bary.y + triangle.v2().y * bary.z);

				float z = (1.0f/triangle.v0().depth) * bary.x + (1.0f/triangle.v1().depth) * bary.y + (1.0f/triangle.v2().depth) * bary.z;
				//int u = round(a.x * bary.x + b.x * bary.y + c.x * bary.z);
				//int v = round(a.y * bary.x + b.y * bary.y + c.y * bary.z);
				z = 1 / z;
				//std::cout << glm::to_string(bary) << '\n';

				//std::cout << u << " " << v << '\n';
				//window.setPixelColour(u, v, colourUint32);
				//window.renderFrame();

				
				//std::cout << textureVectorIndex << '\n';
				
				Uint32 colourUint32 = convColourToUint32(colour);

				if (!((x < 0) || (x >= window.width) || (y < 0) || (y >= window.height))) {

					//window.setPixelColour(x, y, RGBInteger);
					zDepthCheck(XY, z, zBuffer, colourUint32, window);
				}


			}

		}
		//window.renderFrame();
	}




}

CanvasPoint getCanvasIntersectionPoint(glm::vec3 cameraPosition, glm::mat3 cameraOrientation, glm::vec3 vertexPosition, float focalLength) {
	CanvasPoint intersectPoint;
	//std::cout << "vertex position" << glm::to_string(vertexPosition) << std::endl;#

	//z depth buffer check here???


	glm::vec3 dist = vertexPosition - cameraPosition;
	
	glm::vec3 adjustedVector = dist* cameraOrientation;


	intersectPoint.x = (focalLength * (-(adjustedVector.x) / adjustedVector.z));
	intersectPoint.y = (focalLength * ((adjustedVector.y) / adjustedVector.z));

	//float distZ = glm::distance(vertexPosition.z - camerPosition);
	//float zDistance = sqrt((pow(cameraPosition.x- vertexPosition.x, 2)+pow(cameraPosition.y - vertexPosition.y,2)+pow(cameraPosition.z - vertexPosition.z,2)));
	
	intersectPoint.depth = adjustedVector.z;				//COULD BE WRONG TO DO INVERSE AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
	//intersectPoint.depth = zDistance;

	return intersectPoint;
};



