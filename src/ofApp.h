#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"

using namespace ofxCv;
using namespace cv;
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
    float myPosToAngle(float x,float y,float cX,float cY);

    ofxSyphonServer     syphonServer;
    ofFbo               syphonFbo;
    
    ofImage               words;
    ofImage               dataImg;
    
    int                 width;
    int                 height;
    
    ofVideoGrabber vidGrabber;
    ofPixels videoInverted;
    ofTexture videoTexture;
    ofImage   vidImg;
    int camWidth;
    int camHeight;
    
    
    ofxCvColorImage     colorImage;
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCv::ContourFinder contourFinder;
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image

    ofPixels previous;
    ofPixels strench;
    ofImage diff;
    cv::Scalar diffMean;
    
    ofxPanel gui;
    ofParameter<bool> bThreshWithOpenCV;
    ofParameter<float> minAreaRadius;
    ofParameter<float> maxAreaRadius;
    ofParameter<float> trackingThreshold;
    ofParameter<int> nearThreshold;
    ofParameter<int> farThreshold;
    ofParameter<int> outRadius;
    ofParameter<int> inRadius;
    
    ofParameter<int> detectCircleCenterX;
    ofParameter<int> detectCircleCenterY;
    
    ofParameter<int> detectStrenchrX;
    



};
