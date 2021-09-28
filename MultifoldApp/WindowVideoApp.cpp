#include "WindowVideoApp.h"



void WindowVideoApp::setup() {

	//ofBackground(mBkgColor);
	ofBackground(0);
	ofSetVerticalSync(false);

	//load Videos
	inc = 0;

	ofSetCircleResolution(100);
}

void WindowVideoApp::update() {

	//update sync

}

void WindowVideoApp::draw() {


	ofSetColor(mBkgColor);
	ofDrawCircle(glm::vec2(WIDTH_WINDOW / 2.0, HEIGHT_WINDOW / 2.0), inc);


	ofSetColor(0);
	ofDrawBitmapString("fps: " + to_string(ofGetFrameRate()), WIDTH_WINDOW / 2.0, HEIGHT_WINDOW / 2.0);

	
	inc += 2;
	if (inc>=HEIGHT_WINDOW) {
		inc = 0;
	}
}


void WindowVideoApp::setBackground(ofColor bkg) {
	mBkgColor = bkg;
}
