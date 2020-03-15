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
    mVideoPlayer.setDoubleBuffered(true);

    ofLog(OF_LOG_NOTICE)<<"Loaded Video"<<mVideoName<<std::endl;
}

void VideoWarp::update(int64_t currFrame){
    mVideoPlayer.seekToFrame(currFrame);
}

int VideoWarp::getTotalNumFrames(){
    return mVideoPlayer.getTotalNumFrames();
}

void VideoWarp::startPlay(){
    mVideoPlayer.play();
}

void VideoWarp::setPaused(bool status){
    mVideoPlayer.setPaused(status);
}

int VideoWarp::getFrameRate(){
    return mVideoPlayer.getCurrentFrame();
}

void VideoWarp::draw(int x, int y, int width, int height){
    mVideoPlayer.draw(x, y, width, height);
}

}
