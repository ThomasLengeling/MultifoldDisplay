#include "ofApp.h"

static int64_t prev_frame = 0;
static int64_t cur_frame = 0;
static uint8_t offset = 1;

//--------------------------------------------------------------
void ofApp::setup(){
    //start HPV engine
    HPV::InitHPVEngine();
    
    ofSetVerticalSync(true);
    ofSetFrameRate(120);
    ofBackground(0);
    
    //player type
    //1 -> HAP
    //0 ->HVP
    mPlayerType = 0;
    
    std::string displayVide01 = "Videos/mov_01_02.mov";
    mVideoWarp01 = inn::VideoWarp::create(mPlayerType);
    mVideoWarp01->loadVideo(displayVide01);
    
    std::string displayVide02 = "Videos/mov_02_02.mov";
    mVideoWarp02 = inn::VideoWarp::create(mPlayerType);
    mVideoWarp02->loadVideo(displayVide02);
    
    std::string displayVide03 = "Videos/mov_03_02.mov";
    mVideoWarp03 = inn::VideoWarp::create(mPlayerType);
    mVideoWarp03->loadVideo(displayVide03);
    
    std::string displayVide04 = "Videos/mov_04_02.mov";
    mVideoWarp04 = inn::VideoWarp::create(mPlayerType);
    mVideoWarp04->loadVideo(displayVide04);
    
    mVideoWarp01->startPlay();
    mVideoWarp02->startPlay();
    mVideoWarp03->startPlay();
    mVideoWarp04->startPlay();
    
    if(mPlayerType==1){
        mVideoWarp01->setPaused(true);
        mVideoWarp02->setPaused(true);
        mVideoWarp03->setPaused(true);
        mVideoWarp04->setPaused(true);
    }
    
    setupGui();
    
    std::cout<<"Finishing setup"<<std::endl;
    
    std::cout<<"Size"<<ofGetWindowWidth()<<" "<<ofGetWindowHeight()<<std::endl;
    

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    syncVideos();
}

void ofApp::syncVideos(){
    
    if(mPlayerType == 1){
        if (cur_frame != prev_frame)
        {
            mVideoWarp01->update(cur_frame);
            mVideoWarp02->update(cur_frame);
            mVideoWarp03->update(cur_frame);
            mVideoWarp04->update(cur_frame);
            prev_frame = cur_frame;
        }
        
        cur_frame++;
        
        if (cur_frame >= mVideoWarp01->getTotalNumFrames())
        {
            cur_frame = 0;
        }
        
        HPV::Update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    
    playVideosHD();
    
    ofDrawBitmapString( mVideoWarp01->getFrameRate(), 10, 30);
    ofDrawBitmapString( mVideoWarp02->getFrameRate(), 10, 50);
    ofDrawBitmapString( mVideoWarp03->getFrameRate(), 10, 70);
    ofDrawBitmapString( mVideoWarp04->getFrameRate(), 10, 90);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
    
    drawGui();
}
//--------------------------------------------------------------
void ofApp::setupGui(){
    
    parameters.setName("Param");
    parameters.add(mBkgColor.set("bkg Color", ofColor(0,0,0)));
    parameters.add(mWarpSave.set("Warp Save", false));
    parameters.add(mPlayMovie.set("Play", false));
    parameters.add(mResetMovie.set("Reset Movies", false));
    parameters.add(mDebug.set("Debug", false));
    //parameters.add(mWarp->parameters);
    
    mGui.setup(parameters);
    mDrawGUI = true;
}
//--------------------------------------------------------------
void ofApp::drawGui(){
    if (mDrawGUI) {
        mGui.draw();
    }
}
//--------------------------------------------------------------
void ofApp::playVideos4K(){
    mVideoWarp01->draw(0, 0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
    mVideoWarp02->draw(WIDTH_4K/4.0, 0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
    mVideoWarp03->draw(0, HEIGHT_4K/4.0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
    mVideoWarp04->draw(WIDTH_4K/4.0, HEIGHT_4K/4.0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
    
}
//--------------------------------------------------------------
void ofApp::playVideosHD(){
    mVideoWarp01->draw(0, 0, WIDTH_HD/2.0 - offset, HEIGHT_HD/2.0 - offset) ;
    mVideoWarp02->draw(WIDTH_HD/2.0 + offset, 0, WIDTH_HD/2.0 -offset, HEIGHT_HD/2.0-offset);
    mVideoWarp03->draw(0, HEIGHT_HD/2.0 + offset, WIDTH_HD/2.0 - offset, HEIGHT_HD/2.0 - offset);
    mVideoWarp04->draw(WIDTH_HD/2.0 + offset, HEIGHT_HD/2.0 + offset, WIDTH_HD/2.0 + offset, HEIGHT_HD/2.0 + offset);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'o'){
        (offset > 0) ? (offset = 0) : (offset = 1);
    }
    
    if (key == 'g') {
        mDrawGUI = !mDrawGUI;
        std::cout << "gui" << std::endl;
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
