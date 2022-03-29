#include "Sprite.h"

//Sprite System provided by Professor

BaseObject::BaseObject() {
	trans = ofVec3f(0, 0, 0);
	scale = ofVec3f(1, 1, 1);
	rot = 0;
}

void BaseObject::setPosition(ofVec3f pos) {
	trans = pos;
}

//
// Basic Sprite Object
//
Sprite::Sprite() {
	speed = 0;
	velocity = ofVec3f(0, 0, 0);
	lifespan = -1;      // lifespan of -1 => immortal 
	birthtime = 0;
	bSelected = false;
	haveImage = false;
	name = "UnamedSprite";
	width = 20;
	height = 20;

	trans.x = 0;
	trans.y = 0;
	scale.x = 1.0;
	scale.y = 1.0;
	boom.loadSound("GUN.WAV");
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

//   image for the sprite. 
//
void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}



void Sprite::draw() {

	ofSetColor(255, 255, 255, 255);

	if (haveImage) {
		
		image.draw(trans.x, trans.y);
	}
	else {
		
		ofNoFill();
		ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0 + trans.x, -height / 2.0 + trans.y, width, height);
	}
}

//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i);
}

// remove all sprites within a given dist of point, return number removed
//
int SpriteSystem::removeNear(ofVec3f point, float dist, ParticleEmitter* p) {
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;
	int count = 0;

	while (s != sprites.end()) {
		ofVec3f v = s->trans - point;
		if (v.length() < dist) 
		{
			ParticleEmitter* pe = new ParticleEmitter();
			pe->type = DirectionalEmitter;
			pe->setPosition(s->trans);
			pe->sys->addForce(new TurbulenceForce(ofVec3f(-2, -1, -3), ofVec3f(1, 2, 5)));
			ImpulseRadialForce* f = new ImpulseRadialForce(100);
			f->s = "radial";
			//	emitter.sys->addForce(new GravityForce(ofVec3f(0, -gravity, 0)));
			pe->sys->addForce(f);
			pe->setOneShot(true);
			pe->setGroupSize(50);
			pe->setVelocity(ofVec3f(0, 0, 0));
			pe->start();
			tmp = sprites.erase(s);
			count++;
			s = tmp;
			delete pe;
		}
		else s++;
	}
	return count;
}





//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteSystem::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
	}
}

void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}