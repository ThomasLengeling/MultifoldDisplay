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
 tyep = 1 HAP Video fast enconding for testing
 */

#include "ofMain.h"
#include "ofxHPVPlayer.h"

namespace inn {

class VideoWarp;

typedef std::shared_ptr<VideoWarp> VideoWarpRef;

class VideoWarp{
public:
    VideoWarp(){
        mPlayerType = 0;
    }
    
    static VideoWarpRef create() {
        return std::make_shared<VideoWarp>();
        
    }
    
    void loadVideo(std::string name);
    void startPlay();
    void setPaused(bool status);
    void update(int64_t currFrame);
    int getTotalNumFrames();
    int getFrameRate();
    
    void draw(int x, int y, int width, int height);
    ofTexture * getTexture();
    
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
    
};

}
