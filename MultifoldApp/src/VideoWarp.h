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
    VideoWarp(int playerType){
        mPlayerType = playerType;
    }
    
    static VideoWarpRef create(int playerType = 0) {
        return std::make_shared<VideoWarp>(playerType);
        
    }
    
    void loadVideo(std::string name);
    void startPlay();
    void setPaused(bool status);
    void updateFrame(int64_t currFrame);
    void update();
    int getTotalNumFrames();
    int getCurrentFrame();
    
    void draw(int x, int y, int width, int height);
    ofTexture  getTexture();
    ofTexture * getTexturePtr();
    
    void nextFrame();
    
private:
    
    std::string mVideoName;
    
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
    
};

}
