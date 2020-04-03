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
    ofDisableArbTex();
    
    //number of display
    numDisplays = 4;
    
    //player type
    //1 -> HPV
    //0 -> HAP
    mPlayerType = 0;
    
    //0 - HD
    //1 - 4K
    mResolutionType = 1;
    
    std::string displayVide01;
    std::string displayVide02;
    std::string displayVide03;
    std::string displayVide04;
    
    std::vector<std::string> strVideo;
    
    ofJson videoJs;
    std::string configFile = "video.json";
    ofFile videoFile(configFile);
    
    if (videoFile.exists()) {
        ofLog(OF_LOG_NOTICE) << " Reading Config File " << configFile;
        videoFile >> videoJs;
        
        int i = 0;
        for(auto & videoNames : videoJs["videos"]){
            if(!videoNames.empty()){
                std::string name = videoNames["name"];
                strVideo.push_back(name);
                ofLog(OF_LOG_NOTICE) << "Found video "<<i<<" :"<<name<<std::endl;
                i++;
            }
        }
        
        if(strVideo.size() == 4){
            displayVide01 = strVideo.at(0);
            mVideoWarp01 = inn::VideoWarp::create(mPlayerType);
            mVideoWarp01->loadVideo(displayVide01);
            
            displayVide02 =  strVideo.at(1);
            mVideoWarp02 = inn::VideoWarp::create(mPlayerType);
            mVideoWarp02->loadVideo(displayVide02);
            
            displayVide03 =  strVideo.at(2);
            mVideoWarp03 = inn::VideoWarp::create(mPlayerType);
            mVideoWarp03->loadVideo(displayVide03);
            
            displayVide04 =  strVideo.at(3);
            mVideoWarp04 = inn::VideoWarp::create(mPlayerType);
            mVideoWarp04->loadVideo(displayVide04);
        }

    }else{
        displayVide01 = "Videos/P1344322_hap.mov";
        mVideoWarp01 = inn::VideoWarp::create(mPlayerType);
        mVideoWarp01->loadVideo(displayVide01);
        
        displayVide02 = "Videos/P1344336_hap.mov";
        mVideoWarp02 = inn::VideoWarp::create(mPlayerType);
        mVideoWarp02->loadVideo(displayVide02);
        
        displayVide03 = "Videos/P1344337_hap.mov";
        mVideoWarp03 = inn::VideoWarp::create(mPlayerType);
        mVideoWarp03->loadVideo(displayVide03);
        
        displayVide04 = "Videos/P1344370_hap.mov";
        mVideoWarp04 = inn::VideoWarp::create(mPlayerType);
        mVideoWarp04->loadVideo(displayVide04);
    }
    
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
    
    //WARP
    mWarpMapping = inn::Mapping::create(numDisplays);
    mWarpMapping->setupWarp(WIDTH_HD/2.0, HEIGHT_HD/2.0);
    
    //GUI
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
    
    //HAP and HD
    if(mPlayerType == 0){
        
    }else if(mPlayerType == 1){
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
    
   // drawVideos();
    
    drawWarps();
    
    if (mDrawGUI) {
        ofDrawBitmapString(mVideoWarp01->getFrameRate(), 10, 30);
        ofDrawBitmapString(mVideoWarp02->getFrameRate(), 10, 50);
        ofDrawBitmapString(mVideoWarp03->getFrameRate(), 10, 70);
        ofDrawBitmapString(mVideoWarp04->getFrameRate(), 10, 90);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
    }
    
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
    parameters.add(mWarpMapping->parameters);
    
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
void ofApp::drawWarps(){
    ofTexture * tex01 = mVideoWarp01->getTexture();
    if(tex01->isAllocated()){
        mWarpMapping->draw(*tex01, 0);
    }
    
    ofTexture * tex02 = mVideoWarp02->getTexture();
    if(tex02->isAllocated()){
        mWarpMapping->draw(*tex02, 1);
    }
    
    ofTexture * tex03 = mVideoWarp03->getTexture();
    if(tex03->isAllocated()){
        mWarpMapping->draw(*tex03, 2);
    }
    
    ofTexture * tex04 = mVideoWarp04->getTexture();
    if(tex04->isAllocated()){
        mWarpMapping->draw(*tex04, 3);
    }
}

//--------------------------------------------------------------
void ofApp::drawVideos(){
    //new aspect ratio
    if(mResolutionType == 0){
        float wDisplay = ofGetWindowWidth()/(float)numDisplays;
        float hDisplay = ofGetWindowHeight() /(float)numDisplays;
        
        float midScreen =  0;//ofGetWindowHeight()/2.0 - hDisplay/2.0;
        
        //display
        mVideoWarp01->draw(0, midScreen, wDisplay, hDisplay);
        mVideoWarp02->draw(wDisplay, midScreen, wDisplay, hDisplay);
        mVideoWarp03->draw(wDisplay * 2, midScreen, wDisplay, hDisplay);
        mVideoWarp04->draw(wDisplay * 3, midScreen, wDisplay, hDisplay);
    }else{//4K
        
    }
}

void ofApp::debugLayoutVideos(){
    
}

//--------------------------------------------------------------
void ofApp::debugSyncVideos(){
    
    if(mResolutionType){
        mVideoWarp01->draw(0, 0, WIDTH_HD/2.0 - offset, HEIGHT_HD/2.0 - offset) ;
        mVideoWarp02->draw(WIDTH_HD/2.0 + offset, 0, WIDTH_HD/2.0 -offset, HEIGHT_HD/2.0-offset);
        mVideoWarp03->draw(0, HEIGHT_HD/2.0 + offset, WIDTH_HD/2.0 - offset, HEIGHT_HD/2.0 - offset);
        mVideoWarp04->draw(WIDTH_HD/2.0 + offset, HEIGHT_HD/2.0 + offset, WIDTH_HD/2.0 + offset, HEIGHT_HD/2.0 + offset);
    }else{
        mVideoWarp01->draw(0, 0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
        mVideoWarp02->draw(WIDTH_4K/4.0, 0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
        mVideoWarp03->draw(0, HEIGHT_4K/4.0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
        mVideoWarp04->draw(WIDTH_4K/4.0, HEIGHT_4K/4.0, WIDTH_4K/4.0, HEIGHT_4K/4.0);
    }
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

    if (key == 's') {
        mWarpMapping->saveWarp();
    }
    
    if(key == '1'){
        std::string displayVide01 = "Videos/mov_01_02.mov";
        mVideoWarp01 = inn::VideoWarp::create(mPlayerType);
        mVideoWarp01->loadVideo(displayVide01);
        
        std::string displayVide02 = "Videos/mov_01_02.mov";
        mVideoWarp02 = inn::VideoWarp::create(mPlayerType);
        mVideoWarp02->loadVideo(displayVide02);
        
        std::string displayVide03 = "Videos/mov_01_02.mov";
        mVideoWarp03 = inn::VideoWarp::create(mPlayerType);
        mVideoWarp03->loadVideo(displayVide03);
        
        std::string displayVide04 = "Videos/mov_01_02.mov";
        mVideoWarp04 = inn::VideoWarp::create(mPlayerType);
        mVideoWarp04->loadVideo(displayVide04);
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
    mWarpMapping->saveWarp();
}
