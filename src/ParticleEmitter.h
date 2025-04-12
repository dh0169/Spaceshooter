#pragma once

#include <vector>
#include "Particle.h"

class ParticleEmitter {
    public:
        void setup();
        void update(float dt);
        void draw();
        
        void emit(int num, glm::vec3 pos, glm::vec3 vel, ofColor color);
        
        vector<Particle> particles;
        int maxParticles = 1000;
    };
    