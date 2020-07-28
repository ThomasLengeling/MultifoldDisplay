#pragma once

#include "ofMain.h"
#include "VideoWarp.h"
#include "Mapping.h"
#include "ofxHPVPlayer.h"

#include "config.h"

#include "ofxDatGui.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#include "ofxAudioFile.h"
#include "ofxSoundPlayerObject.h"

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

#define WIDTH_SD    640
#define HEIGHT_SD   360

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
    std::vector<inn::VideoWarpRef> mVideoWarps;

    void loadAV(std::string jsonFile);

    void initVideos();
    
    void syncVideos();
    
    //warp
    inn::MappingRef mWarpMapping;
    void drawWarps();
    
    //debug layout videos
    void debugLayoutVideos();
    
    int mPlayerType;

    //number of displays output
    int numDisplays;
    
    //single mode display mode
    int simpleMode;
    
    //multi display mode
    int multiMode;
    
    //multi layout display
    bool mDebugLayoutVideos;
    
    //min frame of all the videos;
    int mMinFrame;
    
    //debug and release mode
    ofParameterGroup      parameters;
    ofParameter<ofColor>  mBkgColor;
    ofParameter<bool>     mWarpSave;
    ofParameter<bool>     mPlayMovie;
    ofParameter<bool>     mResetMovie;
    ofParameter<bool>     mDebugMovie;
    ofParameter<bool>     mDrawWarp;
    ofParameter<int>      mMasterFrame;
    
    //sync video debug
    ofParameter<bool>      mSyncVideosDebug;
    
    
    ofxPanel               mGui;
    bool                   mDrawGUI;
    void                   setupGui();
    void                   drawGui();
    void                   drawVideoInfo(int id);
    void                   drawVideoTime(int id, int currentFrame, int totalFrame);
    
    void                   drawSyncVideos();
    
    void resetMovies(bool & value);
    void playMovies(bool & value);
    void debugMovie(bool & value);
    void frameSlider(int & value);
    void syncVideosDebug(bool & value);
    
    
    bool                    mPause;
    
    std::string             ofPath;


    void setupAudio(std::string audio);

    ofEventListener playerEndListener;
    void playerEnded(size_t& id);

    ofSoundStream stream;
    ofxSoundOutput output;
    // these are all subclasses of ofSoundObject
    ofxSoundPlayerObject player;

    //osc
    int            mPort;
    ofxOscReceiver receiver;
    void           updateOSC();

    bool            playNewVideos;
};
