#pragma once
#include "BaseObject.h"
#include "ofMain.h"
#include "ParticleEmitter.h"

class Sprite : public BaseObject {
public:
	Sprite();
	void draw();
	float age();
	void setImage(ofImage);
	float speed;    //   in pixels/sec
	ofVec3f velocity; // in pixels/sec
	ofImage image;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	string name;
	bool haveImage;
	float width, height;
	ofSoundPlayer boom;
};

//  Manages all Sprites in a system.  
//
class SpriteSystem {
public:
	void add(Sprite);
	void remove(int);
	void update();
	int removeNear(ofVec3f point, float dist, ParticleEmitter* p);
	void draw();
	vector<Sprite> sprites;


	
	

};
