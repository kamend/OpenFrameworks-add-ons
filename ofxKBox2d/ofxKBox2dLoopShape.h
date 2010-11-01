#pragma once


class ofxKBox2dLoopShape : public ofxKBox2dBasicShape {
private:
		vector <b2Vec2> vertices;
	
		b2FixtureDef shapeDef;
		b2LoopShape shape;
public:	
	ofxKBox2dLoopShape() { init(); }
	~ofxKBox2dLoopShape() {  }
	
	void init() {
		vertices.clear();
	}
	
	void setup(b2World *bworld, float x, float y,bool isFixed=true) {	
		
		if(bworld == NULL) {
			ofLog(OF_LOG_NOTICE," -- polygon: world is NULL -- ");
			return;
		}
		
		if(vertices.size() < 3 ) {
			ofLog(OF_LOG_NOTICE," -- not enough vertices to form a shape --");
			return;
		}
		
		world = bworld;
		
		bodyDef.position.Set( x/PTM_RATIO, y/PTM_RATIO);
				
		b2Vec2 *vec = &vertices[0];
		
		shape.Create(vec,vertices.size());
		shapeDef.shape = &shape;
		
		if(isFixed) {
			shapeDef.density = 0.0f;
			shapeDef.friction = 0.6f;
			shapeDef.restitution = 0.0f;
		} else {
			shapeDef.density = density;
			shapeDef.friction = friction;
			shapeDef.restitution = restitution;
			bodyDef.type = b2_dynamicBody;
		}
		
		body = world->CreateBody(&bodyDef);
		
		body->CreateFixture(&shapeDef);
		
		//	ofLog(OF_LOG_NOTICE,"-- polygon shape created --");
		
		dead = false;		
		
	}
	
	void addVertex(float x, float y) {
		vertices.push_back(b2Vec2(x,y));
	}

	void addVertex(b2Vec2 p) {
		vertices.push_back(p);
	}
	
	void addVertex(ofPoint p) {
		vertices.push_back(b2Vec2(p.x,p.y));
	}
	
	void clearVertices() {
		vertices.clear();
	}
	
	int getVertexCount() {
		return vertices.size();
	}	
};