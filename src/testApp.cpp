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

    // Having OpenNI crashes? Try setting [Server] UseServer=0 in "Program Files\PrimeSense\NITE\Features_1_5_2\Data\FeatureExtraction.ini"

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
	ofSetDepthTest(true);
	ofTranslate(ofGetWindowWidth()/2,ofGetWindowHeight()/2, -1000);

    // get number of current users
    int numUsers = openNIDevice.getNumTrackedUsers();

    // iterate through users
    for (int i = 0; i < numUsers; i++){
        int userHue = (80+i*57) % 256;
        // Get a reference to this user
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
        ofVec3f headPos = jointPosition(&user, JOINT_HEAD);
        ofVec3f neckPos = jointPosition(&user, JOINT_NECK);
        ofVec3f torsoPos = jointPosition(&user, JOINT_TORSO);
        ofVec3f lShoulderPos = jointPosition(&user, JOINT_LEFT_SHOULDER);
        ofVec3f lElbowPos = jointPosition(&user, JOINT_LEFT_ELBOW);
        ofVec3f lHandPos = jointPosition(&user, JOINT_LEFT_HAND);
        ofVec3f rShoulderPos = jointPosition(&user, JOINT_RIGHT_SHOULDER);
        ofVec3f rElbowPos = jointPosition(&user, JOINT_RIGHT_ELBOW);
        ofVec3f rHandPos = jointPosition(&user, JOINT_RIGHT_HAND);
        ofVec3f lHipPos = jointPosition(&user, JOINT_LEFT_HIP);
        ofVec3f lKneePos = jointPosition(&user, JOINT_LEFT_KNEE);
        ofVec3f lFootPos = jointPosition(&user, JOINT_LEFT_FOOT);
        ofVec3f rHipPos = jointPosition(&user, JOINT_RIGHT_HIP);
        ofVec3f rKneePos = jointPosition(&user, JOINT_RIGHT_KNEE);
        ofVec3f rFootPos = jointPosition(&user, JOINT_RIGHT_FOOT);

        ofSetColor(ofColor::fromHsb(userHue, 255, 255));
        ofSetLineWidth(5.0);
        // Arms
        drawLimbConnection(neckPos, lShoulderPos, lElbowPos, lHandPos);
        drawLimbConnection(neckPos, rShoulderPos, rElbowPos, rHandPos);
        drawLimbConnection(lShoulderPos, lElbowPos, lHandPos, lHandPos);
        drawLimbConnection(rShoulderPos, rElbowPos, rHandPos, rHandPos);

        //Torso
        ofFill();
        ofVec3f chestFrontPos = torsoPos + 100.0*((lShoulderPos - torsoPos).getCrossed(rShoulderPos - torsoPos)).normalize();
        ofVec3f chestBackPos = torsoPos - 100.0*((lShoulderPos - torsoPos).getCrossed(rShoulderPos - torsoPos)).normalize();
        ofSetColor(ofColor::fromHsb(userHue+21, 200, 240));
        drawTriangle(chestFrontPos, neckPos, lShoulderPos);
        drawTriangle(chestBackPos, neckPos, lShoulderPos);
        ofSetColor(ofColor::fromHsb(userHue-15, 240, 255));
        drawTriangle(chestFrontPos, neckPos, rShoulderPos);
        drawTriangle(chestBackPos, neckPos, rShoulderPos);
        ofSetColor(ofColor::fromHsb(userHue+10, 245, 235));
        drawTriangle(chestFrontPos, lHipPos, lShoulderPos);
        drawTriangle(chestBackPos, lHipPos, lShoulderPos);
        ofSetColor(ofColor::fromHsb(userHue-17, 255, 215));
        drawTriangle(chestFrontPos, rHipPos, rShoulderPos);
        drawTriangle(chestBackPos, rHipPos, rShoulderPos);
        ofSetColor(ofColor::fromHsb(userHue+7, 193, 217));
        drawTriangle(chestFrontPos, lHipPos, rHipPos);
        drawTriangle(chestBackPos, lHipPos, rHipPos);

        // Legs
        ofNoFill();
        ofSetColor(ofColor::fromHsb(userHue, 255, 180));
        drawLimbConnection(lHipPos, lHipPos, lKneePos, lFootPos);
        drawLimbConnection(rHipPos, rHipPos, rKneePos, rFootPos);
        drawLimbConnection(lHipPos, lKneePos, lFootPos, lFootPos);
        drawLimbConnection(rHipPos, rKneePos, rFootPos, rFootPos);

        // Head
        ofSetColor(ofColor::fromHsb(userHue, 150, 255));
        ofDrawSphere(headPos.x, headPos.y, headPos.z, 100.0f);

        //Hands
        ofDrawSphere(lHandPos.x, lHandPos.y, lHandPos.z, 40.0f);
        ofDrawSphere(rHandPos.x, rHandPos.y, rHandPos.z, 40.0f);
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

void testApp::drawLimbConnection(ofVec3f firstPoint, ofVec3f secondPoint, ofVec3f thirdPoint, ofVec3f fourthPoint){
    ofCurve(firstPoint.x, firstPoint.y, firstPoint.z, secondPoint.x, secondPoint.y, secondPoint.z, thirdPoint.x, thirdPoint.y, thirdPoint.z, fourthPoint.x, fourthPoint.y, fourthPoint.z);
}

void testApp::drawTriangle(ofVec3f firstPoint, ofVec3f secondPoint, ofVec3f thirdPoint){
    ofTriangle(firstPoint.x, firstPoint.y, firstPoint.z, secondPoint.x, secondPoint.y, secondPoint.z, thirdPoint.x, thirdPoint.y, thirdPoint.z);
}
