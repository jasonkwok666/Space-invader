#pragma once


#include "BaseObject.h"
#include "Sprite.h"

//create a bullet
class Bullet : public BaseObject {

public:
	Bullet();
	void draw();
	float speed;
	ofImage image;
	void updateBullet(Sprite& s);
};
