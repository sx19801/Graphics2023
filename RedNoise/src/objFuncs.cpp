#include "objFuncs.h"
#include "rayTracing.h"


glm::vec3 convMtlToInt(glm::vec3 colourValues) {
	glm::vec3 RGBColours;

	RGBColours.r = round(colourValues.r * 255);
	RGBColours.b = round(colourValues.b * 255);
	RGBColours.g = round(colourValues.g * 255);

	return RGBColours;
}

std::unordered_map<std::string, glm::vec3> loadMatOBJ() {
	std::unordered_map<std::string, glm::vec3> colourMap;						//initialise global hashmap for colour
	std::string myText;
	std::ifstream MyReadFile("../../../OBJFiles/cornell-box.mtl");
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
			colourMap[a] = {};

		}
		else if (identifier == "Kd") {
			float a, b, c;
			ss >> a >> b >> c;

			glm::vec3 RGBColours = convMtlToInt({ a,b,c });

			colourMap[name] = { RGBColours.r, RGBColours.g, RGBColours.b };

		}

	}
	return colourMap;
}



std::vector<ModelTriangle> loadGeoOBJ(float scalingFactor) {
	std::string myText;
	std::ifstream MyReadFile("../../../OBJFiles/cornell-box.obj");
	std::ifstream MyReadFileSphere("../../../OBJFiles/sphere.obj");

	Colour rgbColour;
	ModelTriangle modelTriangle;
	std::vector<ModelTriangle> modelTriangles;

	glm::vec3 trianglePoints;
	std::string colourNameGlobal;
	glm::vec3 rgbValues;

	std::vector<glm::vec3> multiSetVertices;
	glm::vec3 singleSetVertices;

	std::unordered_map<std::string, glm::vec3> colourMap = loadMatOBJ();
	
	while (getline(MyReadFile, myText)) {
		std::stringstream ss(myText);
		std::string identifier;
		ss >> identifier;

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
			int a, b, c;
			char d;

			while ((ss >> a >> d >> b >> d >> c) && d == '/') {


				//std::cout << "new position (" << a << ", " << b << ", " << c << ")" << std::endl;

				modelTriangle.vertices[0] = multiSetVertices[a - 1];						//a-1 since f starts at 1 but the multiSetVertices index starts at 0
				modelTriangle.vertices[1] = multiSetVertices[b - 1];
				modelTriangle.vertices[2] = multiSetVertices[c - 1];

				rgbValues = colourMap[colourNameGlobal];

				rgbColour.red = rgbValues.r;
				rgbColour.green = rgbValues.g;
				rgbColour.blue = rgbValues.b;

				modelTriangle.colour = rgbColour;
				modelTriangle.colour.name = colourNameGlobal;
				modelTriangle.normal = normalise(calcSurfaceNormal(modelTriangle));
				//std::cout << glm::to_string(modelTriangle.normal) << '\n';

				//modelTriangles.push_back(modelTriangle);

			}
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

				rgbValues = colourMap["Red"];

				rgbColour.red = 255;
				rgbColour.green = 0;
				rgbColour.blue = 0;

				modelTriangle.colour = rgbColour;
				modelTriangle.colour.name = "Red";
				modelTriangle.normal = normalise(calcSurfaceNormal(modelTriangle));
				//std::cout << glm::to_string(modelTriangle.normal) << '\n';

				modelTriangles.push_back(modelTriangle);

			}
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
