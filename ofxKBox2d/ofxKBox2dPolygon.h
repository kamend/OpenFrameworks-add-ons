#pragma once


class ofxKBox2dPolygon : public ofxKBox2dBasicShape {

private:
	ofPoint centroid;
	int vectorCount;
	vector <ofPoint> vertices;
	
	b2FixtureDef shapeDef;
	b2PolygonShape shape;
	
	ofColor backColor;
public:	
	ofxKBox2dPolygon() { }
	~ofxKBox2dPolygon() { }
	
	void setBackColor(float r, float g, float b) {
		backColor.r = r;
		backColor.g = g;	
		backColor.b = b;
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
		
		b2Vec2 polyverts[vertices.size()];
		
		for(int i=0;i<vertices.size();i++) {
			polyverts[i] = b2Vec2(vertices[i].x/PTM_RATIO,vertices[i].y/PTM_RATIO);
		}
		
		shape.Set(polyverts,vertices.size());
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
		
		setBackColor(255,255,255);
	
	}

	
	void addVertex(float x, float y) {
		vertices.push_back(ofPoint(x,y));
	}

	void addVertex(ofPoint p) {
		vertices.push_back(p);
	}
	
	void clearVertices() {
		vertices.clear();
	}
	
	int getVertexCount() {
		return vertices.size();
	}
		
	float getArea() {
		int i, j, n = vertices.size();
		float area = 0;
		
		for (i = 0; i < n; i++) {
			j = (i + 1) % n;
			area += vertices[i].x * vertices[j].y;
			area -= vertices[j].x * vertices[i].y;
		}
		area /= 2.0;
		return (area);			
	}
	
	ofPoint getCenter() {
		float cx = 0, cy = 0;
		float area = getArea();
		int i, j, n = vertices.size();
		
		float factor = 0;
		for (i = 0; i < n; i++) {
			j = (i + 1) % n;
			factor = (vertices[i].x * vertices[j].y
					  - vertices[j].x * vertices[i].y);
			cx += (vertices[i].x + vertices[j].x) * factor;
			cy += (vertices[i].y + vertices[j].y) * factor;
		}
		area *= 6.0f;
		factor = 1 / area;
		cx *= factor;
		cy *= factor;
		ofPoint pt(cx, cy);
		return pt;		
	}
		
	b2Vec2 computeCentroid(const b2Vec2* vs, int32 count) {
		
		//b2Assert(count >= 3);
		
		b2Vec2 c; c.Set(0.0f, 0.0f);
		float32 area = 0.0f;
		
		// pRef is the reference point for forming triangles.
		// It's location doesn't change the result (except for rounding error).
		b2Vec2 pRef(0.0f, 0.0f);
		
		
#if 0
		// This code would put the reference point inside the polygon.
		for (int32 i = 0; i < count; ++i)
		{
			pRef += vs[i];
		}
		pRef *= 1.0f / count;
#endif
		
		
		const float32 inv3 = 1.0f / 3.0f;
		
		for(int32 i=0; i<count; ++i) {
			
			// Triangle vertices.
			b2Vec2 p1 = pRef;
			b2Vec2 p2 = vs[i];
			b2Vec2 p3 = i + 1 < count ? vs[i+1] : vs[0];
			
			b2Vec2 e1 = p2 - p1;
			b2Vec2 e2 = p3 - p1;
			
			float32 D = b2Cross(e1, e2);
			
			float32 triangleArea = 0.5f * D;
			area += triangleArea;
			
			// Area weighted centroid
			c += triangleArea * inv3 * (p1 + p2 + p3);
		}
		
		// Centroid
		//b2Assert(area > B2_FLT_EPSILON);
		c *= 1.0f / area;
		return c;
	}
	
	bool validatePoly() {
	
		int count = vertices.size();
		b2Vec2 vertexArray[count];
		
		for(int i=0;i<count;i++) {
			vertexArray[i] = b2Vec2(vertices[i].x,vertices[i].y);
		}
		
		b2Vec2 center = computeCentroid(vertexArray, count);
		
		if(count < 3 && count >= b2_maxPolygonVertices) {
			ofLog(OF_LOG_NOTICE, " -- polygon has too many/not enough verticies --");
			return false;
		}
		
		// normals
		b2Vec2 v_normals[b2_maxPolygonVertices];
		
		for(int i=0; i<count; i++) {
			int i1 = i;
			int i2 = i + 1 < count ? i + 1 : 0;
			b2Vec2 edge = vertexArray[i2] - vertexArray[i1];
			if (edge.LengthSquared() <= b2_epsilon * b2_epsilon) {
				return false;
			}
			
			v_normals[i] = b2Cross(edge, 1.0f);
			v_normals[i].Normalize();
		}
		// Ensure the polygon is convex.
		for(int i=0; i<count; i++) {
			for(int j=0; j<count; j++) {
				// Don't check vertices on the current edge.
				if (j == i || j == (i + 1) % count) {
					continue;
				}
				
				// Your polygon is non-convex (it has an indentation).
				// Or your polygon is too skinny.
				float32 s = b2Dot(v_normals[i], vertexArray[j] - vertexArray[i]);
				if(s >= -b2_linearSlop) return false;
			}
		}		
		
		
	}
	
	void draw() {
		
		if(!body->IsActive()) return;
		
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