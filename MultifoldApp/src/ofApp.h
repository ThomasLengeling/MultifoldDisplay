#pragma once

#include "ofMain.h"
#include "VideoWarp.h"
#include "Mapping.h"
#include "ofxHPVPlayer.h"

#include "ofxDatGui.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofxGui.h"

/*
 
 4K ->3840 x 2160 or 4096 x 2160
 
 Video Card GeForce 2080
 Max Resolution
 7680x4320 -> 2x2 4K
 
 
 */

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
    
    
    //Video Warp
    inn::VideoWarpRef mVideoWarp01;
    inn::VideoWarpRef mVideoWarp02;
    inn::VideoWarpRef mVideoWarp03;
    inn::VideoWarpRef mVideoWarp04;
    
    void syncVideos();
    
    //warp
    inn::MappingRef mWarpMapping;
    void drawWarps();
    
    //play videos in 4K resolution
    void debugSyncVideos();
    
    //debug layout videos
    void debugLayoutVideos();
    
    //play videos in current #displays
    void drawVideos();
    
    //resolution type of Videos
    // type = 0  HD
    // type = 1  4K
    int mResolutionType;
    
    int mPlayerType;

    //number of displays output
    int numDisplays;
    
    //single mode display mode
    int simpleMode;
    
    //multi display mode
    int multiMode;
    
    //debug and release mode
    ofParameterGroup      parameters;
    ofParameter<ofColor>  mBkgColor;
    ofParameter<bool>     mWarpSave;
    ofParameter<bool>     mPlayMovie;
    ofParameter<bool>     mResetMovie;
    ofParameter<bool>     mDebug;
    
    ofxPanel               mGui;
    bool                   mDrawGUI;
    void                   setupGui();
    void                   drawGui();
};
