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
#include <algorithm>

#define WIDTH 960
#define HEIGHT 720

//#define WIDTH 320
//#define HEIGHT 240

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

void drawLine(DrawingWindow& window, CanvasPoint from, CanvasPoint to, Colour colour) {
	float toX = to.x;
	float fromX = from.x;
	float toY = to.y;
	float fromY = from.y;
	float xDiff = toX - fromX;
	float yDiff = toY - fromY;
	float numberOfSteps = std::max(std::abs(xDiff), std::abs(yDiff));
	float xStepSize = xDiff / numberOfSteps;
	float yStepSize = yDiff / numberOfSteps;
	uint32_t BLACK = (0 << 24) + (int(colour.red) << 16) + (int(colour.green) << 8) + int(colour.blue);
	for (float i = 0.0; i <= numberOfSteps; i++) {
		float x = fromX + (xStepSize * i);
		float y = fromY + (yStepSize * i);
		window.setPixelColour(round(x), round(y), BLACK);
	}
}

void drawStrokedTriangle(DrawingWindow& window, CanvasTriangle triangle, Colour colour) {
	//std::cout << triangle.v0();
	drawLine(window, triangle.v0(), triangle.v1(), colour);
	drawLine(window, triangle.v1(), triangle.v2(), colour);
	drawLine(window, triangle.v2(), triangle.v0(), colour);
}

void drawFilledTriangle(DrawingWindow& window, CanvasTriangle triangle, Colour colour) {
	drawLine(window, triangle.v0(), triangle.v1(), colour);
	drawLine(window, triangle.v1(), triangle.v2(), colour);
	drawLine(window, triangle.v2(), triangle.v0(), colour);
	//Step 1: segment triangle into two
	/*float maxXPoint = std::max({ triangle.v0().x,triangle.v1().x,triangle.v2().x });
	float minXPoint = std::min({ triangle.v0().x,triangle.v1().x,triangle.v2().x });*/

	float maxYPoint = std::max({ triangle.v0().y,triangle.v1().y,triangle.v2().y });
	float minYPoint = std::min({ triangle.v0().y,triangle.v1().y,triangle.v2().y });

	CanvasPoint topPoint;
	CanvasPoint bottomPoint;
	CanvasPoint middlePoint;

	std::cout << triangle.v0() << std::endl;
	std::cout << triangle.v1() << std::endl;
	std::cout << triangle.v2() << std::endl;


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

	else {
		middlePoint = triangle.v2();
		if (triangle.v0().y == maxYPoint) {
			bottomPoint = triangle.v0();
			topPoint = triangle.v1();
		}
		else {
			bottomPoint = triangle.v1();
			topPoint = triangle.v0();
		}
	};
	std::cout << topPoint << std::endl;
	std::cout << middlePoint << std::endl;
	std::cout << bottomPoint << std::endl;

	//Step 1.5: find gradient from top to bottom points to find x coordinate
	float gradient;
	float changeInX = bottomPoint.x - topPoint.x;
	gradient = (bottomPoint.y - topPoint.y) / (changeInX);

	if (gradient == 0) {
		return;
	}
	float midXPoint = (middlePoint.y - topPoint.y) / gradient;
	float actualX = midXPoint + topPoint.x;

	CanvasPoint lineStart;
	CanvasPoint lineEnd;
	lineStart.x = actualX;
	lineStart.y = middlePoint.y;
	lineEnd.x = middlePoint.x;
	lineEnd.y = middlePoint.y;

	std::cout << "mid x point: ";
	std::cout << actualX;
	Colour WHITE(255, 255, 255);

	if (middlePoint.y == topPoint.y) {
		drawLine(window, middlePoint, topPoint, WHITE);
	}
	else if (middlePoint.y == bottomPoint.y){
		drawLine(window, middlePoint, bottomPoint, WHITE);
	}
	else {
		drawLine(window, lineStart, lineEnd, WHITE);
	}
}
//test for git

void draw(DrawingWindow& window) {
	window.clearPixels();
	size_t i = 0;
	glm::vec3 topLeft(255, 0, 0);        // red 
	glm::vec3 topRight(0, 0, 255);       // blue 
	glm::vec3 bottomRight(0, 255, 0);    // green 
	glm::vec3 bottomLeft(255, 255, 0);   // yellow
	//std::vector<float> gradient = interpolateSingleFloats(255, 0, window.width);									for black and white gradient
	std::vector<glm::vec3> leftColumn = interpolateThreeElementValues(topLeft, bottomLeft, window.height);
	std::vector<glm::vec3> rightColumn = interpolateThreeElementValues(topRight, bottomRight, window.height);

	
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			std::vector<glm::vec3> rowRGB = interpolateThreeElementValues(leftColumn[y], rightColumn[y], window.width);
			glm::vec3 currentRGB = rowRGB[x];
			float red = currentRGB[0];
			float green = currentRGB[1];
			float blue = currentRGB[2];
			uint32_t colour = (0 << 24) + (int(0) << 16) + (int(0) << 8) + int(0);
			/*uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);*/

			window.setPixelColour(x, y, colour);
		}
	}
}

void handleEvent(SDL_Event event, DrawingWindow& window) {
	CanvasTriangle triangle(CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height), CanvasPoint(rand() % window.width, rand() % window.height));
	//CanvasTriangle triangle(CanvasPoint(500,700),CanvasPoint(400,200),CanvasPoint(100,700));
	Colour COLOUR = Colour(rand()%255, rand()%255, rand()%255);

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
		else if (event.key.keysym.sym == SDLK_u) {
			std::cout << "u" << std::endl;
			drawStrokedTriangle(window, triangle, COLOUR);
		}
		else if (event.key.keysym.sym == SDLK_f) {
			std::cout << "top middle then bottom points" << std::endl;
			drawFilledTriangle(window, triangle, COLOUR);
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char* argv[]) {
	//Ensuring randomness
	srand((unsigned int)time(NULL));
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	std::vector<float> result;
	std::vector<glm::vec3> result2;
	glm::vec3 from(1.0, 4.0, 9.2);
	glm::vec3 to(4.0, 1.0, 9.8);
	
	CanvasTriangle triangle(CanvasPoint(rand()%window.width,rand()%window.height), CanvasPoint(rand()%window.width,rand()%window.height), CanvasPoint(rand()%window.width,rand()%window.height));
	Colour BLACK = Colour(0, 0, 0);

	/*result2 = interpolateThreeElementValues(from, to, 4);
	result = interpolateSingleFloats(2.2, 8.5, 7);*/
	/*for (size_t i = 0; i < result.size(); i++) std::cout << result[i] << " ";
	std::cout << std::endl;
	for (size_t i = 0; i < result2.size(); i++) std::cout << glm::to_string(result2[i]) << " ";
	std::cout << std::endl;*/
	
	//drawLine(window, CanvasPoint(0,0), CanvasPoint(window.width/2,window.height/2), BLACK);



	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		//draw(window);
		//drawStrokedTriangle(window, triangle, BLACK);
		/*drawLine(window, CanvasPoint(0.0, 0.0), CanvasPoint(window.width / 2, window.height / 2), BLACK);
		drawLine(window, CanvasPoint(window.width/3,window.height/2), CanvasPoint(window.width*2 / 3, window.height/2), BLACK);
		drawLine(window, CanvasPoint(window.width/2,window.height/2), CanvasPoint(window.width, 0), BLACK);
		drawLine(window, CanvasPoint(window.width/2,0), CanvasPoint(window.width / 2, window.height), BLACK);*/
		 //Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
