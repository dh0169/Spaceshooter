#include "ofApp.h"

void ofApp::setup() {
    ofSetBackgroundColor(ofColor::black);
    
	loadMedia();

    player.setup();
    player.position = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
    
    // Setup particle systems
    thrustEmitter.setup();
    explosionEmitter.setup();
    
    // Initialize time
    lastTime = ofGetElapsedTimef();
    
    // Initial game setup
    resetGame();
}

void ofApp::loadMedia(){
	if (!thrustSound.load("sounds/thrust.wav")) {
		ofLogError() << "Cannot load thrust sound";
	}
	
	if (!shootSound.load("sounds/shoot.wav")) {
		ofLogError() << "Cannot load shoot sound";
	}
	
	if (!explosionSound.load("sounds/explosion.wav")) {
		ofLogError() << "Cannot load explosion sound";
	}
	
	if (!playerDeathSound.load("sounds/player_death.wav")) {
		ofLogError() << "Cannot load player death sound";
	}
	
	// Setup background
	if (!backgroundImage.load("images/bg.png")) {
		ofLogError() << "Cannot load background image";
		ofExit(0);
	}
}

void ofApp::resetGame() {
    // Reset game state
    score = 0;
    level = 1;
    gameOver = false;
    
    // Reset player
    player.position = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
    player.velocity = glm::vec3(0, 0, 0);
    player.angle = 0;
    player.angularVelocity = 0;
    player.lives = 3;
    player.invincibleTimer = 3.0; // Invincible for 3 seconds on spawn
    
    bullets.clear();
    asteroids.clear();
    
    spawnAsteroids(4);
}

void ofApp::spawnAsteroids(int num) {
    for (int i = 0; i < num; i++) {
        Asteroid asteroid;
        
        // Spawn astroid a certain distnace away from player
        glm::vec3 pos = glm::vec3(ofRandom(100, ofGetWidth() - 100),ofRandom(100, ofGetHeight() - 100),0);
		while (glm::distance(pos, player.position) < 350){
            pos = glm::vec3(ofRandom(100, ofGetWidth() - 100),ofRandom(100, ofGetHeight() - 100),0);
        }
        
        asteroid.setup(3, pos);
        asteroids.push_back(asteroid);
    }
}

void ofApp::update() {
    // Calculate delta time
    float currentTime = ofGetElapsedTimef();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    
    

    
    if (gameOver) {
        // In game over state, only handle restart
        return;
    }
    
    // Update player invincibility timer
    if (player.invincibleTimer > 0) {
        player.invincibleTimer -= deltaTime;
    }

	handleUserInput();

    // Update player respawn timer
    if (respawnTimer > 0) {
        respawnTimer -= deltaTime;
        if (respawnTimer <= 0) {
            // Respawn player
            player.position = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
            player.velocity = glm::vec3(0, 0, 0);
            player.angularVelocity = 0;
            player.bActive = true;
            player.invincibleTimer = 3.0; // Invincible after respawn
        }
    }
    
    // Apply damping to player velocity
    player.velocity *= player.damping;
    player.angularVelocity *= player.damping;
    
    // Integrate physics for all entities
    if (player.bActive) {
        player.integrate(deltaTime);
        player.checkBounds();
    }
    
    // Thrust particles
    if (player.bThrusting && player.bActive) {
        // Emit thruster particles behind the Spaceship
        glm::vec3 thrustPos = player.position - player.getHeading() * 20.0f;
        glm::vec3 thrustVel = -player.getHeading() * 100.0f;
        thrustEmitter.emit(5, thrustPos, thrustVel, ofColor::orange);
    }
    
    // Update bullets
    for (int i = bullets.size() - 1; i >= 0; i--) {
        bullets[i].integrate(deltaTime);
        if(bullets[i].isOutOfBounds()){
            bullets.erase(bullets.begin() + i);
        }
    }
    
    // Update asteroids
    for (auto &asteroid : asteroids) {
        // Add random forces occasionally
        if (ofRandom(1.0) < 0.05) {
            asteroid.applyForce(glm::vec3(ofRandom(-10, 10),ofRandom(-10, 10),0));
        }
        
        asteroid.integrate(deltaTime);
        asteroid.checkBounds();
    }
    
    // Update particle systems
    thrustEmitter.update(deltaTime);
    explosionEmitter.update(deltaTime);
    
    // Check for collisions
    checkCollisions();
    
    // Check if level is complete (no asteroids left)
    if (asteroids.empty()) {
        level++;
        spawnAsteroids(4 + level);
    }
}

void ofApp::fireBullet() {
    if (!player.bActive) return;
    
    Bullet bullet;
    glm::vec3 bulletVel = player.getHeading() * 500.0f + player.velocity;
    bullet.setup(player.position + player.getHeading() * 30.0f, bulletVel);
    bullets.push_back(bullet);
    
    // Play sound
    shootSound.play();
}

void ofApp::splitAsteroid(Asteroid& asteroid, Bullet& bullet) {
    // Score based on asteroid size
    score += (4 - asteroid.size) * 100;
    
    // Create explosion at asteroid position
    explosionEmitter.emit(50, asteroid.position, asteroid.velocity * 0.5f,ofColor(255, 200, 100));
    
    // Play explosion sound
    explosionSound.play();
    
    // Split asteroid if it's not the smallest size
    if (asteroid.size > 1) {
        for (int i = 0; i < 3; i++) { // Create 3 smaller asteroids
            Asteroid newAsteroid;
            newAsteroid.setup(asteroid.size - 1, asteroid.position);
            
            // Add some velocity in random directions
            glm::vec3 dir = glm::normalize(glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), 0));
            newAsteroid.velocity = asteroid.velocity * 0.5f + dir * ofRandom(20, 50);
            
            asteroids.push_back(newAsteroid);
        }
    }
}

void ofApp::checkCollisions() {
    // Check bullet-asteroid collisions
    for (int i = bullets.size() - 1; i >= 0; i--) {
        for (int j = asteroids.size() - 1; j >= 0; j--) {
            float dist = glm::distance(bullets[i].position, asteroids[j].position);
            
            if (dist < (bullets[i].radius + asteroids[j].radius)) {
                // Collision occurred
                splitAsteroid(asteroids[j], bullets[i]);
                
                // Remove the bullet and asteroid
                bullets.erase(bullets.begin() + i);
                asteroids.erase(asteroids.begin() + j);
                goto nextBullet; // Skip to next bullet after collision
            }
        }
        nextBullet: continue;
    }
    
    // Check player-asteroid collisions (only if not invincible)
    if (player.bActive && !player.isInvincible()) {
        for (auto &asteroid : asteroids) {
            float dist = glm::distance(player.position, asteroid.position);
            
            if (dist < (player.radius + asteroid.radius)) {
                // Player hit asteroid
                player.lives--;
                explosionEmitter.emit(100, player.position, player.velocity * 0.5f, ofColor::red);
                playerDeathSound.play();
                
                if (player.lives <= 0) {
                    gameOver = true;
                } else {
                    player.bActive = false;
                    respawnTimer = 2.0;
                }
                break;
            }
        }
    }
    
    // Check asteroid-asteroid collisions for bouncing
    for (int i = 0; i < asteroids.size(); i++) {
        for (int j = i + 1; j < asteroids.size(); j++) {
            float dist = glm::distance(asteroids[i].position, asteroids[j].position);
            float minDist = asteroids[i].radius + asteroids[j].radius;
            
            if (dist < minDist) {
                // Simple elastic collision response
                glm::vec3 normal = glm::normalize(asteroids[j].position - asteroids[i].position);
                
                // Calculate relative velocity
                glm::vec3 relVel = asteroids[j].velocity - asteroids[i].velocity;
                
                // Calculate impulse
                float impulse = glm::dot(relVel, normal) * 1.5f; // 1.5 arbitrary value
                
                // Apply impulse
                asteroids[i].velocity += normal * impulse * (asteroids[j].mass / (asteroids[i].mass + asteroids[j].mass));
                asteroids[j].velocity -= normal * impulse * (asteroids[i].mass / (asteroids[i].mass + asteroids[j].mass));
                
                // Separate the asteroids to prevent sticking
                float overlap = minDist - dist;
                glm::vec3 separation = normal * overlap * 0.5f;
                asteroids[i].position -= separation;
                asteroids[j].position += separation;
            }
        }
    }
}
//-----------------------------------------//

//--------------- GUI/Visual ---------------//

void ofApp::draw() {
    // Draw background
    ofSetColor(255);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    // Draw asteroids
    for (auto &asteroid : asteroids) {
        asteroid.draw();
    }
    
    // Draw bullets
    for (auto &bullet : bullets) {
        bullet.draw();
    }
    
    // Draw player if active
    if (player.bActive) {
        player.draw();
    }
    
    // Draw particle systems
    thrustEmitter.draw();
    explosionEmitter.draw();
    
    // Draw HUD
    drawHUD();
    
    // Draw game over screen
    if (gameOver) {
        ofSetColor(255);
        string gameOverText = "GAME OVER";
        string scoreText = "Final Score: " + ofToString(score);
        string restartText = "Press SPACE to restart";
        
        ofDrawBitmapString(gameOverText, ofGetWidth()/2 - gameOverText.length() * 4, ofGetHeight()/2 - 30);    
        ofDrawBitmapString(scoreText, ofGetWidth()/2 - scoreText.length() * 4, ofGetHeight()/2);
        ofDrawBitmapString(restartText, ofGetWidth()/2 - restartText.length() * 4, ofGetHeight()/2 + 30);
    }
    

}

void ofApp::drawHUD() {
    ofSetColor(255);
    
   
    string scoreText = "SCORE: " + ofToString(score);
    string levelText = "LEVEL: " + ofToString(level);
	string livesText = "LIVES: ";

	ofDrawBitmapString(scoreText, 20, 30);
    ofDrawBitmapString(levelText, 20, 50);
	ofDrawBitmapString(livesText, 20, 70);
    
    // Draw small SpaceShips for lives
    for (int i = 0; i < player.lives; i++) {
        ofPushMatrix();
        ofTranslate(90 + i * 25, 65);
        ofScale(0.5, 0.5);
        ofSetColor(ofColor::white);
        ofNoFill();
        ofDrawTriangle(0, -10, -8, 8, 8, 8);
        ofPopMatrix();
    }
    
    string controlsText = "Use Arrow Keys to move, SPACE to fire, 'R' to restart";
    ofDrawBitmapString(controlsText, 20, ofGetHeight() - 20);
}
//-----------------------------------------//

//--------------- User Input ---------------//
void ofApp::handleUserInput(){

	if (keysHeld.count(OF_KEY_UP) || keysHeld.count('w')) {
		player.bThrusting = true;
		player.applyForce(player.getHeading() * player.thrustPower);
		if (!thrustSoundPlaying) {
			thrustSound.setLoop(true);  // Make the sound loop continuously
			thrustSound.play();
			thrustSoundPlaying = true;
		}
	} else {
		// Stop thrust sound if not thrusting
		player.bThrusting = false;
		if (thrustSoundPlaying) {
			thrustSound.stop();
			thrustSoundPlaying = false;
		}
		
	}


	if (keysHeld.count(OF_KEY_LEFT) || keysHeld.count('a')) {
		player.applyRotationalForce(-player.rotationPower);
	}

	if (keysHeld.count(OF_KEY_RIGHT) || keysHeld.count('d')) {
		player.applyRotationalForce(player.rotationPower);
	}

}


void ofApp::keyPressed(int key) {
	keysHeld.insert(key);	
	if(key == 'r'){
		resetGame();    
    }else if (key == ' '){
		fireBullet();
	}
}

void ofApp::keyReleased(int key) {
	keysHeld.erase(key);
}
//-----------------------------------------//



//--------------- Mouse Events ---------------//
void ofApp::mouseDragged(int x, int y, int button){
    if (player.bMouseInside){
        glm::vec3 mouse_pos = glm::vec3(x, y, 0);
        player.position = mouse_pos;
        cout << "Position: " <<  x << ", " << y << endl;        
    }
}



void ofApp::mousePressed(int x, int y, int button){
    player.bMouseInside = player.inside(glm::vec3(x, y, 0));
    cout << "inside: " <<  player.bMouseInside << endl;
}


void ofApp::mouseReleased(int x, int y, int button){
    player.bMouseInside = false;
    cout << "inside: " <<  player.bMouseInside << endl;
}


void ofApp::windowResized(int w, int h) {
    // Reset player position when window resizes
    player.position = glm::vec3(w / 2.0, h / 2.0, 0);
}
//-----------------------------------------//