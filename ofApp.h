#pragma once

#include "ofMain.h"
#include "Asteroid.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "ParticleEmitter.h"

class Asteroid;
class Spaceship;
class Bullet;
class ParticleEmitter;



// Main application
class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void loadMedia();

	void handleUserInput();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    // Game methods
    void resetGame();
    void spawnAsteroids(int num);
    void checkCollisions();
    void drawHUD();
    void fireBullet();
    void splitAsteroid(Asteroid& asteroid, Bullet& bullet);
    
    // Time variables for physics
    float lastTime = 0;
    float deltaTime = 0;
    
    // Player
    Spaceship player;
	std::set<int> keysHeld;
    
    // Game entities
    vector<Bullet> bullets;
    vector<Asteroid> asteroids;
    
    // Effects systems
    ParticleEmitter thrustEmitter;
    ParticleEmitter explosionEmitter;
    
    int score = 0;
    int level = 1;
    bool gameOver = false;
    bool gameStarted = false;
    float respawnTimer = 0;
    
    // Media, Sounds and Image
    ofSoundPlayer thrustSound;
    ofSoundPlayer shootSound;
    ofSoundPlayer explosionSound;
    ofSoundPlayer playerDeathSound;
    ofImage backgroundImage;
    
	bool thrustSoundPlaying = false;

};