#include "Bullet.h"

Bullet::Bullet() {
	trans.x = 0;
	trans.y = 0;
	scale.x = 1.0;
	scale.y = 1.0;
	rot = 0;
	speed = 0;
	bSelected = false;
}

void Bullet::draw()
{
	ofSetColor(255, 255, 255, 255);
	//image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
	image.draw(trans.x, trans.y);
	//image.resize(30, 30);
}

//direction (up in this case) and movement of bullet
void Bullet::updateBullet(Sprite &s)
{
	float dist = s.speed * 2 / ofGetFrameRate();
	ofVec3f dir;
	ofRectangle r = ofGetWindowRect();
	dir = ofVec3f(0, -dist, 0);
	this->trans += dir;
}