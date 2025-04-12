#include "Shape.h"

glm::mat4 Shape::getTransform() {
    glm::mat4 T = glm::translate(glm::mat4(1.0), position);
    glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(angle), glm::vec3(0, 0, 1));
    glm::mat4 S = glm::scale(glm::mat4(1.0), scale);
    return (T * R * S);
}


void Shape::applyForce(glm::vec3 force) {
    forces += force;
}

void Shape::applyRotationalForce(float force) {
    rotationalForce += force;
}

// Integrate physics using Euler integration
// Fnet = ma
// a = Fnet / mass
void Shape::integrate(float dt) {

    // Linear motion
    acceleration = forces / mass; 
    velocity += acceleration * dt;
    position += velocity * dt;

    forces = glm::vec3(0, 0, 0); // reset forces
    
    // Angular motion
    angularAcceleration = rotationalForce / mass;
    angularVelocity += angularAcceleration * dt;
    angle += angularVelocity * dt;
    rotationalForce = 0;
}


void Shape::checkBounds() {
    // Wrap horizontally
    if (position.x < -radius){
		position.x = ofGetWidth() + radius;
	}else if (position.x > ofGetWidth() + radius){
		position.x = -radius;
	}
    
    // Wrap vertically
    if (position.y < -radius){
		position.y = ofGetHeight() + radius;
	}else if(position.y > ofGetHeight() + radius){
		position.y = -radius;
	}
}