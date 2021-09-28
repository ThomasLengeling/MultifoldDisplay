#pragma once

#include "ofMain.h"
#include "ofxHPVPlayer.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();
    
    void onHPVEvent(const HPVEvent& event);
    
	void keyPressed(int key);
    
	ofxHPVPlayer hpvPlayer01;
	ofxHPVPlayer hpvPlayer02;
	ofxHPVPlayer hpvPlayer03;
	ofxHPVPlayer hpvPlayer04;
	ofxHPVPlayer hpvPlayer05;
	ofxHPVPlayer hpvPlayer06;
	ofxHPVPlayer hpvPlayer07;
	ofxHPVPlayer hpvPlayer08;
	ofxHPVPlayer hpvPlayer09;

    ofSoundPlayer soundPlayer;
};
