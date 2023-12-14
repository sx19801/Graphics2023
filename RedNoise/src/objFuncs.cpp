#include "objFuncs.h"
#include "rayTracing.h"


glm::vec3 convMtlToInt(glm::vec3 colourValues) {
	glm::vec3 RGBColours;

	RGBColours.r = round(colourValues.r * 255);
	RGBColours.b = round(colourValues.b * 255);
	RGBColours.g = round(colourValues.g * 255);

	return RGBColours;
}

std::unordered_map<std::string, glm::vec4> loadMatOBJ() {
	std::unordered_map<std::string, glm::vec4> materialMap;						//initialise hashmap for colour

	std::string myText;
	std::ifstream MyReadFile("../objFiles/textured-cornell-box.mtl");
	Colour colour;
	std::string name;


	while (getline(MyReadFile, myText)) {
		std::stringstream ss(myText);
		std::string identifier;
		std::vector<Colour> colours;


		ss >> identifier;

		if (identifier == "newmtl") {
			std::string a;
			ss >> a;
			name = a;
			materialMap[a] = {};

		}
		else if (identifier == "Kd") {
			float a, b, c;
			ss >> a >> b >> c;

			glm::vec3 RGBColours = convMtlToInt({ a,b,c });

			materialMap[name] = { RGBColours.r, RGBColours.g, RGBColours.b, 0.0f};
			

			//std::cout << "AAHHHHHHHHHH " << materialMap[name].r << " " << materialMap[name].g << " " << materialMap[name].b << " " << materialMap[name].w << " " << '\n';

		}
		else if (identifier == "map_Kd") {
			std::string textureFileName;
			ss >> textureFileName;

			materialMap[name].w = 1.0f;


		}

	}
	return materialMap;
}

Uint32 convColourToUint32(Colour colour) {
	return (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;
}

std::vector<ModelTriangle> loadGeoOBJ(float scalingFactor, DrawingWindow& window) {
	std::string myText;
	std::ifstream MyReadFile("../objFiles/textured-cornell-box.obj");
	std::ifstream MyReadFileSphere("../objFiles/sphere.obj");


	Colour rgbColour;
	ModelTriangle modelTriangle;
	std::vector<ModelTriangle> modelTriangles;

	glm::vec3 trianglePoints;
	std::string colourNameGlobal;
	glm::vec4 rgbValues;

	std::vector<TexturePoint> textureUVCoords;
	std::vector<glm::vec3> multiSetVertices;
	glm::vec3 singleSetVertices;

	
	std::unordered_map<std::string, glm::vec4> colourMap = loadMatOBJ();
	
	while (getline(MyReadFile, myText)) {
		std::stringstream ss(myText);
		std::stringstream ss2(myText);
		std::stringstream ss3(myText);
		std::string identifier;
		ss >> identifier;

		//std::cout << MyReadFile.rdbuf() << '\n';

		/*std::cout << myText[0] << std::endl;
		for (size_t i = 0; i < myText.size(); i++) std::cout << myText[i] << " " << std::endl;*/
		if (identifier == "usemtl") {
			std::string colourName;
			ss >> colourName;
			colourNameGlobal = colourName;
		}
		else if (identifier == "v") {

			float x, y, z;
			ss >> x >> y >> z;

			//SCALING FACTOR INTRODUCED HERE

			singleSetVertices.x = scalingFactor * x;
			singleSetVertices.y = scalingFactor * y;
			singleSetVertices.z = scalingFactor * z;

			//std::cout << "new aksfa (" << singleSetVertices.x << ", " << singleSetVertices.y << ", " << singleSetVertices.z << ")" << std::endl;


			multiSetVertices.push_back(singleSetVertices);

		}

		else if (identifier == "f") {
			int a, b, c, h, i,j, k, l, m;
			char d;

			std::string e, f, g;

			ss >> e >> f >> g;

			if (isdigit(g.back())) {
				
				while ((ss2 >> identifier >> a >> d >> b >> c >> d >> h >> i >> d >> j) && (d == '/')) {
					modelTriangle.vertices[0] = multiSetVertices[a - 1];
					modelTriangle.vertices[1] = multiSetVertices[c - 1];
					modelTriangle.vertices[2] = multiSetVertices[i - 1];

					modelTriangle.texturePoints[0] = textureUVCoords[b-1];
					modelTriangle.texturePoints[1] = textureUVCoords[h-1];
					modelTriangle.texturePoints[2] = textureUVCoords[j-1];

					rgbValues = colourMap[colourNameGlobal];

					rgbColour.red = rgbValues.r;
					rgbColour.green = rgbValues.g;
					rgbColour.blue = rgbValues.b;

					modelTriangle.colour = rgbColour;
					modelTriangle.colour.name = colourNameGlobal;
					modelTriangle.normal = normalise(calcSurfaceNormal(modelTriangle));

					modelTriangles.push_back(modelTriangle);
				}
			}
			else {
				//ss3 >> identifier >> k >> d >> l >> d >> m;
				while ((ss3 >> identifier >> k >> d >> l >> d >> m) && d == '/') {


					//std::cout << "new position (" << a << ", " << b << ", " << c << ")" << std::endl;

					modelTriangle.vertices[0] = multiSetVertices[k - 1];						//a-1 since f starts at 1 but the multiSetVertices index starts at 0
					modelTriangle.vertices[1] = multiSetVertices[l - 1];
					modelTriangle.vertices[2] = multiSetVertices[m - 1];

					rgbValues = colourMap[colourNameGlobal];

					rgbColour.red = rgbValues.r;
					rgbColour.green = rgbValues.g;
					rgbColour.blue = rgbValues.b;

					modelTriangle.colour = rgbColour;
					modelTriangle.colour.name = colourNameGlobal;
					modelTriangle.normal = normalise(calcSurfaceNormal(modelTriangle));
					//std::cout << glm::to_string(modelTriangle.normal) << '\n';

					modelTriangles.push_back(modelTriangle);

				}
			}
		
		}

		else if (identifier == "vt") {
			float a, b;
			ss >> a >> b;
			TexturePoint UV = {a,b};
			textureUVCoords.push_back(UV);
		}

	}

	std::vector<glm::vec3> multiSetVertices2;
	glm::vec3 singleSetVertices2;

	while (getline(MyReadFileSphere, myText)) {
		float scalingFactor2 = 0.3;
		std::stringstream ss(myText);
		std::string identifier;
		ss >> identifier;

		if (identifier == "v") {

			float x, y, z;
			ss >> x >> y >> z;

			//SCALING FACTOR INTRODUCED HERE

			singleSetVertices2.x = scalingFactor2 * x;
			singleSetVertices2.y = scalingFactor2 * y;
			singleSetVertices2.z = scalingFactor2 * z;

			//std::cout << "new aksfa (" << singleSetVertices.x << ", " << singleSetVertices.y << ", " << singleSetVertices.z << ")" << std::endl;


			multiSetVertices2.push_back(singleSetVertices2);

		}
		
		else if (identifier == "f") {
			int a, b, c;
			char d;

			while ((ss >> a >> d >> b >> d >> c) && d == '/') {


				//std::cout << "new position (" << a << ", " << b << ", " << c << ")" << std::endl;

				modelTriangle.vertices[0] = multiSetVertices2[a - 1];						//a-1 since f starts at 1 but the multiSetVertices index starts at 0
				modelTriangle.vertices[1] = multiSetVertices2[b - 1];
				modelTriangle.vertices[2] = multiSetVertices2[c - 1];

				rgbValues = colourMap["Green"];

				rgbColour.red = 0;
				rgbColour.green = 255;
				rgbColour.blue = 0;

				modelTriangle.colour = rgbColour;
				modelTriangle.colour.name = "Green";
				modelTriangle.normal = normalise(calcSurfaceNormal(modelTriangle));
				//std::cout << glm::to_string(modelTriangle.normal) << '\n';

				modelTriangles.push_back(modelTriangle);

			}
		}

	}

	for (size_t i = 0; i < modelTriangles.size(); i++) {
		if (modelTriangles[i].colour.name == "Cobbles") {
			CanvasTriangle triangle;
			triangle.v0().texturePoint.x = modelTriangles[i].texturePoints[0].x;
			triangle.v1().texturePoint.x = modelTriangles[i].texturePoints[1].x;
			triangle.v2().texturePoint.x = modelTriangles[i].texturePoints[2].x;
			triangle.v0().texturePoint.y = modelTriangles[i].texturePoints[0].y;
			triangle.v1().texturePoint.y = modelTriangles[i].texturePoints[1].y;
			triangle.v2().texturePoint.y = modelTriangles[i].texturePoints[2].y;


			triangle.v0().x = modelTriangles[i].vertices[0].x;
			triangle.v1().x = modelTriangles[i].vertices[1].x;
			triangle.v2().x = modelTriangles[i].vertices[2].x;
			triangle.v0().y = modelTriangles[i].vertices[0].y;
			triangle.v1().y = modelTriangles[i].vertices[1].y;
			triangle.v2().y = modelTriangles[i].vertices[2].y;
			triangle.v0().depth = modelTriangles[i].vertices[0].z;
			triangle.v1().depth = modelTriangles[i].vertices[1].z;
			triangle.v2().depth = modelTriangles[i].vertices[2].z;

			//loadTextureMap(triangle, window);
		}
	}

	MyReadFileSphere.close();
	MyReadFile.close();
	return modelTriangles;

}

glm::vec3 calcSurfaceNormal(ModelTriangle triangle) {
	//glm::vec3 triangleNormal = glm::triangleNormal(triangle.vertices[1], triangle.vertices[2], triangle.vertices[0]);
	//cool stuff happens if use line below 
	glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
	glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
	glm::vec3 triangleNormal = glm::cross(e0, e1);
	
	//triangleNormal = normalise(triangleNormal);
	//std::cout << "normal: " << glm::to_string(triangleNormal) << '\n';
	return triangleNormal;
}

CanvasTriangle convertModelTriToCanvas(ModelTriangle modelTriangle) {
	CanvasTriangle triangle;
	triangle.v0().texturePoint.x = modelTriangle.texturePoints[0].x;
	triangle.v1().texturePoint.x = modelTriangle.texturePoints[1].x;
	triangle.v2().texturePoint.x = modelTriangle.texturePoints[2].x;
	triangle.v0().texturePoint.y = modelTriangle.texturePoints[0].y;
	triangle.v1().texturePoint.y = modelTriangle.texturePoints[1].y;
	triangle.v2().texturePoint.y = modelTriangle.texturePoints[2].y;


	triangle.v0().x = modelTriangle.vertices[0].x;
	triangle.v1().x = modelTriangle.vertices[1].x;
	triangle.v2().x = modelTriangle.vertices[2].x;
	triangle.v0().y = modelTriangle.vertices[0].y;
	triangle.v1().y = modelTriangle.vertices[1].y;
	triangle.v2().y = modelTriangle.vertices[2].y;
	triangle.v0().depth = modelTriangle.vertices[0].z;
	triangle.v1().depth = modelTriangle.vertices[1].z;
	triangle.v2().depth = modelTriangle.vertices[2].z;


	return triangle;
}