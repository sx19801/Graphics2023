#include "textureMapping.h"

int textureXYToVectorLocation(CanvasPoint xy, int width) {
	return (xy.texturePoint.y * width)+xy.texturePoint.x;
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


void loadTextureMap(CanvasTriangle& triangle) {
	TextureMap textureMap = TextureMap("../../../OBJFiles/texture.ppm");
	
	//for (size_t i = 0; i < TextureMap("../../../OBJFiles/texture.ppm").pixels.size(); i++) std::cout << TextureMap("../../../OBJFiles/texture.ppm").pixels.size() << '\n';

	//would usually need to multiply u and v by Width and height to get xy for texture but already have atm

	CanvasPoint a = triangle.v0();
	CanvasPoint b = triangle.v1();
	CanvasPoint c = triangle.v2();


	int minX = std::min({ a.x,b.x,c.x });
	int minY = std::min({ a.y, b.y, c.y });
	int maxX = std::max({ a.x,b.x,c.x });
	int maxY = std::max({ a.y, b.y, c.y });

	//bounding box

	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			//calculate barycentric of pixel 
			CanvasPoint XY(x,y);
			glm::vec3 bary = barycentricCoords(XY, a, b, c);
			if (bary.x >= 0 && bary.y >= 0 && bary.z >= 0) {
				//pixel inside triangle 
			}

		}
	}

}