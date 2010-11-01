#pragma once

#include "ofMain.h"
#include "Box2D.h"

class ofxKBox2dBasicShape {

public:
	
	float mass;
	float density;
	float restitution;
	float friction;
	float dead;	
	float isFixed;
	
	b2World *world;
	b2Body *body;	
	b2BodyDef bodyDef;	
	
	// some color/alpha properties
	
	float alpha;
	
	
		
	ofxKBox2dBasicShape() {
		
		dead = true;
		isFixed = false;
		
		world = NULL;
		body = NULL;
		
		friction = 0.0f;
		mass = 0.0f;
		restitution = 0.0f;
		
		alpha = 255;
		
		bodyDef.allowSleep = true;
	}

	void setPhysics(float d, float r, float f) {
		density = d; restitution = r; friction = f;
	}
	
	void setAlpha(float a){
		alpha = a;
	}
	
	ofPoint getPosition() {
		ofPoint p;
		if(body != NULL) {
			p.set(body->GetPosition().x * PTM_RATIO,body->GetPosition().y * PTM_RATIO,0.0f);
			return p;
		} 
		return p;
	}
	
	~ofxKBox2dBasicShape() {
		ofLog(OF_LOG_NOTICE, "destructor called");
		if(!dead) destroyShape();
	}
	
	void destroyShape() {
		
		if(world == NULL) {
			ofLog(OF_LOG_NOTICE,"-- world does not exists --");
			return;
		} else if(!body) {
			ofLog(OF_LOG_NOTICE,"-- body appears to be dead --");
			return;
		} else if(dead) {
			ofLog(OF_LOG_NOTICE,"-- already dead --");
			return;
		}	
		
		b2Fixture *sd = body->GetFixtureList();
		
		while(sd) {
			b2Fixture *b = sd;
			sd = sd->GetNext();
			
			body->DestroyFixture(b);
		}
			
		world->DestroyBody(body);
		body = NULL;
		dead = true;
		
	//	ofLog(OF_LOG_NOTICE, "dead!");
		
		
	}
	
	void draw() {
	
	}
};
