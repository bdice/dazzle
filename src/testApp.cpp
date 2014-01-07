#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {

    ofSetLogLevel(OF_LOG_VERBOSE);

    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(2);
    openNIDevice.start();

    // set properties for all user masks and point clouds
    //openNIDevice.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers(2); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel

    // you can alternatively create a 'base' user class
//    ofxOpenNIUser user;
//    user.setUseMaskTexture(true);
//    user.setUsePointCloud(true);
//    user.setPointCloudDrawSize(2);
//    user.setPointCloudResolution(2);
//    openNIDevice.setBaseUserClass(user);

    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofPushMatrix();
	ofTranslate(ofGetWindowWidth()/2,ofGetWindowHeight()/2, -2000);
    ofDrawGrid(3000.0f, 5.0f, false, true, true, true);

    // get number of current users
    int numUsers = openNIDevice.getNumTrackedUsers();

    // iterate through users
    for (int i = 0; i < numUsers; i++){
        // get a reference to this user
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
        ofVec3f headPos = jointPosition(&user, JOINT_HEAD);
        ofVec3f torsoPos = jointPosition(&user, JOINT_TORSO);
        ofVec3f lHandPos = jointPosition(&user, JOINT_LEFT_HAND);
        ofSetColor(255, 255, 255);
        ofDrawSphere(headPos.x, headPos.y, headPos.z, 100.0f);
        ofSetColor(255, 255, 0);
        ofDrawSphere(torsoPos.x, torsoPos.y, torsoPos.z, 100.0f);
        ofSetColor(0, 255, 0);
        ofDrawSphere(lHandPos.x, lHandPos.y, lHandPos.z, 100.0f);
    }
    ofPopMatrix();

    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
ofVec3f testApp::jointPosition(ofxOpenNIUser* user, Joint j){
    ofVec3f worldPos = (*user).getJoint(j).getWorldPosition();
    worldPos.y *= -1;
    worldPos.z *= -1;
    return worldPos;
}
