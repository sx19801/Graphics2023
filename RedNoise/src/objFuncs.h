#ifndef OBJFUNCS_H
#define OBJFUNCS_H
#include <iostream>
#include <fstream>
#include <Utils.h>
#include <Colour.h>
#include <string>
#include <sstream>
#include <ModelTriangle.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


std::unordered_map<std::string, glm::vec3> colourMap;						//initialise global hashmap for colour

glm::vec3 convMtlToInt(glm::vec3 colourValues){
	glm::vec3 RGBColours;
	
	RGBColours.r = round(colourValues.r * 255);
	RGBColours.b = round(colourValues.b	* 255);
	RGBColours.g = round(colourValues.g * 255);

	return RGBColours;
}

void loadMatOBJ() {
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
			std::cout << a << " << a" << std::endl;

		}
		else if (identifier == "Kd") {
			float a, b, c;
			ss >> a >> b >> c;

			glm::vec3 RGBColours = convMtlToInt({ a,b,c });

			colourMap[name] = { RGBColours.r, RGBColours.g, RGBColours.b };
			
		}

	}


	for (auto const& pair : colourMap) {																//print out all elements in hash map
		std::cout << "{" << pair.first << ": " << glm::to_string(pair.second) << "}\n";
	}

	std::cout << colourMap.size() << std::endl;
}

std::vector<ModelTriangle> loadGeoOBJ(float scalingFactor) {
	std::string myText;
	std::ifstream MyReadFile("../../../OBJFiles/cornell-box.obj");

	Colour rgbColour;
	ModelTriangle modelTriangle;
	std::vector<ModelTriangle> modelTriangles;
	std::vector<glm::vec3> multiSetVertices;
	glm::vec3 singleSetVertices;
	glm::vec3 trianglePoints;
	std::string colourNameGlobal;
	glm::vec3 rgbValues;
	while (getline(MyReadFile, myText)) {
		std::stringstream ss(myText);
		std::string identifier;
		ss >> identifier;
		//USING SPLIT FUNC INSTEAD OF STRINGSTREAM
		//std::vector<std::string> newText = split(myText, ' ');

		//std::cout << newText[0] << std::endl;
		//for (size_t i = 0; i < newText.size(); i++) std::cout << newText[i] << " " << std::endl;
		if (identifier == "usemtl") {
			std::string colourName;
			ss >> colourName;
			colourNameGlobal = colourName;
		}
		else if (identifier == "v") {
			//std::cout << myText << std::endl;

			float x, y, z;
			ss >> x >> y >> z;

			//SCALING FACTOR INTRODUCED HERE
			/*singleSetVertices.x = x;
			singleSetVertices.y = y;
			singleSetVertices.z = z;*/

			singleSetVertices.x = scalingFactor * x;
			singleSetVertices.y = scalingFactor * y;
			singleSetVertices.z = scalingFactor * z;

			multiSetVertices.push_back(singleSetVertices);

		}

		else if (identifier == "f") {
			int a, b, c;
			char d;

			while ((ss >> a >> d >> b >> d >> c) && d == '/') {
				//std::cout << myText << std::endl;

				//std::cout << "new position (" << a << ", " << b << ", " << c << ")" << std::endl;

				modelTriangle.vertices[0] = multiSetVertices[a - 1];						//a-1 since f starts at 1 but the multiSetVertices index starts at 0
				modelTriangle.vertices[1] = multiSetVertices[b - 1];
				modelTriangle.vertices[2] = multiSetVertices[c - 1];

				//std::cout << glm::to_string(colourMap[colourNameGlobal]) << " < this the colourmap yo" << std::endl;
				
				rgbValues = colourMap[colourNameGlobal];

				//std::cout << glm::to_string(rgbValues) << std::endl;
				/*rgbValues.r = colourMap[colourNameGlobal].r;
				rgbValues.b = colourMap[colourNameGlobal].b;
				rgbValues.g = colourMap[colourNameGlobal].g;*/

				//rgbColour = {2 ,1 ,3 };
				//rgbColour = { rgbValues.r, rgbValues.g, rgbValues.b };

				rgbColour.red = rgbValues.r;
				rgbColour.green = rgbValues.g;
				rgbColour.blue = rgbValues.b;


				//rgbColour( rgbValues.r, rgbValues.b, rgbValues.g);

				modelTriangle.colour = rgbColour;
				modelTriangle.colour.name = colourNameGlobal;

				modelTriangles.push_back(modelTriangle);

			}
		}

	}
	//for (size_t i = 0; i < multiSetVertices.size(); i++) { std::cout << glm::to_string(multiSetVertices[i]) << " <---- NEW SHIT <---" << std::endl; }

	//seems to be working fine 

	MyReadFile.close();
	return modelTriangles;
}


#endif