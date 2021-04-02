//
//  VideoWarp.hpp
//  MultifoldApp
//
//  Created by Thomas on 3/3/20.
//

#pragma once

/*
 Thomas Sanchez Lengeling Mapping
 MIT 2020
 
 
 Config file config.json
 
 video:{
 type:
 }
 
 type = 0 HPV Video fast encoding for installation
 type = 1 HAP Video fast enconding for testing
 tyep = 2 MP4 Video slow testing enconding
 */

#include "ofMain.h"
#include "ofxHPVPlayer.h"
#include "ofxHapPlayer.h"



namespace inn {

class VideoWarp;

typedef std::shared_ptr<VideoWarp> VideoWarpRef;

class VideoWarp{
public:
    VideoWarp(int playerType, int id);
    
    static VideoWarpRef create(int playerType = 0, int id = 0) {
        return std::make_shared<VideoWarp>(playerType, id);
        
    }

    void close();
    
    void loadVideo(std::string & name);
    void startPlay();
    void setPaused(bool status);
    void updateFrame(int64_t currFrame);
    void goToFirstFrame();
    
    void update();
    int getTotalNumFrames();
    int getCurrentFrame();
    
    void draw(int x, int y, int width, int height);
    ofTexture  getTexture();
    ofTexture * getTexturePtr();
    
    void loadPlayer(bool & value);
    
    void nextFrame();

    void closeVideo();
    bool  isLoaded();

    void setPosition(float value);
    
    bool isDone();

    ofParameterGroup & getParamGroup(){return mGroupParam;}
    
   // bool isLoaded();
    
private:
    
    //video name
    std::string mVideoName;
    
    //video alias name
    std::string  mAlias;
    
    //type of player
    int mPlayerType;
    
    //fast encoder player
    //mPlayerType = 1
    ofxHPVPlayer mHPVPlayer;
    
    //multiplatfor .mov fast enconder
    //mPlayerType = 0
    //default
    ofxHapPlayer mHAPPlayer;
    
    //mPlayerType = 2
    ofVideoPlayer mOFVideoPlayer;
    
    int mVideoId;
    
    //video files
    ofParameterGroup        mGroupParam;
    ofParameter<glm::vec2>  mSizeParam;
    ofParameter<string>     mVideoParam;
    ofParameter<bool>        mLoadParam;
    
};

}
