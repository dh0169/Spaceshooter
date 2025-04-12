#pragma once

#include "Shape.h"

// Player's Spaceship
class Spaceship: public Shape {
public:
    glm::vec3 verts[3] = {
        glm::vec3(0, -25, 0),     // top vertex (front of Spaceship)
        glm::vec3(-15, 15, 0),    // bottom left
        glm::vec3(15, 15, 0),     // bottom right
    };
    
    void setup();
    void draw() override;
    bool inside(glm::vec3 p0) override;  
    
    // Game specific 
    float thrustPower = 350.0;
    float rotationPower = 350.0;
    float damping = 0.985;         
    
    // Display options
    bool bImgShow = false;
    bool bMouseInside = false;
    bool bThrusting = false;
    
    int lives = 3;
    float invincibleTimer = 0;
    bool isInvincible() { return invincibleTimer > 0; }
    
    // Get heading vector
    glm::vec3 getHeading();
};