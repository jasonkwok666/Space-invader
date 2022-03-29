#include "ofApp.h"
#include "Emitter.h"
#include <stdlib.h>

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	spark.loadImage("images/space.jpg");
	explosion.loadSound("Explosion.wav");
	bgm.loadSound("game.wav");
	ofSetBackgroundAuto(false);
	spark.resize(ofGetWindowWidth(), ofGetWindowHeight());
	//ofBackground(0, 0, 0);
	ofSetWindowShape(500, 500);

	// Record when the game starts - game will run for 10 sec
	//
	gameStartTime = ofGetElapsedTimeMillis();
	spritelife = 10;

	s1 = 0;
	s2 = 0;
	spriteHurt = 0;
	sh1 = 0;
	sh2 = 0;

	//create emmiters
	invaders = new Emitter(new SpriteSystem());
	invaders2 = new Emitter(new SpriteSystem());
	if (defaultImage.loadImage("images/invader1.png")) {
		defaultImage.resize(30, 30);
		invaders->setChildImage(defaultImage);
		imageLoaded = true;
	}
	else imageLoaded = false;

	if (defaultImage.loadImage("images/invader2.png")) {
		defaultImage.resize(30, 30);
		invaders2->setChildImage(defaultImage);
		imageLoaded = true;
	}
	else imageLoaded = false;

	if (defaultImage.loadImage("images/sprite.png")) {
		defaultImage.resize(80, 80);
		sprite.setImage(defaultImage);
		imageLoaded = true;
	}
	else imageLoaded = false;

	//health
	health.loadImage("images/health.png");
	health.resize(30, 150);
	
	score = 0;
	start_point = ofVec3f(sprite.image.getWidth() + 50, sprite.image.getHeight() + 270);
	sprite.trans.set(start_point);
	sprite.speed = 190; // in pixels per second (screenspace 1 unit = 1 pixel)
						//bullet.speed = 120;
	moveDir = MoveStop;

	gameState = false;

	shotsFired = 0;
	finalShot = 0;
	shotRate = 3;
	checkFire = false;

	invaderVelo = 400;

	invaders->setPosition(ofVec3f(ofGetWindowWidth() / 4, 20, 0));
	invaders->velocity.set(0, invaderVelo, 0);
	invaders->setLifespan(5000);
	invaders->setRate(2);
	invaders->setChildSize(20, 20);

	invaders2->setPosition(ofVec3f(ofGetWindowWidth() / 1.7, 20, 10));
	invaders2->velocity.set(0, 900, 0);
	invaders2->setLifespan(9000);
	invaders2->setRate(1);
	invaders2->setChildSize(40, 40);

	explosive1.type = DirectionalEmitter;
	explosive2.type = SphereEmitter;
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(120);
	ofSetFrameRate(60);

	explosive1.sys->addForce(new TurbulenceForce(ofVec3f(-2, -1, -3), ofVec3f(1, 2, 5)));
	ImpulseRadialForce* f = new ImpulseRadialForce(100);
	f->s = "radial";
	explosive1.sys->addForce(f);
	explosive1.setOneShot(true);
	explosive1.setGroupSize(50);
	explosive1.setVelocity(ofVec3f(0, 0, 0));
	
	
	

	explosive2.sys->addForce(new TurbulenceForce(ofVec3f(-2, -1, -3), ofVec3f(1, 2, 5)));
	ImpulseRadialForce* f1 = new ImpulseRadialForce(100);
	f1->s = "spherical";
	explosive2.sys->addForce(f1);
	explosive2.setOneShot(true);
	explosive2.setGroupSize(50);
	explosive2.setVelocity(ofVec3f(0, 0, 0));
	

	// start them up
	//
	invaders->start();
	invaders2->start();

	
}
void ofApp::checkCollisions() {
	


	//check enemy 1 collision
	float collisionDist1 = bullet.image.getHeight() / 2 + invaders->childHeight / 2;
	
	
	for (int i = 0; i < playerBullets.size(); i++) {
		int x= invaders->sys->removeNear(playerBullets[i].trans, collisionDist1, &explosive1);
		s1 += x;
		
		if (x > 0) {
			explosive1.sys->reset();
			explosive1.start();
			explosive2.sys->reset();
			explosive2.start();
		}
		
		//increase difficulty
		invaders->velocity.set(0, invaderVelo +20, 0);
	}
	
	//check enemy 2 collision
	float collisionDist2 = bullet.image.getHeight() / 2 + invaders2->childHeight / 2;
	for (int i = 0; i < playerBullets.size(); i++) {
		int y= invaders2->sys->removeNear(playerBullets[i].trans, collisionDist2, &explosive1);
		s2 += y;
		if (y > 0) {
			explosive1.sys->reset();
			explosive1.start();
			explosive2.sys->reset();
			explosive2.start();
		}
	}
	score = s1 + 2*s2;
}




//sprite gets hurt
void ofApp::iamDead() {
	
	float spritecollide1 = sprite.image.getHeight() / 2 + invaders->childHeight / 2;
		sh1 += invaders->sys->removeNear(sprite.trans, spritecollide1, &explosive1);
		

	float spritecollide2 = sprite.image.getHeight() / 2 + invaders2->childHeight / 2;
	sh2 += invaders2->sys->removeNear(sprite.trans, spritecollide2, &explosive1);
	spriteHurt = sh1 + sh2;
		
}


void ofApp::whatsmyhealth()
{
	
		
	if (7.5*spriteHurt >= 150) {
		gameOver = true;
	}
	health.resize(30, 150 - 7.5 * spriteHurt);


	
	
}

//--------------------------------------------------------------
void ofApp::update(){

	if (gameState) {
		updateSprite();
		for (int i = 0; i < playerBullets.size(); i++)
		{
			playerBullets[i].updateBullet(sprite);
		}
		invaders->update();
		invaders2->update();
		checkCollisions();
		iamDead();
		whatsmyhealth();
		ofSeedRandom();
		explosive1.setLifespan(0.001);
		explosive1.setRate(1000);
		explosive1.setParticleRadius(0.1153);
		explosive1.min = 0.1;
		explosive1.max = 1;
		explosive1.update();

		explosive2.setLifespan(0.001);
		explosive2.setRate(1000);
		explosive2.setParticleRadius(0.1153);
		explosive2.min = 0.1;
		explosive2.max = 1;
		explosive2.update();

		//everytime score increses, play explosion sound
		if (score > tempScore) {
			explosion.play();
		}
		tempScore = score;
		if (score > difficulty * 15)
			difficulty++;
		
	}
	 

	ofVec3f v = invaders->velocity;
	invaders->setVelocity(ofVec3f(ofRandom(-v.y / 2, v.y / 2), v.y, v.z));

	ofVec3f v1 = invaders2->velocity;
	invaders2->setVelocity(ofVec3f(ofRandom(-v1.y / 2, v1.y / 2), v1.y, v1.z));

		

}

//--------------------------------------------------------------
void ofApp::draw(){

	spark.draw(0,0);

	// if game is over, just draw a label in middle of screen
	//
	if (!gameState)
	{
		
		ofSetColor(0, 0, 255);
		ofDrawBitmapString("START GAME HIT SPACEBAR", ofPoint(ofGetWindowWidth() / 4, ofGetWindowHeight() / 2));
		
	}
	else {
		for (int i = 0; i < playerBullets.size();i++)
		{
			playerBullets[i].draw();
		}
		sprite.draw();
	}

	if (gameOver) {
		ofSetColor(200, 200, 200);
		ofDrawBitmapString("GAME OVER", ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
		ofDrawBitmapString("SCORE: "+score, ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 4));
	}
	else {
		invaders->draw();
		invaders2->draw();

		cam.begin();
		explosive1.draw();
		explosive2.draw();
		//  end drawing in the camera
		// 
		cam.end();

		health.draw((ofGetWindowWidth() / 2)+200, (ofGetWindowHeight()/2)-230);
		
	}

	// draw current score
	//
	string scoreText;
	scoreText += "Score: " + std::to_string(score);
	ofDrawBitmapString(scoreText, ofPoint(10,20));

	//how many times is our sprite getting hurt
	string spritehurt;
	spritehurt += "Hurted: " + std::to_string(spriteHurt);
	ofDrawBitmapString(spritehurt, ofPoint(10, 40));

	string DifficultyText;
	DifficultyText += "Difficulty level: " + std::to_string(difficulty);
	ofDrawBitmapString(DifficultyText, ofPoint(10, 60));
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_LEFT) {

		moveSprite(MoveLeft);

	}

	if (key == OF_KEY_RIGHT) {


		moveSprite(MoveRight);


	}

	if (key == OF_KEY_UP) {

		moveSprite(MoveUp);

	}

	if (key == OF_KEY_DOWN) {

		moveSprite(MoveDown);
	}

	if (key == 'e')
	{
		moveSprite(MoveDiagonalUpRight);
	}

	if (key == ' ')
	{
		//check for gamestate
		if (!gameState)
		{
			gameState = true;
			bgm.play();
		}
		else {
			/*
			spawn and fire bullet when player hits spacebar
			*/
			if (shotsFired != 0)
			{
				checkFire = true;
				if (1000 * (clock() - finalShot) / CLOCKS_PER_SEC >= 1000 / shotRate)
				{
					Bullet b;
					b = Bullet();
					b.image.loadImage("images/Bullet.png");
					b.image.resize(30, 30);
					b.trans.x = sprite.trans.x + 0.5 * sprite.image.getWidth();
					b.trans.y = sprite.trans.y;
					playerBullets.push_back(b);
					sprite.boom.play();
					shotsFired++;
					
					
					finalShot = clock();
				}
			}
			else {
				Bullet b;
				b = Bullet();
				b.image.loadImage("images/Bullet.png");
				b.image.resize(30, 30);
				b.trans.x = sprite.trans.x + 0.5 * sprite.image.getWidth();
				b.trans.y = sprite.trans.y;
				playerBullets.push_back(b);
				sprite.boom.play();
				shotsFired++;
				
				
			}
		}
	}

	if (key == 'q')
	{
		gameOver = true;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if (key == OF_KEY_LEFT) {

		stopSprite();
		stopAccel();
	}

	if (key == OF_KEY_RIGHT) {

		stopSprite();
		stopAccel();
	}

	if (key == OF_KEY_UP) {

		stopSprite();
		stopAccel();
	}

	if (key == OF_KEY_DOWN) {
		stopSprite();
		stopAccel();
	}
	if (key == 'e')
	{
		stopSprite();
		stopAccel();
	}
	if (key == ' ')
	{
		shotsFired = 0;
		checkFire = false;
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	float dist = sprite.speed * 1 / ofGetFrameRate();
	ofPoint mouse_cur = ofPoint(x, y);
	ofVec3f delta = mouse_cur - mouse_last;
	//if(this->sprite
	//if (mouse_cur.x >= 0 && mouse_cur.x +sprite.image.getWidth() <= ofGetWindowRect().getMaxX() && mouse_cur.y >= 0 && mouse_cur.y +sprite.image.getHeight() <= ofGetWindowRect().getMaxY())

	if (mouse_cur.x >= 0 && mouse_cur.x <= ofGetWindowRect().getMaxX() && mouse_cur.y >= 0 && mouse_cur.y <= ofGetWindowRect().getMaxY())

	{
		sprite.trans += delta;
		mouse_last = mouse_cur;
	}
}
float ofApp::modulateAccel(float dist) {
	return sin(dist * PI) * 5.0 + 1.0;
}


void ofApp::updateSprite() {
	// 
	// calculate distance to travel for this update and cases are to move the sprite in a particular direction based on vector
	//
	float dist = sprite.speed * 1 / ofGetFrameRate();
	ofVec3f dir;
	ofRectangle r = ofGetWindowRect();

	{
		switch (moveDir)
		{
		case MoveUp:
			if (sprite.trans.y - dist < r.getMinY())
			{
				dir = ofVec3f(0, 0, 0);
			}
			else {
				dir = ofVec3f(0, -dist, 0);
			}
			break;
		case MoveDown:
			if (sprite.trans.y + dist + sprite.image.getHeight() > r.getMaxY())
			{
				dir = ofVec3f(0, 0, 0);
			}
			else {
				dir = ofVec3f(0, dist, 0);
			}
			break;
		case MoveLeft:
			if (sprite.trans.x - dist < r.getMinX())
			{
				dir = ofVec3f(0, 0, 0);
			}
			else {
				dir = ofVec3f(-dist, 0, 0);
			}
			break;
		case MoveRight:
			if (sprite.trans.x + dist + sprite.image.getWidth() > r.getMaxX())
			{
				dir = ofVec3f(0, 0, 0);
			}
			else {
				dir = ofVec3f(dist, 0, 0);
			}
			break;
		case MoveDiagonalUpRight:
			dir = ofVec3f(dist, -dist, 0);
		}
	}
	sprite.trans += dir;

}

void ofApp::moveSprite(MoveDir dir) {
	moveDir = dir;

	/*
	Nathan helped me with the logic of shooting by holding spacebar Key.
	*/
	if (checkFire)
	{
		keyPressed(' ');
	}
}

//Stop the Sprite
void ofApp::stopSprite() {
	moveDir = MoveStop;
}

//Accelerate the Sprite
void ofApp::startAccel() {
	startAccelPoint = sprite.trans;
	accel = true;
}

void ofApp::stopAccel() {
	accel = false;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mouse_last = ofPoint(x, y);

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	stopSprite();

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
