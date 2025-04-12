#pragma once

#include "ofMain.h"
#include "Shape.h"

class Asteroid : public Shape {
	public:
		void setup(int size, glm::vec3 pos);
		void draw() override;
		
		vector<glm::vec3> vertices;
		int numVertices = 10;  // Minimum 10 sides as required
		int size = 3;          // 3 is large, 2 medium, and 1 small
		
		void generateShape();
	
};