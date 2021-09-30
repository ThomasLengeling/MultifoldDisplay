#include "WindowVideoApp.h"



void WindowVideoApp::setup() {

	//ofBackground(mBkgColor);
	//ofSetFullscreen(true);
	ofSetFrameRate(30);
	ofDisableSmoothing();
	ofSetBackgroundAuto(false);
	ofSetVerticalSync(true);

	//load Videos
	inc = 0;

	//ofSetCircleResolution(100);

		//start HPV engine
	ofLog(OF_LOG_NOTICE) << "Video ...."  << mId;

	mHPVPlayer.init(HPV::NewPlayer());
	
	mHPVPlayer.load("videos/test_w.hpv");
	mHPVPlayer.play();
	mHPVPlayer.setPaused(true);
	mHPVPlayer.setLoopState(OF_LOOP_NORMAL);

	ofLog(OF_LOG_NOTICE) << "Done Loading "<< mId;

}

void WindowVideoApp::update() {

	if (common->startVideo == true) {
		mHPVPlayer.play();
	}

	//update sync
	HPV::Update();
}

//--------------------------------------------------------------
void WindowVideoApp::loadDebugImg() {
	if (mDebugImg.load("imgs/display_4k.png")) {
		ofLog(OF_LOG_NOTICE) << "Loaded Debug Img";
	}
	else {
		ofLog(OF_LOG_NOTICE) << "Error Debug Img";
	}
}

//--------------------------------------------------------------
void WindowVideoApp::draw() {


	ofSetColor(mBkgColor);
	ofDrawCircle(glm::vec2(WIDTH_WINDOW / 2.0, HEIGHT_WINDOW / 2.0), inc);


	ofSetColor(0);
	ofDrawBitmapString("fps: " + to_string(ofGetFrameRate()), WIDTH_WINDOW / 2.0, HEIGHT_WINDOW / 2.0);

	
	inc += 2;
	if (inc>=HEIGHT_WINDOW/1.5) {
		inc = 0;
	}

	ofSetColor(255);
	mHPVPlayer.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

}

//--------------------------------------------------------------
void WindowVideoApp::setBackground(ofColor bkg) {
	mBkgColor = bkg;
}

//--------------------------------------------------------------
void WindowVideoApp::exit() {
	HPV::DestroyHPVEngine();
}