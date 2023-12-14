#include "textureMapping.h"

int textureXYToVectorLocation(CanvasPoint xy, int width, int height) {
	xy.y = round(xy.y*height);
	xy.x = round(xy.x*width);
	return (xy.y * width)+xy.x;
}

float edgeFunction(CanvasPoint& a, CanvasPoint& b, CanvasPoint& c) {
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}
glm::vec3 barycentricCoords(CanvasPoint& xy, CanvasPoint& a, CanvasPoint& b, CanvasPoint& c) {
	float areaABC = edgeFunction(a,b,c);
	float areaXYBC = edgeFunction(b, c, xy);
	float areaXYCA = edgeFunction(c, a, xy);

	float u = areaXYBC / areaABC;
	float v = areaXYCA / areaABC;
	float w = 1 - u - v;

	return glm::vec3(u, v, w);
}

void loadTextureMap(CanvasTriangle& triangle, DrawingWindow& window, std::vector<std::vector<float>>& zBuffer) {
	TextureMap textureMap = TextureMap("../objFiles/texture.ppm");
	
	//for (size_t i = 0; i < TextureMap("../../../OBJFiles/texture.ppm").pixels.size(); i++) std::cout << TextureMap("../../../OBJFiles/texture.ppm").pixels.size() << '\n';

	//would usually need to multiply u and v by Width and height to get xy for texture but already have atm

	CanvasPoint a = triangle.v0();
	CanvasPoint b = triangle.v1();
	CanvasPoint c = triangle.v2();

	//std::cout << a.texturePoint << " " << b.texturePoint << " " << c.texturePoint << '\n';
	//std::cout << textureMap.height << " " << textureMap.width << '\n';

	int minX = std::min({ a.x,b.x,c.x });
	int minY = std::min({ a.y, b.y, c.y });
	int maxX = std::max({ a.x,b.x,c.x });
	int maxY = std::max({ a.y, b.y, c.y });

	//std::cout << minX << " " << minY << " " << maxX << " " << maxY << '\n';
	
	//bounding box
	Uint32 colourUint32 = (255 << 24) + (255 << 16) + (255 << 8) + 255;
	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			//calculate barycentric of pixel 
			CanvasPoint XY(x,y);
			glm::vec3 bary = barycentricCoords(XY, a, b, c);
			if (bary.x >= 0 && bary.y >= 0 && bary.z >= 0) {
				//pixel inside triangle 
				float u = (a.texturePoint.x * bary.x + b.texturePoint.x * bary.y + c.texturePoint.x * bary.z);
				float v = (a.texturePoint.y * bary.x + b.texturePoint.y * bary.y + c.texturePoint.y * bary.z);

				float z = a.depth * bary.x + b.depth * bary.y + c.depth * bary.z;
				//int u = round(a.x * bary.x + b.x * bary.y + c.x * bary.z);
				//int v = round(a.y * bary.x + b.y * bary.y + c.y * bary.z);

				//std::cout << glm::to_string(bary) << '\n';

				//std::cout << u << " " << v << '\n';
				//window.setPixelColour(u, v, colourUint32);
				//window.renderFrame();

				int textureVectorIndex = textureXYToVectorLocation(CanvasPoint(u, v), textureMap.width, textureMap.height);
				//std::cout << textureVectorIndex << '\n';
				Uint32 RGBInteger = textureMap.pixels[textureVectorIndex];
				

				if (!((x < 0) || (x >= window.width) || (y < 0) || (y >= window.height))) {
					
					//window.setPixelColour(x, y, RGBInteger);
					zDepthCheck(XY,z, zBuffer, RGBInteger, window);
				}


			}

		}
	}
	//window.renderFrame();
}