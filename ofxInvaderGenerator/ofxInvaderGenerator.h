/*
 *  InvaderGenerator.h
 *  ofInvaderGenerator
 *
 *  Created by Kamen Dimitrov on 10/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

class ofxInvaderGenerator {
	
	int* invaderMatrix;
	int matrixSize;
	ofColor invaderColor;
	
public: 	
	ofxInvaderGenerator() {
		matrixSize = 8;
		invaderMatrix = new int[matrixSize*matrixSize];
		invaderMatrix = generate(matrixSize);
		invaderColor = ofColor();
	}
	
	int shuffle() {
		invaderMatrix = generate(matrixSize);
	}
	
	int *getMatrix() {
		return invaderMatrix;
	}
	
	int getSize() {
		return matrixSize;
	}
	
	int* getRandomLine(int length,int min_filled, int max_filled) {
		int* r = new int[length];
		
		ofSeedRandom();
		
		int filled=0;
		
		for(int i=0;i<length;i++) {
			r[i] = round(ofRandom(0,1))*255;
			if(r[i] > 0) filled++;
		}
		
		if(filled < min_filled || filled > max_filled) {
			return getRandomLine(length,min_filled,max_filled);
		}
		
		return r;
	}
	
	void flipValue(int &v) {
		if(v == 0) v = 255;
		else v = 0;
	}
	
	int* generate(int size) {
		
		int* invader = new int[size*size];
		
		for(int i=0;i<size*size;i++) {
			invader[i] = 0;
		}
		
		int ind,opind;
		int opx,opy;
		
		for(int y=1;y<size-1;y++) {
			int* t = new int[size/2];
			
			if(y == 1 || y == 2) {
				t = getRandomLine(size/2,size/2/2,size/2/2);
			} else 
				if(y == 3 || y == 4) {
					t = getRandomLine(size/2,3,4);
				} else {
					t = getRandomLine(size/2,2,3);
				}
			
			int tempi = 0;		
			for(int x=1;x<size/2;x++) {
				
				ind = x + (y * size);
				opx = 7 - x;
				opy = y;
				opind = opx +(opy*size);
				
				invader[ind] = t[x];
				invader[opind] = t[x];
				
				tempi++;
			}
			
			delete(t);
		}
		
		return invader;
	}	
	
	void setColor(int r,int g, int b) {
		invaderColor.r = r;
		invaderColor.g = g;
		invaderColor.b = b;
		
	}
	
	void draw(int x, int y, int rectSize,bool show_grid=true) {
			
		int startX = x;
		int startY = y;
		int indX=0;
		int indY=0;
		int aind=0;
			
		for(int x=startX; x < (rectSize*matrixSize+startX);x+=rectSize) {
			indY=0;
			for(int y=startY;y < (rectSize*matrixSize+startY);y+=rectSize){
				
				aind = indX + (indY*matrixSize);
				
				if(invaderMatrix[aind] == 0)  {
					if(show_grid) {
						ofNoFill();
						ofSetColor(255,255,255);
						ofRect(x,y,rectSize,rectSize);
					}
				}
				else {
					ofFill();
					ofSetColor(invaderColor.r,invaderColor.g,invaderColor.b);
					ofRect(x,y,rectSize,rectSize);		
				}
				
				indY++;
			}
			indX++;
		}		
	}
	
	
	
};


