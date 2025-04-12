#include "Bullet.h"

bool Bullet::isOutOfBounds(){
    return (
        position.x < -radius ||
        position.x > ofGetWidth() + radius ||
        position.y < -radius ||
        position.y > ofGetHeight() + radius
    );
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