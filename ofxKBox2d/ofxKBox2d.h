#pragma once

#include "ofMain.h"
#include "Box2D.h"

#include "ofxKBox2dSettings.h"
#include "ofxKBox2dRender.h"

#include "ofxKBox2dBasicShape.h"
#include "ofxKBox2dRect.h"
#include "ofxKBox2dPolygon.h"
#include "ofxKBox2dLoopShape.h"
#include "ofxKBox2dLine.h"


class ofxKBox2d {
private:
	float fps;
	int velocityIterations;
	int positionIterations;
	
public:
	b2World *world;
	
	ofPoint gravity;
	
	float doSleep;
	bool bWorldCreated;
	
	b2Body* ground;
	
	ofxKBox2dRender drawRender;
	
	ofxKBox2d();	
	~ofxKBox2d();
	void init();
	void setFPS(float frameRate) { fps = frameRate; }
	void createFloor(float floorWidth,float bottomPosition);
	void createBounds(float x, float y, float w, float h);
	
	void setGravity(float x, float y);
	void setGravity(ofPoint grav);
	
	int32 getBodyCount() { return world->GetBodyCount(); }
			
	b2World* getWorld() { return world; } 
	
	void update();
	void draw();

	
};
