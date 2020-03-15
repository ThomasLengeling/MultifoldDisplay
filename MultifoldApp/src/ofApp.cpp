#include "ofApp.h"

static int64_t prev_frame = 0;
static int64_t cur_frame = 0;
static uint8_t offset = 1;

//--------------------------------------------------------------
void ofApp::setup(){
    //start HPV engine
       HPV::InitHPVEngine();
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0);
    
   
    
    std::string displayVide01 = "Videos/top_right.hpv";
    
    mVideoWarpLeft = inn::VideoWarp::create();
    mVideoWarpLeft->loadVideo(displayVide01);
    
    std::string displayVide02 = "Videos/top_left.hpv";
    mVideoWarpRight = inn::VideoWarp::create();
    mVideoWarpRight->loadVideo(displayVide02);
    
    std::string displayVide03 = "Videos/bottom_right.hpv";
    mVideoWarpUp = inn::VideoWarp::create();
    mVideoWarpUp->loadVideo(displayVide03);
    
    std::string displayVide04 = "Videos/bottom_left.hpv";
    mVideoWarpDown = inn::VideoWarp::create();
    mVideoWarpDown->loadVideo(displayVide04);
    
    std::cout<<"Finishing setup"<<std::endl;
    
    std::cout<<"Size"<<ofGetWindowWidth()<<" "<<ofGetWindowHeight()<<std::endl;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    
    if (cur_frame != prev_frame)
    {
        mVideoWarpLeft->update(cur_frame);
        mVideoWarpRight->update(cur_frame);
        mVideoWarpUp->update(cur_frame);
        mVideoWarpDown->update(cur_frame);
        prev_frame = cur_frame;
    }
    
    cur_frame++;
    
    if (cur_frame >= mVideoWarpLeft->getTotalNumFrames())
    {
        cur_frame = 0;
    }
    
    HPV::Update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    
    playVideosHD();

    ofDrawBitmapString( mVideoWarpLeft->getFrameRate(), 10, 30);
    ofDrawBitmapString( mVideoWarpRight->getFrameRate(), 10, 50);
    ofDrawBitmapString( mVideoWarpUp->getFrameRate(), 10, 70);
    ofDrawBitmapString( mVideoWarpDown->getFrameRate(), 10, 90);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
    
}
//--------------------------------------------------------------
void ofApp::playVideos4K(){
    mVideoWarpRight->draw(0, 0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
    mVideoWarpLeft->draw(WIDTH_4K/4.0, 0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
    mVideoWarpDown->draw(0, HEIGHT_4K/4.0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
    mVideoWarpUp->draw(WIDTH_4K/4.0, HEIGHT_4K/4.0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
    
}
//--------------------------------------------------------------
void ofApp::playVideosHD(){
    mVideoWarpRight->draw(0, 0, WIDTH_HD/2.0 - offset, HEIGHT_HD/2.0 - offset) ;
    mVideoWarpLeft->draw(WIDTH_HD/2.0 + offset, 0, WIDTH_HD/2.0 -offset, HEIGHT_HD/2.0-offset);
    mVideoWarpDown->draw(0, HEIGHT_HD/2.0 + offset, WIDTH_HD/2.0 - offset, HEIGHT_HD/2.0 - offset);
    mVideoWarpUp->draw(WIDTH_HD/2.0 + offset, HEIGHT_HD/2.0 + offset, WIDTH_HD/2.0 + offset, HEIGHT_HD/2.0 + offset);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'g'){
        (offset > 0) ? (offset = 0) : (offset = 1);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    HPV::DestroyHPVEngine();
}
