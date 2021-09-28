#pragma once
#include "ofMain.h"

#define WIDTH_4K    3840
#define HEIGHT_4K   2160

#define WIDTH_WINDOW 6480
#define HEIGHT_WINDOW 3840


class WindowVideoApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();

	void setBackground(ofColor bkg);


private:
	ofColor mBkgColor;
	int mDisplayId;
	int mNumDisplays;
	float inc;


};


