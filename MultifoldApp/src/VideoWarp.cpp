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
    
    if(mPlayerType == 1){
        ofLog(OF_LOG_NOTICE)<<"Player HPV";
        mHPVPlayer.init(HPV::NewPlayer());
        mHPVPlayer.load(mVideoName);
        mHPVPlayer.setLoopState(OF_LOOP_NORMAL);
        mHPVPlayer.setDoubleBuffered(true);
    }
    
    if(mPlayerType == 0){
        ofLog(OF_LOG_NOTICE)<<"Player HAP";
        mHAPPlayer.load(mVideoName);
        mHAPPlayer.setLoopState(OF_LOOP_NORMAL);
    }

    ofLog(OF_LOG_NOTICE)<<"Loaded Video "<<mVideoName<<std::endl;

}

void VideoWarp::update(int64_t currFrame){
    if(mPlayerType == 0){
        mHAPPlayer.setFrame(currFrame);
        
        float pos = mHAPPlayer.getPosition();
        mHAPPlayer.setPosition(pos + (1.0/25.0));
    }else{
        mHPVPlayer.seekToFrame(currFrame);
    }
}

int VideoWarp::getTotalNumFrames(){
    if(mPlayerType == 0){
        return mHAPPlayer.getTotalNumFrames();
    }else{
        return mHPVPlayer.getTotalNumFrames();
    }
    return -1;
}

void VideoWarp::startPlay(){
    if(mPlayerType == 0){
        mHAPPlayer.play();
    }else{
        mHPVPlayer.play();
    }
}

void VideoWarp::setPaused(bool status){
    if(mPlayerType == 0){
        mHAPPlayer.setPaused(status);
    }else{
        mHPVPlayer.setPaused(status);
    }
}

int VideoWarp::getFrameRate(){
    if(mPlayerType == 0){
        //return mHAPPlayer.getCurrentFrame();
    }else{
        return mHPVPlayer.getCurrentFrame();
    }
    return -1;
}

void VideoWarp::draw(int x, int y, int width, int height){
    if(mPlayerType == 0){
        mHAPPlayer.draw(x, y, width, height);
    }else{
        mHPVPlayer.draw(x, y, width, height);
    }
}

ofTexture * VideoWarp::getTexture(){
    if(mPlayerType == 0){
        return mHAPPlayer.getTexturePtr();
    }else{
        return  mHPVPlayer.getTexturePtr();
    }
    return new ofTexture;
}

}
