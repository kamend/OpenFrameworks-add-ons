#ifndef _OFX_KCOORDS_TRANSFORM
#define _OFX_KCOORDS_TRANSFORM

/*

Class for various coordinate transformations 
 
Credits: Theo Wathson / L.A.S.E.R. Tag code
 
*/

#include "ofxOpenCv.h"


class ofxKCoordsTransform {
	
public:
	ofxKCoordsTransform() {
		translate = cvCreateMat(3,3,CV_32FC1);
		matrixCalculated = false;
	}
	
	
	// calculate transformation matrix for two QUADs
	
	void calculateMatrix(CvPoint2D32f src_points[4],CvPoint2D32f dst_points[4]) {
		cvSetZero(translate);
		
		for(int i=0;i<4;i++) {
			cvsrc[i].x = src_points[i].x;
			cvsrc[i].y = src_points[i].y;
			cvdst[i].x = dst_points[i].x;
			cvdst[i].y = dst_points[i].y;
		}
		
		cvWarpPerspectiveQMatrix(cvsrc,cvdst,translate); // calculate homography		
		matrixCalculated = true;
	}
	
	CvPoint2D32f transformCoords(CvPoint2D32f &coordsIn) {
		
		CvPoint2D32f coordsOut;
		
		float *data = translate->data.fl;
		
		float a = data[0];
		float b = data[1];
		float c = data[2];
		float d = data[3];
		
		float e = data[4];
		float f = data[5];
		float i = data[6];
		float j = data[7];
		
		coordsOut.x = ((a*coordsIn.x + b*coordsIn.y+c)/(i*coordsIn.x + j*coordsIn.y +1));
		coordsOut.y = ((d*coordsIn.x + e*coordsIn.y +f)/(i*coordsIn.x + j*coordsIn.y +1));
		
		return coordsOut;
	}	
	
};	

protected:
	CvPoint2D32f cvsrc[4];
	CvPoint2D32f cvdst[4];
	CvMat *translate;
	bool matrixCalculated;
	


#endif
