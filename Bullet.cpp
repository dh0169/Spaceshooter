#include "Bullet.h"

void Bullet::checkBounds(){
	return;
}

void Bullet::setup(glm::vec3 pos, glm::vec3 vel) {
    position = pos;
    velocity = vel;
    radius = 3.0;
    color = ofColor::yellow;
    mass = 0.1;
}

void Bullet::draw() {
    ofPushMatrix();
    ofSetColor(color);
    ofTranslate(position);
    ofDrawCircle(0, 0, radius);
    ofPopMatrix();
}