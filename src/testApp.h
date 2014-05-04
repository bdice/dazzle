#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"

class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();
    void exit();

	ofVec3f jointPosition(ofxOpenNIUser* user, Joint j);
	void drawLimbConnection(ofVec3f firstPoint, ofVec3f secondPoint, ofVec3f thirdPoint, ofVec3f fourthPoint);
	void drawTriangle(ofVec3f firstPoint, ofVec3f secondPoint, ofVec3f thirdPoint);

    void userEvent(ofxOpenNIUserEvent & event);

	ofxOpenNI openNIDevice;

    ofTrueTypeFont verdana;

};

#endif
