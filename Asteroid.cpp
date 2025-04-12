// Asteroid implementation
#include "Asteroid.h"

void Asteroid::setup(int _size, glm::vec3 pos) {
    size = _size;
    position = pos;
    
    // Scale radius and mass based on size
    radius = 20.0 * size;
    mass = 0.5 * size;
    
    // Random initial velocity and rotation
    velocity = glm::vec3(ofRandom(-30, 30), ofRandom(-30, 30), 0);
    angularVelocity = ofRandom(-30, 30);
    
    color = ofColor(200, 200, 200);
    generateShape();
}

void Asteroid::generateShape() {
    vertices.clear();
    
    // Create a random polygon with at least 10 sides
    numVertices = ofRandom(10, 16);
    
    // Generate points on a circle with random variations
    for (int i = 0; i < numVertices; i++) {
        float angle = (TWO_PI / numVertices) * i;
        float rad = radius * ofRandom(0.5, 1.0); //affects pointiness of aestroids
        
        glm::vec3 vert;
        vert.x = cos(angle) * rad;
        vert.y = sin(angle) * rad;
        vert.z = 0;
        
        vertices.push_back(vert);
    }
}

void Asteroid::draw() {
    ofPushMatrix();
    ofMultMatrix(getTransform());
    
    ofSetColor(color);
    ofSetLineWidth(2.0);
    ofNoFill();
    
    // Draw the asteroid shape
    ofBeginShape();
    for (auto &v : vertices) {
        ofVertex(v);
    }
    ofEndShape(true); // Close the shape
    
    ofPopMatrix();
}