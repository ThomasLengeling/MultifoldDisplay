#pragma once

#include <cstddef>
#include <stdint.h>

#include "ofMain.h"
#include "ofxHPVPlayer.h"

#include "config.h"

#include "ofxDatGui.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxNetwork.h"

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

class CommonState;

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

    void loadAudio(std::string jsonFile);
   
    
    //min frame of all the videos;
    int mMinFrame;

    //debug and release mode
    ofParameterGroup      parameters;     
    ofParameter<bool>     mPlayVideos;
    ofParameter<bool>     mResetVideos;
    ofParameter<bool>     mStopVideos;
    ofParameter<float>    mMasterAudio;
    
    ofxPanel mGui;
    bool mDrawGUI;

    void setupGui();
    void drawGui();
    
    void resetVideos(bool & value);
    void playVideos(bool & value);
    void audioSlider(float & value);

    
    bool                    mPause;
    
    std::string             ofPath;

    void setupAudio(std::string jsonFile);

    ofSoundStream stream;
    ofxSoundOutput output;
    // these are all subclasses of ofSoundObject
    ofxSoundPlayerObject player;

    ofEventListener playerEndListener;
    void playerEnded(size_t& id);



    //send recevied UDP
    ofxUDPManager udpReceiver; //receiver

    ofxUDPManager udpSendCenter; //send
    ofxUDPManager udpSendLeft;   //send

    //send = true, receiver = false;
    bool mMasterUDP; //sender or receiver
    bool mSlaveUDP;

    //master send to center and left
    int mUDPPortCenter;
    int mUDPPortLeft;

    int mUDPPortReceiver; //udp to send to center or left

    void setupUDP();
    void updateUDP();

    //udp commands
    void sendAudioPosUDP(float audioPos);

    //osc
    int             mPort;
    ofxOscReceiver  receiver;
    void            setupOSC();
    void            updateOSC();

    bool            playNewVideos;

    bool            mInitialize;
    float           mInitTimer;

    //main states
    bool            mStartVideoLoop;


    void setupCommonState();
    shared_ptr<CommonState> mCommon;


    //utilities
    std::vector<std::string> string_split(const std::string& str);
};

//---------------------------
class CommonState{
public:
    float mAudioPos;
    bool startVideo;
    std::string mAlias;
    int mId;
};