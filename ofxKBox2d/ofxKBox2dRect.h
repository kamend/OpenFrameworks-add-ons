#pragma once


class ofxKBox2dRect : public ofxKBox2dBasicShape {

public:
	b2FixtureDef shapeDef;
	b2PolygonShape shape;
	
	float width, height;
	
	ofColor backColor;
	
	ofxKBox2dRect() {}
	
	void setBackColor(float r, float g, float b) {
		backColor.r = r;
		backColor.g = g;	
		backColor.b = b;
	}
	
	void setup(b2World *bworld, float x, float y,float w, float h, bool isFixed=false) {
		
		if(bworld == NULL) {
			ofLog(OF_LOG_NOTICE, "-- world does not exists --");
			return;
		}
		
		width = w;
		height = h;
		
		world = bworld;		
		
		w/=2;
		h/=2;

		shape.SetAsBox(w / PTM_RATIO, h / PTM_RATIO);
		shapeDef.shape = &shape;
		
		x += w;
		y += h;
		
		bodyDef.position.Set(x/PTM_RATIO,y/PTM_RATIO);
		
		if(isFixed) {
			shapeDef.density = 0.0f;
			shapeDef.friction = 0.6f;
			shapeDef.restitution = 0.0f;
		} else {
			shapeDef.density = density;
			shapeDef.friction = friction;
			shapeDef.restitution = restitution;
			bodyDef.type = b2_dynamicBody;
			bodyDef.allowSleep = true;
			bodyDef.awake = true;
			bodyDef.bullet = true;
		}
		
		body = world->CreateBody(&bodyDef);
						   
		body->CreateFixture(&shapeDef);
		
		ofLog(OF_LOG_NOTICE," -- shape created --");
		
		dead = false;
		
		setBackColor(255,255,255);	
	}
	
	void draw(ofImage &im) {
		
		if(!body->IsActive()) return;
		
		int count = shape.GetVertexCount();
		
		b2Vec2 pos = body->GetPosition();
		
		const b2Transform& xf = body->GetTransform();
			
		float32 angle = xf.GetAngle() * (180/b2_pi);
		
		printf("px: %f tx: %f\n",pos.x,xf.position.x);
					
		ofPushMatrix();
		//ofRotate((angle*(180/b2_pi)));
		//printf("angle: %f",angle*(180/b2_pi));
		ofTranslate(xf.position.x*PTM_RATIO, xf.position.y*PTM_RATIO);
		ofRotate(angle);
		im.draw(-20,-20);
				
		ofPopMatrix();
		
		//draw();	
	}

	void draw() {
		
		if(!body->IsActive()) return;
		
		if(!body->IsAwake()) {draw(); return;}
		
		int count = shape.GetVertexCount();
		
		const b2Transform& xf = body->GetTransform();
		b2Vec2 verts[b2_maxPolygonVertices];
		
		b2PolygonShape* poly = &shape;
		int32 vertexCount = poly->m_vertexCount;
		b2Assert(vertexCount <= b2_maxPolygonVertices);
		
		
		for (int32 i = 0; i < vertexCount; ++i)
		{
			verts[i] = b2Mul(xf, poly->m_vertices[i]);
			
		}
		
		ofPushStyle();
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor(backColor.r,backColor.g,backColor.b,alpha);
		ofBeginShape();
		for (int32 i = 0; i <count; i++) {
			ofVertex(verts[i].x*PTM_RATIO, verts[i].y*PTM_RATIO);
		}
		ofEndShape();
		ofDisableAlphaBlending();
		ofPopStyle();
		

	}

};