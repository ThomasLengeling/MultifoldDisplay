#include "ofApp.h"

static int64_t prev_frame = 0;
static int64_t cur_frame = 0;
static uint8_t offset = 1;

//--------------------------------------------------------------
void ofApp::setup(){
    //start HPV engine
    HPV::InitHPVEngine();
    
    ofSetVerticalSync(true);
    ofSetFrameRate(25);
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
    mResolutionType = 0;
    
    std::vector<std::string> strVideo;
    
    ofJson videoJs;
    std::string configFile = "video.json";
    ofFile videoFile(configFile);
    
    if (videoFile.exists()) {
        ofLog(OF_LOG_NOTICE) << "Reading Config File " << configFile;
        videoFile >> videoJs;
        
        int videoType = int(videoJs["videoType"]);
        mPlayerType = videoType;
        if(videoType == 0){
            ofLog(OF_LOG_NOTICE) <<"Loading video HAP "<<int(mPlayerType);
        }
        if(videoType == 1){
            ofLog(OF_LOG_NOTICE) <<"Loading video HPV "<<int(mPlayerType);
        }
        if(videoType == 2){
            ofLog(OF_LOG_NOTICE) <<"Loading video MOV "<<int(mPlayerType);
        }
        
        int i = 0;
        for(auto & videoNames : videoJs["videos"]){
            if(!videoNames.empty()){
                std::string name = videoNames["name"];
                strVideo.push_back(name);
                ofLog(OF_LOG_NOTICE) << "Found video "<<i<<" : "<<name;
                i++;
            }
        }
        
        //load videos
        if(strVideo.size() == 4){
            for(int i = 0; i < 4; i++){
                inn::VideoWarpRef warp = inn::VideoWarp::create(mPlayerType);
                warp->loadVideo(strVideo.at(i));
                mVideoWarps.push_back(warp);
            }
            
        }
        
    }else{
        //loat temp video
        for(int i = 0; i < 4; i++){
            inn::VideoWarpRef warp = inn::VideoWarp::create(mPlayerType);
            warp->loadVideo(strVideo.at(i));
            mVideoWarps.push_back(warp);
        }
    }
    
    //play videos
    for(int i = 0; i < 4; i++){
        mVideoWarps.at(i)->startPlay();
    }
    
    if(mPlayerType == 1){
        
    }
    
    //native video
    if(mPlayerType == 2){
        
        //initialize video
        for(int i = 0; i < 4; i++){
            mVideoWarps.at(i)->updateFrame(0);
            mVideoWarps.at(i)->setPaused(true);
            mVideoWarps.at(i)->update();
        }
        
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
    
    syncVideos();
}

void ofApp::syncVideos(){
    
    //HAP and HD
    if(mPlayerType == 0){
        
    }else if(mPlayerType == 1){ // HPV
        if (cur_frame != prev_frame){
            
            //update video frame
            for(int i = 0; i < 4; i++){
                mVideoWarps.at(i)->updateFrame(cur_frame);
            }
            prev_frame = cur_frame;
        }
        
        cur_frame++;
        
        if (cur_frame >= mVideoWarps.at(0)->getTotalNumFrames())
        {
            cur_frame = 0;
        }
        
        HPV::Update();
    }else if(mPlayerType == 2){
        
        if(!mPause){
            if (cur_frame != prev_frame){
                
                //update frame
                for(int i = 0; i < 4; i++){
                    mVideoWarps.at(i)->nextFrame();
                }
                
                //update video
                for(int i = 0; i < 4; i++){
                    mVideoWarps.at(i)->update();
                }
                
                prev_frame = cur_frame;
                
            }
            
            cur_frame++;
            
            if (cur_frame >= mVideoWarps.at(0)->getTotalNumFrames()){
                
                //reset alll the videos:
                for(int i = 0; i < 4; i++){
                    mVideoWarps.at(i)->updateFrame(0);
                }

                cur_frame = 0;
            }
            
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    
    //drawVideos();
    
    if(mDrawWarp){
        drawWarps();
    }
    
    if(mDebugLayoutVideos){
        debugLayoutVideos();
    }
    
    
    drawGui();
}
//--------------------------------------------------------------
void ofApp::setupGui(){
    
    parameters.setName("Param");
    parameters.add(mBkgColor.set("bkg Color", ofColor(0,0,0)));
    parameters.add(mWarpSave.set("Warp Save", false));
    parameters.add(mPlayMovie.set("Toggle Pause", false));
    parameters.add(mResetMovie.set("Reset Movies", false));
    parameters.add(mDebugMovie.set("Debug", false));
    parameters.add(mDrawWarp.set("Draw Warp", true));
    parameters.add(mMasterFrame.set("Master Frame", 0, 0, 50));
    parameters.add(mWarpMapping->parameters);
    
    mPlayMovie.addListener(this, &ofApp::playMovies);
    mResetMovie.addListener(this, &ofApp::resetMovies);
    mDebugMovie.addListener(this, &ofApp::debugMovie);
    mMasterFrame.addListener(this, &ofApp::frameSlider);
    
    
    mGui.setup(parameters);
    mDrawGUI = true;
    mDebugLayoutVideos = false;
}


void ofApp::frameSlider(int & value){
    int framePosition = int(value);
    
    //reset alll the videos:
    for(int i = 0; i < 4; i++){
        mVideoWarps.at(i)->updateFrame(framePosition);
    }
    
    //update video
    for(int i = 0; i < 4; i++){
        mVideoWarps.at(i)->update();
    }
    
     ofLog(OF_LOG_NOTICE) <<framePosition;
}
//--------------------------------------------------------------
void ofApp::resetMovies(bool & value){
    //  bool status = static_cast<ofParameter<bool>& >(e);
    
    if(value){
        //reset to the begining of the video
        for(int i = 0; i < 4; i++){
            mVideoWarps.at(i)->updateFrame(0);
        }
    }
    
    ofLog(OF_LOG_NOTICE) << "RESET MOVIE "<<value;
}

//--------------------------------------------------------------
void ofApp::playMovies(bool & value){
    bool status = value;
    
    //stop videos:
    for(int i = 0; i < 4; i++){
        mVideoWarps.at(i)->setPaused(status);
    }
    
    mPause = status;
    ofLog(OF_LOG_NOTICE) << "Puase MOVIE "<<status;
}

//--------------------------------------------------------------
void ofApp::debugMovie(bool & value){
    mDebugLayoutVideos = !mDebugLayoutVideos;
}
//--------------------------------------------------------------
void ofApp::drawVideoInfo(int id){
    ofSetColor(255, 255, 255);
    std::string infoName ="Video "+ to_string(id);
    infoName += " :"+to_string(mVideoWarps.at(id)->getCurrentFrame())+" "+to_string(mVideoWarps.at(id)->getTotalNumFrames());
    ofDrawBitmapString(infoName, 10, 30 + id*25);
}
//--------------------------------------------------------------
void ofApp::drawGui(){
    if (mDrawGUI) {
        
        //draw info videos;
        for(int i = 0; i < 4; i++){
            drawVideoInfo(i);
        }
        
        mGui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::drawWarps(){
    
    //get texture for the video and update the warp
    for(int i = 0; i < 4; i++){
        ofTexture  tex =  mVideoWarps.at(i)->getTexture();
        if(tex.isAllocated()){
            mWarpMapping->draw(tex, i);
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawVideos(){
    //new aspect ratio
    if(mResolutionType == 0){
        float wDisplay = ofGetWindowWidth()/(float)numDisplays;
        float hDisplay = ofGetWindowHeight() /(float)numDisplays;
        
        float midScreen =  0;//ofGetWindowHeight()/2.0 - hDisplay/2.0;
        
        //draw the video
        for(int i = 0; i < 4; i++){
            mVideoWarps.at(i)->draw(wDisplay * i, midScreen, wDisplay, hDisplay);
        }
    }else{//4K
        
    }
}

//--------------------------------------------------------------
void ofApp::debugLayoutVideos(){
    float wDisplay = ofGetWindowWidth()/(float)numDisplays;
    float hDisplay = ofGetWindowHeight() /(float)numDisplays;
    
    float midScreen =  0;//ofGetWindowHeight()/2.0 - hDisplay/2.0;
    
    //draw the video
    for(int i = 0; i < 4; i++){
        mVideoWarps.at(i)->draw(wDisplay * i, midScreen, wDisplay, hDisplay);
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
    if(key == 'z'){
        
        //reset video
        for(int i = 0; i < 4; i++){
            mVideoWarps.at(i)->setPaused(false);
        }
        
        //start plays
        for(int i = 0; i < 4; i++){
            mVideoWarps.at(i)->startPlay();
        }
        
    }
    
    if(key == 'x'){
        for(int i = 0; i < 4; i++){
            mVideoWarps.at(i)->setPaused(true);
         }
    }
    if(key == 'c'){
        mWarpMapping->setSize(WIDTH_SD, HEIGHT_SD);
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
