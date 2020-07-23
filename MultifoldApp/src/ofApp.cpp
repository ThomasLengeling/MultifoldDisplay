#include "ofApp.h"

static int64_t prev_frame = 0;
static int64_t cur_frame = 0;
static uint8_t offset = 1;

//--------------------------------------------------------------
void ofApp::setup(){

    ofLog(OF_LOG_NOTICE) << "Starting App" << std::endl;
    
    ofSetLogLevel("Logging items", OF_LOG_VERBOSE);
    
    //start HPV engine
    HPV::InitHPVEngine();
    
    ofSetVerticalSync(false);
    ofSetFrameRate(25);
    ofBackground(0);
    ofDisableArbTex();
    
    //number of display
    numDisplays = SystemVars::getInstance().numDisplays;
    
    ofLog(OF_LOG_NOTICE) << " Num Displays: "<<numDisplays<<std::endl;
    
    //player type
    //1 -> HPV
    //0 -> HAP
    mPlayerType = 0;
  

    //load av files
    std::string avfile = "avset_00.json";
    loadAV(avfile);
    initVideos();
    
    //WARP
    mWarpMapping = inn::Mapping::create(numDisplays);
    mWarpMapping->setupWarp(WIDTH_HD, HEIGHT_HD);
    
    //GUI
    setupGui();

    mPort = 32000;
    ofLog() << "listening for osc messages on port " << mPort;
    receiver.setup(mPort);
    

    ofLog(OF_LOG_NOTICE)<<"Finishing setup";
    ofLog(OF_LOG_NOTICE)<<"Size"<<ofGetWindowWidth()<<" "<<ofGetWindowHeight();
    
}

//--------------------------------------------------------------
void ofApp::loadAV(std::string jsonFile) {

    //clear data
    mVideoWarps.clear();

    //audio path;
    std::string audioPath = "";

    std::vector<std::string> strVideo;

    ofJson avJs;
    ofFile avFile(jsonFile);

    ofPath = ofFilePath::getAbsolutePath(ofToDataPath(""));

    ofLog(OF_LOG_NOTICE) << " OF data path is: " << ofPath << endl;

    if (avFile.exists()) {
        ofLog(OF_LOG_NOTICE) << "Reading Config File " << jsonFile;
        avFile >> avJs;

        int videoType = int(avJs["videoType"]);
        mPlayerType = videoType;
        if (videoType == 0) {
            ofLog(OF_LOG_NOTICE) << "Loading video HAP " << int(mPlayerType);
        }
        if (videoType == 1) {
            ofLog(OF_LOG_NOTICE) << "Loading video HPV " << int(mPlayerType);
        }
        if (videoType == 2) {
            ofLog(OF_LOG_NOTICE) << "Loading video MOV " << int(mPlayerType);
        }

        //videos
        int i = 0;
        for (auto& videoNames : avJs["videos"]) {
            if (!videoNames.empty()) {
                std::string name = videoNames["name"];
                strVideo.push_back(name);
                ofLog(OF_LOG_NOTICE) << "Found video " << i << " : " << name;
                i++;
            }
        }

        ofLog(OF_LOG_NOTICE) << "Found Videos: " << strVideo.size() << std::endl;

        //load videos
        int id = 0;
        for (auto& strVideoNames : strVideo) {
            inn::VideoWarpRef warp = inn::VideoWarp::create(mPlayerType, id);

            warp->loadVideo(strVideoNames);
            //warp->addListener();
            mVideoWarps.push_back(warp);
            id++;
        }
        //audio
        for (auto& audioNames : avJs["audios"]) {
            if (!audioNames.empty()) {
                std::string file = audioNames["name"];
                audioPath = file;
                ofLog(OF_LOG_NOTICE) << "Found audio file " << " : " << audioPath;
            }
        }
    }
    else {
        //loat temp video
        for (auto& strVideoNames : strVideo) {
            inn::VideoWarpRef warp = inn::VideoWarp::create(mPlayerType);
            warp->loadVideo(strVideoNames);


            mVideoWarps.push_back(warp);
        }
    }

    //audio
    setupAudio(audioPath);
}

//--------------------------------------------------------------
void ofApp::initVideos() {
    //play videos
    for (auto& video : mVideoWarps) {
        video->startPlay();
    }

    if (mPlayerType == 1) {

    }

    if (mPlayerType == 0) {
        //initialize video
        for (auto& video : mVideoWarps) {
            video->updateFrame(0);
            video->setPaused(true);
            video->update();
        }
    }


    //native video
    if (mPlayerType == 2) {

        //initialize video
        for (auto& video : mVideoWarps) {
            video->updateFrame(0);
            video->setPaused(true);
            video->update();
        }
    }

    //get the min fram
    mMinFrame = 999999;
    int indexVideo = -1;
    int id = 0;
    for (auto& video : mVideoWarps) {
        if (mMinFrame > video->getTotalNumFrames()) {
            mMinFrame = video->getTotalNumFrames();
            indexVideo = id;
            id++;
        }
    }

    ofLog(OF_LOG_NOTICE) << "Min Frame: " << mMinFrame << " " << indexVideo;

}


//--------------------------------------------------------------
void ofApp::setupAudio(std::string filepath) {

    audiofile.setVerbose(true);
  
    if (ofFile::doesFileExist(filepath)) {
        audiofile.load(filepath);
        if (!audiofile.loaded()) {
            ofLogError() << "error loading file, double check the file path";
        }
    }
    else {
        ofLogError() << "sound input file does not exists";
    }

    // audio setup for testing audio file stream 
    ofSoundStreamSettings settings;
    sampleRate = 44100.0;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = 512;

    ofSoundStreamSetup(settings);

    playhead = std::numeric_limits<int>::max(); // because it is converted to int for check
    playheadControl = -1.0;
    step = audiofile.samplerate() / sampleRate;
    playhead = 0;
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& buffer) {

    // really spartan and not efficient sample playing, just for testing

    if (playheadControl >= 0.0) {
        playhead = playheadControl;
        playheadControl = -1.0;
    }

    for (size_t i = 0; i < buffer.getNumFrames(); i++) {

        int n = playhead;

        if (n < audiofile.length() - 1) {

            for (size_t k = 0; k < buffer.getNumChannels(); ++k) {
                if (k < audiofile.channels()) {
                    float fract = playhead - (double)n;
                    float s0 = audiofile.sample(n, k);
                    float s1 = audiofile.sample(n + 1, k);
                    float isample = s0 * (1.0 - fract) + s1 * fract; // linear interpolation
                    buffer[i * buffer.getNumChannels() + k] = isample;
                }
                else {
                    buffer[i * buffer.getNumChannels() + k] = 0.0f;
                }
            }

            playhead += step;

        }
        else {
            buffer[i * buffer.getNumChannels()] = 0.0f;
            buffer[i * buffer.getNumChannels() + 1] = 0.0f;
            playhead = std::numeric_limits<int>::max();
        }

    }
}

//--------------------------------------------------------------
void ofApp::update() {

    syncVideos();

    //set the master frame with the current change frame
    //mMasterFrame.set(cur_frame);

    updateOSC();
}

//--------------------------------------------------------------
void ofApp::updateOSC() {
    while (receiver.hasWaitingMessages()) {

        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        // check for mouse moved message
        if (m.getAddress() == "/av") {
            int id = m.getArgAsInt32(0);
            if (id == 0) {
                std::string avfile = "avset_00.json";
                loadAV(avfile);
                initVideos();
                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
            }

            if (id == 1) {
                std::string avfile = "avset_01.json";
                loadAV(avfile);
                initVideos();
                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
            }

            if (id == 2) {
                std::string avfile = "avset_02.json";
                loadAV(avfile);
                initVideos();
                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
            }

            if (id == 3) {
                std::string avfile = "avset_03.json";
                loadAV(avfile);
                initVideos();
                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
            }
        }

        if (m.getAddress() == "/stop") {
            bool play = true;
            playMovies(play);
        }

        if (m.getAddress() == "/play") {
            bool play = false;
            playMovies(play);
        }

        if(m.getAddress() == "/reset"){
            bool reset = true;
            resetMovies(reset);
        }
    }
}

//--------------------------------------------------------------
void ofApp::syncVideos(){
    
    //HAP and HD
    if(mPlayerType == 0){
		if (!mPause) {
			if (cur_frame != prev_frame) {

				//update video
				for (auto & video : mVideoWarps) {
					video->update();
				}
				prev_frame = cur_frame;
			}

			//increase frame and reset when the current frame hits the min of all number of frame
			cur_frame++;

		}
		else {

			//update video
			for (auto & video : mVideoWarps) {
				video->update();
			}


		}

    }else if(mPlayerType == 1){ // HPV
        if (cur_frame != prev_frame){
            
            //update video frame
             for(auto & video : mVideoWarps){
               video->updateFrame(cur_frame);
            }
            prev_frame = cur_frame;
        }
        
        
        //increase frame and reset when the current frame hits the min of all number of frame
        cur_frame++;
        if (cur_frame >= mMinFrame){
            cur_frame = 0;
        }
        
        HPV::Update();
    }else if(mPlayerType == 2){
        
        if(!mPause){
            if (cur_frame != prev_frame){
                
                //update frame
               for(auto & video : mVideoWarps){
                   video->nextFrame();
                }
                    
                //update video
                for(auto & video : mVideoWarps){
                    video->update();
                }
                prev_frame = cur_frame;
            }
            
            //increase frame and reset when the current frame hits the min of all number of frame
            cur_frame++;
            if (cur_frame >= mMinFrame){
                ofLog(OF_LOG_NOTICE)<<"Finishing";
                //reset alll the videos:
                
                for(auto & video : mVideoWarps){
                   video->goToFirstFrame();
                }
                cur_frame = 0;
            }
            
        }else{
            //update to the global frame
            for(auto & video : mVideoWarps){
                video->updateFrame(cur_frame);
            }
            
            //update video
            for(auto & video : mVideoWarps){
                video->update();
            }
            
            
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(mBkgColor);
    
  
    //draw warps
    if(mDrawWarp){
        ofSetColor(255);
        drawWarps();
    }
    
    //sync debug videos
    if(mSyncVideosDebug){
        drawSyncVideos();
    }
    
    //debug layout videos
    if(mDebugLayoutVideos){
        debugLayoutVideos();
    }
    
    //draw gui
    drawGui();
}
//--------------------------------------------------------------
void ofApp::setupGui(){
    //params
    parameters.setName("Param");
    parameters.add(mBkgColor.set("bkg Color", ofColor(0, 0, 0)));
    parameters.add(mWarpSave.set("Warp Save", false));
    parameters.add(mPlayMovie.set("Toggle Pause", false));
    parameters.add(mResetMovie.set("Reset Movies", false));
    parameters.add(mDebugMovie.set("Debug", false));
    parameters.add(mDrawWarp.set("Draw Warp", true));
    parameters.add(mMasterFrame.set("Master Frame", 0, 0, mMinFrame));
    parameters.add(mSyncVideosDebug.set("Sync Frame", false));
    
    //load parameters
    for( auto & videos : mVideoWarps){
        parameters.add(videos->getParamGroup());
    }
    
    //map params
    auto params = mWarpMapping->getParams();
    for(auto & pam : params){
        parameters.add(pam);
    }
    
    //add listeners
    mPlayMovie.addListener(this, &ofApp::playMovies);
    mResetMovie.addListener(this, &ofApp::resetMovies);
    mDebugMovie.addListener(this, &ofApp::debugMovie);
    mMasterFrame.addListener(this, &ofApp::frameSlider);
    mSyncVideosDebug.addListener(this, &ofApp::syncVideosDebug);
    
    //setup gui
    mGui.setup(parameters);
    
    //minimize the windows
    mGui.getGroup("Video 0").minimize();
    mGui.getGroup("Video 1").minimize();
    mGui.getGroup("Video 2").minimize();
    mGui.getGroup("Video 3").minimize();
    
    mGui.getGroup("Mapping 0").minimize();
    mGui.getGroup("Mapping 1").minimize();
    mGui.getGroup("Mapping 2").minimize();
    mGui.getGroup("Mapping 3").minimize();
    
    //mGui.setSize(300, 200);
    mGui.setPosition(30, 150);
    
    //default values
    mDrawGUI = false;
    mDebugLayoutVideos = false;
    mDebugMovie.set(true);
    mResetMovie.set(true);
    
    mGui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::drawSyncVideos(){
    float wDisplay = ofGetWindowWidth()/(float)numDisplays;
    float hDisplay = ofGetWindowHeight() /(float)numDisplays;
        
    glm::vec2 midScreen(ofGetWindowWidth()/2.0 - wDisplay,ofGetWindowHeight()/2.0 - hDisplay);
        
    //draw the video
    int i = 0;
    int j = 0;
    for(auto & video : mVideoWarps){
        video->draw(midScreen.x + i * wDisplay, midScreen.y + j * hDisplay, wDisplay, hDisplay);
        i++;
        if(i >=2){
            j++;
            i=0;
        }
    }
}
//--------------------------------------------------------------
void ofApp::frameSlider(int & value){
    int framePosition = int(value);
    
    //check if the movie is finished
    if(framePosition >= mMinFrame){
        for(auto & video : mVideoWarps){
            video->goToFirstFrame();
        }
        cur_frame = 0;
    }
           
    //reset alll the videos:
    for(auto & video : mVideoWarps){
        video->updateFrame(framePosition);
    }
        
    //update video
    for(auto & video : mVideoWarps){
        video->update();
    }
    
    //reset alll the videos:

    
    //update frame
    cur_frame = framePosition;
    ofLog(OF_LOG_NOTICE) <<framePosition;
}
//--------------------------------------------------------------
void ofApp::resetMovies(bool & value){
    //  bool status = static_cast<ofParameter<bool>& >(e);
    
    if(value){
        //reset to the begining of the video
        for(auto & video : mVideoWarps){
            video->goToFirstFrame();
        }
        
    }

    playheadControl = 0.0;
    
    ofLog(OF_LOG_NOTICE) << "RESET MOVIE "<<value;
}
//--------------------------------------------------------------
void ofApp::playMovies(bool & value){
    
    //stop movies
    bool status = value;
    
    //stop videos:
    for(auto & video : mVideoWarps){
        video->setPaused(status);
     }
        
    //update to the global frame
    for(auto & video : mVideoWarps){
        video->updateFrame(cur_frame);
    }
    
    //update video
    for(auto & video : mVideoWarps){
        video->update();
    }
    
    mPause = status;
    if (status = true){
        playhead = 0;
    }
    ofLog(OF_LOG_NOTICE) << "Puase MOVIE "<<status;
}

//--------------------------------------------------------------
void ofApp::syncVideosDebug(bool & value){
    
    if(value){
        mDebugMovie.set(false);
        mDrawWarp.set(false);
        
        //update frames
    }
}

//--------------------------------------------------------------
void ofApp::debugMovie(bool & value){
    mDebugLayoutVideos = !mDebugLayoutVideos;
}

//--------------------------------------------------------------
void ofApp::drawVideoTime(int id, int currentFrame, int totalFrame){
    float wDisplay = ofGetWindowWidth()/(float)numDisplays;
    float hDisplay = ofGetWindowHeight() /(float)numDisplays;
    
    ofPushStyle();
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofDrawRectangle(id * wDisplay, hDisplay + 5, wDisplay, 10);
    
    ofSetColor(255, 255, 255);
    int mapFrame = ofMap(currentFrame, 0, totalFrame, 0, wDisplay);
    ofFill();
    ofDrawRectangle(id*wDisplay, hDisplay + 5, mapFrame, 10);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawVideoInfo(int id){
    ofSetColor(255, 255, 255);
    std::string infoName ="Video "+ to_string(id);
    infoName += " :"+to_string(mVideoWarps.at(id)->getCurrentFrame())+" "+to_string(mVideoWarps.at(id)->getTotalNumFrames());
    ofDrawBitmapString(infoName, 20, 50 + id * 20);
}

//--------------------------------------------------------------
void ofApp::drawGui(){
    if (mDrawGUI) {
        
        //draw fps
        ofDrawBitmapString("fps: "+to_string(ofGetFrameRate()), 10, 15);
        ofDrawBitmapString("Frame: "+to_string(cur_frame)+" - "+to_string(mMinFrame), 10, 30);
        
        //draw info videos;
        int i = 0;
        for(auto & video : mVideoWarps){
            drawVideoInfo(i);
            i++;
        }
        
        ofDrawBitmapString(ofPath, 10, 130);
        
        mGui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::drawWarps(){
    
    //get texture for the video and update the warp
    int i = 0;
    for(auto & video : mVideoWarps){
        ofTexture  tex =  video->getTexture();
        if(tex.isAllocated()){
            mWarpMapping->draw(tex, i);
            i++;
        }
    }
}


//--------------------------------------------------------------
void ofApp::debugLayoutVideos(){
    float wDisplay = ofGetWindowWidth()/(float)numDisplays;
    float hDisplay = ofGetWindowHeight() /(float)numDisplays;
    
    float midScreen =  0;//ofGetWindowHeight()/2.0 - hDisplay/2.0;
    
    //draw the video
    int i = 0;
    for(auto & video : mVideoWarps){
        video->draw(wDisplay * i, midScreen, wDisplay, hDisplay);
        i++;
    }
    
    //draw timeline
    int j = 0;
    for(auto & video : mVideoWarps){
        drawVideoTime(j, video->getCurrentFrame(), video->getTotalNumFrames());
        j++;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == '0') {
        std::string avfile = "avset_00.json";
        loadAV(avfile);
        initVideos();
        ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
    }

    if (key == '1') {
        std::string avfile = "avset_00.json";
        loadAV(avfile);
        initVideos();
        ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
    }

    if (key == '2') {
        std::string avfile = "avset_01.json";
        loadAV(avfile);
        initVideos();
        ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
    }

    if (key == '3') {
        std::string avfile = "avset_02.json";
        loadAV(avfile);
        initVideos();
        ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
    }

 

    if (key == ' ') {
        playheadControl = 0.0;
    }
    
    if (key == 'g') {
        mDrawGUI = !mDrawGUI;
        ofLog(OF_LOG_NOTICE) << "gui";
    }
    
    if (key == 's') {
        mWarpMapping->saveWarp();
    }
    
    if(key == 'm'){
        mGui.saveToFile("settings.xml");
    }
    
    if (key == 'z') {
        bool play = false;
        playMovies(play);
    }

    if(key == 'x'){
        bool play = true;
        playMovies(play);
    }

    if (key == 'r') {
        bool reset = true;
        resetMovies(reset);
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
    ofSoundStreamClose();
}
