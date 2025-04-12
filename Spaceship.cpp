// Spaceship (Player) implementation

#include "Spaceship.h"

void Spaceship::setup() {
    radius = 25;
    mass = 1.0;
    color = ofColor::white;
}

void Spaceship::draw() {
    ofPushMatrix();
    ofMultMatrix(getTransform());
    

    
	// Change to green to signifiy invincibility 
	if (invincibleTimer > 0) {
		ofSetColor(ofColor::green);
	}else{
		// Draw Spaceship body
		ofSetColor(color);

	}

	ofSetLineWidth(2.0);
	ofNoFill();
    ofDrawTriangle(verts[0], verts[1], verts[2]);

    
    // Draw thruster flame when thrusting
    if (bThrusting) {
        ofSetColor(ofColor::orange);
    }
    
    ofPopMatrix();
}

// Point in triangle test for collision
bool Spaceship::inside(glm::vec3 p0) {
    glm::vec3 p = glm::inverse(getTransform()) * glm::vec4(p0, 1);
    glm::vec3 v1 = glm::normalize(verts[0] - p);
    glm::vec3 v2 = glm::normalize(verts[1] - p);
    glm::vec3 v3 = glm::normalize(verts[2] - p);
    float a1 = glm::orientedAngle(v1, v3, glm::vec3(0, 0, -1));
    float a2 = glm::orientedAngle(v3, v2, glm::vec3(0, 0, -1));
    float a3 = glm::orientedAngle(v2, v1, glm::vec3(0, 0, -1));
    if (a1 < 0 && a2 < 0 && a3 < 0) return true;
    else return false;
}


// Get heading vector based on Spaceship's angle
glm::vec3 Spaceship::getHeading() {
    glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(angle), glm::vec3(0, 0, 1));
    return glm::normalize(rot * glm::vec4(0, -1, 0, 0));
}
