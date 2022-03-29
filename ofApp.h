#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Sprite.h"
#include "Emitter.h"
#include "Bullet.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include <cstdio>
#include <ctime>
#include<vector>


//  General Sprite class  (similar to a Particle)
//

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//Sprite movements
		void updateSprite();
		void moveSprite(MoveDir);
		void stopSprite();
		void startAccel();
		void stopAccel();
		float modulateAccel(float);
		
		//check collision between bullet and invader
		void checkCollisions();

		//is sprite dead or no
		void iamDead();

		//health
		void whatsmyhealth();
		

		//sprite
		Sprite sprite;
		ofVec3f start_point;
		float start_time;
		float finish_time;
		ofVec3f mouse_last;
		MoveDir moveDir;
		bool accel;
		ofVec3f startAccelPoint;
		ofSoundPlayer bgm;

		//Bullet	
		Bullet bullet;
		ofVec3f  bullet_start;
		vector<Bullet> playerBullets;
		
		//GameState
		bool gameState;

		//Shot rate
		clock_t finalShot;
		int shotRate;
		int shotsFired;
		bool checkFire;

		//Emitter (by professor)
		Emitter *invaders, *invaders2;
		int score;
		int difficulty = 1;
		float gameStartTime;
		bool gameOver = false;
		ofVec3f mouseLast;
		ofImage defaultImage;
		bool imageLoaded;
		vector<Sprite> sprites;
		
		//background image
		ofImage spark;
		
		//sprite hirt and sprite life
		int s1, s2, spriteHurt;
		int sh1, sh2;
		int spritelife;
		int invaderVelo;

		//explosion Emitters
		ofEasyCam    cam;
		ParticleEmitter explosive1;
		ParticleEmitter explosive2;
		ofSoundPlayer explosion;
		int tempScore;

		//health
		ofImage health;

		
};
