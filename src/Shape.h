#pragma once

#include "ofMain.h"

// Base Shape class
class Shape {
public:
    virtual void draw() {}
    virtual bool inside(glm::vec3 p){ 
		return false;
	}
    glm::mat4 getTransform();
    
    glm::vec3 position = glm::vec3(100, 100, 0);
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    glm::vec3 acceleration = glm::vec3(0, 0, 0);
    glm::vec3 forces = glm::vec3(0, 0, 0);
    
    // degrees
    float angle = 0.0;            
    float angularVelocity = 0.0;  // degrees per second
    float angularAcceleration = 0.0;
    float rotationalForce = 0.0;
    
    glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
    
    // Collision detection radius 
    float radius = 10.0;          
    ofColor color = ofColor::white;
    
    float mass = 1.0;
    bool bActive = true;
    
    // Check screen boundaries and wrap if needed
    virtual void checkBounds();
    
    // Euler method
    virtual void integrate(float dt);
    
    virtual void applyForce(glm::vec3 force);
    virtual void applyRotationalForce(float force);
};
