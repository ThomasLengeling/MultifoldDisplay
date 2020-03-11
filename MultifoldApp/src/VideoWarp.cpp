//
//  VideoWarp.cpp
//  MultifoldApp
//
//  Created by Thomas on 3/3/20.
//

#include "VideoWarp.h"


namespace inn {

void VideoWarp::loadVideo(std::string name){
    mVideoName = name;
    
    mVideoPlayer.init(HPV::NewPlayer());
    mVideoPlayer.load(mVideoName);
    mVideoPlayer.setLoopState(OF_LOOP_NORMAL);
}

void VideoWarp::update(int64_t currFrame){
    mVideoPlayer.seekToFrame(currFrame);
}

void VideoWarp::startPlay(){
    mVideoPlayer.play();
}

}
