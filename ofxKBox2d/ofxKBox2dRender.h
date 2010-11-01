#pragma once

#include "ofMain.h"
#include "Box2D.h"

#define PTM_RATIO 30.0f

class ofxKBox2dRender : public b2DebugDraw {
	
public:
	
	float scaleFactor;
	
	void setScale(float f) {
		scaleFactor = f;
	}
	
	void DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
		printf("draw polygon");
	}
	
	void DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
		
		ofEnableAlphaBlending();
		ofSetColor(color.r * 255, color.g * 255, color.b * 255);
		ofBeginShape();
		
		for(int i=0;i < vertexCount;i++) {
			ofVertex(vertices[i].x*PTM_RATIO, vertices[i].y*PTM_RATIO);
		}
		ofEndShape();
		
	}
	
	void DrawCircle (const b2Vec2 &center, float32 radius, const b2Color &color) {
		printf("draw circle");
	}
	
	void DrawSolidCircle (const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {
		printf("draw solid circle");
	}
	
	void DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
		printf("draw segment");
	}
	
	void DrawTransform (const b2Transform &xf) {
		printf("draw transform");
	}
};