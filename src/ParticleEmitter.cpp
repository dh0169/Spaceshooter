
#include "ParticleEmitter.h"

void ParticleEmitter::setup() {
    particles.clear();
}

void ParticleEmitter::update(float dt) {
    // Update existing particles
    for (int i = particles.size() - 1; i >= 0; i--) {
        particles[i].integrate(dt);
        particles[i].age += dt;
        
        // Remove dead particles
        if (particles[i].age >= particles[i].lifespan) {
            particles.erase(particles.begin() + i);
        }
    }
}

void ParticleEmitter::draw() {
    for (auto &p : particles) {
        p.draw();
    }
}

void ParticleEmitter::emit(int num, glm::vec3 pos, glm::vec3 vel, ofColor color) {
    for (int i = 0; i < num; i++) {
        if (particles.size() >= maxParticles) return;
        
        Particle p;
        p.setup(
            pos + glm::vec3(ofRandom(-5, 5), ofRandom(-5, 5), 0),
            vel + glm::vec3(ofRandom(-50, 50), ofRandom(-50, 50), 0),
            color
        );
        particles.push_back(p);
    }
}

