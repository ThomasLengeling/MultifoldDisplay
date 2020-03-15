#include "ofApp.h"

static int64_t prev_frame = 0;
static int64_t cur_frame  = 0;
static uint8_t offset     = 1;

//--------------------------------------------------------------
void ofApp::setup(){
    
    HPV::InitHPVEngine();
    
    std::string videoName01 = "Videos/top_left.hpv";
    mVideoPlayer01.init(HPV::NewPlayer());
    mVideoPlayer01.load(videoName01);
    mVideoPlayer01.setLoopState(OF_LOOP_NORMAL);
    mVideoPlayer01.play();
    mVideoPlayer01.setDoubleBuffered(true);
    
    std::string videoName02 = "Videos/top_right.hpv";
    mVideoPlayer02.init(HPV::NewPlayer());
    mVideoPlayer02.load(videoName02);
    mVideoPlayer02.setLoopState(OF_LOOP_NORMAL);
    mVideoPlayer02.play();
    mVideoPlayer02.setDoubleBuffered(true);
    
    std::string videoName03 = "Videos/bottom_left.hpv";
    mVideoPlayer03.init(HPV::NewPlayer());
    mVideoPlayer03.load(videoName03);
    mVideoPlayer03.setLoopState(OF_LOOP_NORMAL);
    mVideoPlayer03.play();
    mVideoPlayer03.setDoubleBuffered(true);
    
    std::string videoName04 = "Videos/bottom_right.hpv";
    mVideoPlayer04.init(HPV::NewPlayer());
    mVideoPlayer04.load(videoName04);
    mVideoPlayer04.setLoopState(OF_LOOP_NORMAL);
    mVideoPlayer04.play();
    mVideoPlayer04.setDoubleBuffered(true);
    
    ofSetVerticalSync(false);
    ofSetFrameRate(120);
    
    mWidthVideo  = ofGetWidth() / 2;
    mHeightVideo = ofGetHeight() / 2;
}


//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    if (cur_frame != prev_frame)
    {
        mVideoPlayer01.seekToFrame(cur_frame);
        mVideoPlayer02.seekToFrame(cur_frame);
        mVideoPlayer03.seekToFrame(cur_frame);
        mVideoPlayer04.seekToFrame(cur_frame);
        prev_frame = cur_frame;
    }
    
    cur_frame++;
    
    if (cur_frame >= mVideoPlayer01.getTotalNumFrames())
    {
        cur_frame = 0;
    }
    
    HPV::Update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    
    playVideosHD();

    ofDrawBitmapString( mVideoPlayer01.getFrameRate(), 10, 30);
    ofDrawBitmapString( mVideoPlayer02.getFrameRate(), 10, 50);
    ofDrawBitmapString( mVideoPlayer03.getFrameRate(), 10, 70);
    ofDrawBitmapString( mVideoPlayer04.getFrameRate(), 10, 90);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
}

//--------------------------------------------------------------
void ofApp::playVideosHD(){
    mVideoPlayer01.draw(0, 0, mWidthVideo - offset, mHeightVideo - offset) ;
    mVideoPlayer02.draw(mWidthVideo + offset, 0, mWidthVideo -offset, mHeightVideo - offset);
    mVideoPlayer03.draw(0, mHeightVideo + offset, mWidthVideo - offset, mHeightVideo - offset);
    mVideoPlayer04.draw(mWidthVideo + offset, mHeightVideo+ offset, mWidthVideo + offset, mHeightVideo + offset);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'a')
    {
        cur_frame++;
    }
    else if (key == 'p')
    {
        mVideoPlayer01.setPaused(true);
        mVideoPlayer02.setPaused(true);
        mVideoPlayer03.setPaused(true);
        mVideoPlayer04.setPaused(true);
    }
    else if (key == 'f')
    {
        ofToggleFullscreen();
    }
    else if (key == 'g')
    {
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
void ofApp::exit()
{
    HPV::DestroyHPVEngine();
}
