#pragma once

#include "ofMain.h"
#include "VideoWarp.h"
#include "Mapping.h"
#include "ofxHPVPlayer.h"

#define WIDTH_4K    3840
#define HEIGHT_4K   2160

#define WIDTH_HD    1920
#define HEIGHT_HD   1080

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
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
    
    
    void playVideos4K();
    void playVideosHD();
    
    
    inn::VideoWarpRef mVideoWarpLeft;
    inn::VideoWarpRef mVideoWarpRight;
    inn::VideoWarpRef mVideoWarpUp;
    inn::VideoWarpRef mVideoWarpDown;
};
