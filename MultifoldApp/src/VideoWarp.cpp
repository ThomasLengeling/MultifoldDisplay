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
    
    if(mPlayerType == 2){
        ofLog(OF_LOG_NOTICE)<<"OF Native Player";
        mOFVideoPlayer.load(mVideoName);
        mOFVideoPlayer.setLoopState(OF_LOOP_NORMAL);
    }
    
    ofLog(OF_LOG_NOTICE)<<"Loaded Video "<<mVideoName<<std::endl;
    
}

//--------------------------------------------------------------
void VideoWarp::updateFrame(int64_t currFrame){
    if(mPlayerType == 0){
        mHAPPlayer.setFrame(currFrame);
        float pos = mHAPPlayer.getPosition();
        mHAPPlayer.setPosition(pos + (1.0/25.0));
    }else if(mPlayerType == 1){
        mHPVPlayer.seekToFrame(currFrame);
    }else if(mPlayerType == 2){
        mOFVideoPlayer.setFrame(currFrame);
    }
}

//--------------------------------------------------------------
void VideoWarp::update(){
    if(mPlayerType == 0){
       
    }else if(mPlayerType == 1){
       
    }else if(mPlayerType == 2){
        mOFVideoPlayer.update();
    }
}

//--------------------------------------------------------------
int VideoWarp::getTotalNumFrames(){
    if(mPlayerType == 0){
        return mHAPPlayer.getTotalNumFrames();
    }else if(mPlayerType == 1){
        return mHPVPlayer.getTotalNumFrames();
    }else if(mPlayerType == 2){
        return mOFVideoPlayer.getTotalNumFrames();
    }
    return -1;
}

//--------------------------------------------------------------
void VideoWarp::startPlay(){
    if(mPlayerType == 0){
        mHAPPlayer.play();
    }else if(mPlayerType == 1){
        mHPVPlayer.play();
    }else if(mPlayerType == 2){
        mOFVideoPlayer.play();
    }
}

void VideoWarp::nextFrame(){
    mOFVideoPlayer.nextFrame();
}

//--------------------------------------------------------------
void VideoWarp::setPaused(bool status){
    if(mPlayerType == 0){
        mHAPPlayer.setPaused(status);
    }else if(mPlayerType == 1){
        mHPVPlayer.setPaused(status);
    }else if(mPlayerType == 2){
        mOFVideoPlayer.setPaused(status);
    }
}

//--------------------------------------------------------------
int VideoWarp::getCurrentFrame(){
    if(mPlayerType == 0){
        return mHAPPlayer.getCurrentFrame();
    }else if(mPlayerType == 1 ){
        return mHPVPlayer.getCurrentFrame();
    }else if(mPlayerType == 2){
        return mOFVideoPlayer.getCurrentFrame();
    }
    return -1;
}

//--------------------------------------------------------------
void VideoWarp::draw(int x, int y, int width, int height){
    if(mPlayerType == 0){
        mHAPPlayer.draw(x, y, width, height);
    }else if(mPlayerType == 1){
        mHPVPlayer.draw(x, y, width, height);
    }else if(mPlayerType == 2){
        mOFVideoPlayer.draw(x, y, width, height);
    }
}

//--------------------------------------------------------------
ofTexture  VideoWarp::getTexture(){
    if(mPlayerType == 0){
        if(mHAPPlayer.getTexture()->isAllocated())
            return * mHAPPlayer.getTexture();
    }else if(mPlayerType == 1){
        if(mHPVPlayer.getTexturePtr()->isAllocated())
            return * mHPVPlayer.getTexturePtr();
    }else if(mPlayerType == 2){
        if(mOFVideoPlayer.getTexture().isAllocated()){
            return mOFVideoPlayer.getTexture();
        }
    }
    ofTexture tex;
    return  tex;
}
//--------------------------------------------------------------
ofTexture * VideoWarp::getTexturePtr(){
    if(mPlayerType == 0){
        if(mHAPPlayer.getTexture()->isAllocated())
            return mHAPPlayer.getTexture();
    }else if(mPlayerType == 1){
        if(mHPVPlayer.getTexturePtr()->isAllocated())
            return  mHPVPlayer.getTexturePtr();
    }else if(mPlayerType == 2){
        if(mOFVideoPlayer.getTexture().isAllocated()){
            return mOFVideoPlayer.getTexturePtr();
        }
    }
    return new ofTexture;
}

}
