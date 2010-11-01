/*
 *  ofxKBox2d.cpp
 *  ofBox2DFun
 *
 *  Created by Kamen Dimitrov on 10/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxKBox2d.h"

ofxKBox2d::ofxKBox2d() {
	
}

ofxKBox2d::~ofxKBox2d() {
	if(world == NULL || !bWorldCreated) {
		ofLog(OF_LOG_NOTICE,"-- world is already dead --");
		return;
	}

	if(world->GetBodyCount() > 0) {
		ofLog(OF_LOG_NOTICE,"-- there are still alive bodies!! --");
		return;
	}
	
	delete world;
	
	bWorldCreated = false;
}

void ofxKBox2d::init() {
	
	// default values
	bWorldCreated		= false;
	velocityIterations  = 40;
	positionIterations  = 20;
	doSleep				= true;
	
	setFPS(60);
	gravity.set(0.0f,9.81f);
	
	// debugdraw
	
	drawRender.setScale(PTM_RATIO);
	drawRender.SetFlags(1);
	
	world = new b2World(b2Vec2(gravity.x,gravity.y), doSleep);
	
	bWorldCreated = true;
	
	world->SetDebugDraw(&drawRender);
}

void ofxKBox2d::createFloor(float floorWidth,float bottomPosition) {
	
	if(!bWorldCreated) return;
	
	float thick = 2 / PTM_RATIO;
	
	b2BodyDef bdef;
	
	bdef.position.Set(0.0f,0.0f);
	
	ground = world->CreateBody(&bdef);
	
	b2PolygonShape ps;
	
	ps.SetAsBox((floorWidth / PTM_RATIO)/2, thick, b2Vec2((floorWidth/PTM_RATIO)/2, (bottomPosition - thick)/PTM_RATIO), 0.0);

	b2FixtureDef fdef;
	
	fdef.density = 0.0f;
	fdef.restitution = 0.0f;
	fdef.friction = 0.6f;
	fdef.filter.groupIndex = 1;
	fdef.shape = &ps;	
	
	ground->CreateFixture(&fdef);
	
}


void ofxKBox2d::createBounds(float x, float y, float w, float h) {
	
	if(!bWorldCreated) return;
	
	float thick = 2 / PTM_RATIO;
	
	b2BodyDef bdef;
	
	bdef.position.Set(0,0);
	
	ground = world->CreateBody(&bdef);

	b2FixtureDef fd;
	
	b2PolygonShape ps;
	
	fd.density = 0.0f;
	fd.friction = 0.6f;
	fd.restitution = 0.0f;
	fd.shape = &ps;	
		
	// right
	ps.SetAsBox(thick/2,(h/PTM_RATIO)/2,b2Vec2(w/PTM_RATIO,(h/PTM_RATIO)/2),0.0);
	ground->CreateFixture(&fd);
	
	// bottom
	ps.SetAsBox(w/2/PTM_RATIO,thick/2,b2Vec2((w/2)/PTM_RATIO,(h/PTM_RATIO)),0.0);
	ground->CreateFixture(&fd);
	
	// left
	ps.SetAsBox(thick/2,(h/2)/PTM_RATIO,b2Vec2(thick/2,(h/2)/PTM_RATIO),0.0);
	ground->CreateFixture(&fd);
	
	// top
	ps.SetAsBox((w/2)/PTM_RATIO,thick/2,b2Vec2( (w/2)/PTM_RATIO,thick/2),0.0);
	ground->CreateFixture(&fd);
}

void ofxKBox2d::update() {
	
	float	timeStep  = (1.0f / fps);
	world->Step(timeStep,velocityIterations,positionIterations);
	world->ClearForces();

}

void ofxKBox2d::draw() {
	world->DrawDebugData();
}

