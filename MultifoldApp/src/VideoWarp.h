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
 */

#include "ofMain.h"
#include "Mapping.h"
#include "ofxHPVPlayer.h"

namespace inn {

class VideoWarp;

typedef std::shared_ptr<VideoWarp> VideoWarpRef;

class VideoWarp : public Mapping{
public:
    VideoWarp(){
        
    }
    
    static VideoWarpRef create() {
        return std::make_shared<VideoWarp>();
        
    }
    
    void loadVideo(std::string name);
    void startPlay();
    void update(int64_t currFrame);
    
private:
    
    std::string mVideoName;
    ofxHPVPlayer mVideoPlayer;
};

}
