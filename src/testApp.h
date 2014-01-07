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

    void userEvent(ofxOpenNIUserEvent & event);

	ofxOpenNI openNIDevice;

    ofTrueTypeFont verdana;

};

#endif
