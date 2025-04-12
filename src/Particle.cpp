#include "Particle.h"

void Particle::setup(glm::vec3 pos, glm::vec3 vel, ofColor c) {
    position = pos;
    velocity = vel;
    color = c;
    radius = ofRandom(1, 3);
    mass = 0.1;
    lifespan = ofRandom(0.5, 1.5);
}

void Particle::draw() {
    ofSetColor(color, ofMap(age, 0, lifespan, 255, 0)); // Fade out
    ofDrawCircle(position, radius * (1 - age/lifespan)); // Shrink
}
