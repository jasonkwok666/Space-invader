#pragma once

#include "ofMain.h"

typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown, MoveDiagonalUpRight, MoveDiagonalUpLeft, MoveDiagonalDownRight, MoveDiagonalDownLeft } MoveDir;

class BaseObject {
public:
	BaseObject();
	ofVec2f trans, scale;
	float	rot;
	bool	bSelected;
	void setPosition(ofVec3f);
};
