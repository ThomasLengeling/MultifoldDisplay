#pragma once

#include "ofMain.h"
#include "ofxHPVPlayer.h"
#include "ofApp.h"
#include "VideoPlayers.h"

#define WIDTH_4K    3840
#define HEIGHT_4K   2160

#define WIDTH_WINDOW 2160
#define HEIGHT_WINDOW 3840


class WindowVideoApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();

	void setBackground(ofColor bkg);

	void loadDebugImg();

	void setId(int id) { mId = id; }

	void setVideoName(std::string name);

	shared_ptr<CommonState> mCommon;

private:
	ofColor mBkgColor;
	int mDisplayId;
	int mNumDisplays;
	float inc;

	std::string mVideoName;

	int mId;

	ofImage         mDebugImg;
	

	inn::VideoPlayersRef  mVideoPlayer;



};


