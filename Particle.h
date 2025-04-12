#pragma once

#include "Shape.h"

class Particle : public Shape {
    public:
        void setup(glm::vec3 pos, glm::vec3 vel, ofColor c);
        void draw() override;
        float lifespan = 1.0;  // seconds before despawning
        float age = 0;
};