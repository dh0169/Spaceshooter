#pragma once

#include "ofApp.h"


class Bullet : public Shape {
    public:
        void setup(glm::vec3 pos, glm::vec3 vel);
        void draw() override;
    
        // Prevent bullets from wrapping around.
        void checkBounds() override;
    
};