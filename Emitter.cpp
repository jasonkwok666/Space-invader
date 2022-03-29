#include "Emitter.h"
#include "ofMain.h"

//Emitter Class provided by Professor

//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;
	lifespan = 3000;    // milliseconds
	started = false;

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;
	childWidth = 10;
	childHeight = 10;
}

//  Draw the Emitter if it is drawable. 
//
//
void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
		}
		else {
			ofSetColor(0, 0, 0);
			ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
		}
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. 
//
void Emitter::update() {
	if (!started) return;

	float time = ofGetElapsedTimeMillis();
	if ((time - lastSpawned) > (1000.0 / rate)) {
		// spawn a new sprite
		Sprite sprite;
		if (haveChildImage) sprite.setImage(childImage);
		sprite.velocity = velocity;
		sprite.lifespan = lifespan;
		sprite.setPosition(trans);
		sprite.birthtime = time;
		sprite.width = childWidth;
		sprite.height = childHeight;
		sys->add(sprite);
		lastSpawned = time;
	}
	sys->update();
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
	childWidth = img.getWidth();
	childHeight = img.getHeight();
}

void Emitter::setImage(ofImage img) {
	image = img;
}

void Emitter::setRate(float r) {
	rate = r;
}

float Emitter::maxDistPerFrame() {
	return  velocity.length() / ofGetFrameRate();
}