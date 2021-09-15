#include "ofApp.h"

static int64_t prev_frame = 0;
static int64_t cur_frame = 0;
static uint8_t offset = 1;

//--------------------------------------------------------------
void ofApp::setup(){

    ofLog(OF_LOG_NOTICE) << "Starting App" << std::endl;
    
    ofSetLogLevel("Logging items", OF_LOG_VERBOSE);
    
    //start HPV engine
   // HPV::InitHPVEngine();
    
    ofSetVerticalSync(false);
    ofSetFrameRate(24.3);
    ofBackground(0);
    ofDisableArbTex();
    
    //number of display
    numDisplays = 4;// SystemVars::getInstance().numDisplays;
    
    ofLog(OF_LOG_NOTICE) << "Num Displays for Videos: "<<numDisplays<<std::endl;
    
    //player type
    //1 -> HPV
    //0 -> HAP
    mPlayerType = 0;
  
    //WARP
    mWarpMapping = inn::Mapping::create(3);
    mWarpMapping->setupWarp(WIDTH_HD, HEIGHT_HD);
    
    //GUI
    setupGui();

    //debug imgs
    loadDebugImgs();


    //load av files
    std::string avfile = "final_videos.json";// "intro_00.json"; /// "idle_00.json";
    loadAV(avfile);
    initVideos();
    playNewVideos = false;
    mPause = true;

    mPort = 32000;
    ofLog(OF_LOG_NOTICE) << "Listening for OSC messages on port " << mPort;
    receiver.setup(mPort);
    
    ofLog(OF_LOG_NOTICE) << "Loading sound" << std::endl;

    ofxSoundUtils::printOutputSoundDevices();
    auto outDevices = ofxSoundUtils::getOutputSoundDevices();

    int outDeviceIndex = ofxSoundUtils::getOutputSoundDevices().size() - 1;
    cout << ofxSoundUtils::getSoundDeviceString(outDevices[outDeviceIndex], false, true) << endl;

    // audio setup for testing audio file stream 
    ofSoundStreamSettings soundSettings;
    soundSettings.numInputChannels = 0;
    soundSettings.numOutputChannels = 2;
    soundSettings.sampleRate = player.getSoundFile().getSampleRate();
    soundSettings.setOutDevice(outDevices[outDeviceIndex]);
 
    cout << " Sample Rate videos " << std::endl;
    cout << player.getSoundFile().getSampleRate()<< std::endl;

    soundSettings.bufferSize = 512;// 256
    soundSettings.numBuffers = 2; //2

    player.volume = 0.3; //.3

    stream.setup(soundSettings);

    stream.setOutput(output);

    if (player.isLoaded()) {
        player.connectTo(output);
        ofLog(OF_LOG_NOTICE) << "Audio added to player: ";
    }
    else {
        ofLog(OF_LOG_NOTICE) << "Error loading adding player to Device output ";
    }

    // set if you want to either have the player looping (playing over and over again) or not (stop once it reaches the its end).
    player.setLoop(true);

    if (!player.getIsLooping()) {
        // if the player is not looping you can register  to the end event, which will get triggered when the player reaches the end of the file.
        playerEndListener = player.endEvent.newListener(this, &ofApp::playerEnded);
    }

    ofLog(OF_LOG_NOTICE)<<"Finishing setup Audio";

  
    mInitialize = false;
    mDebugImgWarp = true;

    ofResetElapsedTimeCounter();
    mInitTimer = 14000;//seconds

    ofLog(OF_LOG_NOTICE) << "Finishing setup";
    ofLog(OF_LOG_NOTICE) << "Size" << ofGetWindowWidth() << " " << ofGetWindowHeight();
}

//--------------------------------------------------------------
void ofApp::setupVideo() {


    ofClear(0);
/*
    if (mDebugImgDisplay.load("imgs/display_hd.png")) {
        mDebugImgDisplay.setUseTexture(true);
        ofLog(OF_LOG_NOTICE) << "Loaded Display Debug Img";
    }
    else {
        ofLog(OF_LOG_NOTICE) << "Error Display Debug Img";

    }
    */
}
//--------------------------------------------------------------
void ofApp::loadDebugImgs() {

    ofImage         mDebugImgLeft;
    ofImage         mDebugImgCenter;
    ofImage         mDebugImgRight;
    ofImage         mDebugImgDisplay;

    //left
    if (mDebugImgLeft.load("imgs/left_hd.png")) {
        ofLog(OF_LOG_NOTICE) << "Loaded Left Debug Img";
        mDebugImgs.push_back(mDebugImgLeft);
    }
    else {
        ofLog(OF_LOG_NOTICE) << "Error Left Debug Img";
    }

    ///center
    if (mDebugImgCenter.load("imgs/center_hd.png")) {
        ofLog(OF_LOG_NOTICE) << "Loaded Center Debug Img";
        mDebugImgs.push_back(mDebugImgCenter);
    }
    else {
        ofLog(OF_LOG_NOTICE) << "Error Center Debug Img";
    }

    //
    if (mDebugImgRight.load("imgs/right_hd.png")) {
        ofLog(OF_LOG_NOTICE) << "Loaded Right Debug Img";
        mDebugImgs.push_back(mDebugImgRight);
    }
    else {
        ofLog(OF_LOG_NOTICE) << "Error Right Debug Img";
    }

    if (mDebugImgDisplay.load("imgs/display_hd.png")) {
        mDebugImgs.push_back(mDebugImgDisplay);
        ofLog(OF_LOG_NOTICE) << "Loaded Display Debug Img";
    }
    else {
        ofLog(OF_LOG_NOTICE) << "Error Display Debug Img";
    }
    //

}
//--------------------------------------------------------------
void ofApp::loadAV(std::string jsonFile) {

    //clear data
    for (auto & videos: mVideoWarps) {
        videos->closeVideo();
    }
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
            ofLog(OF_LOG_NOTICE) << "Loading Videos Type HAP " << int(mPlayerType);
        }
        if (videoType == 1) {
            ofLog(OF_LOG_NOTICE) << "Loading Videos Type HPV " << int(mPlayerType);
        }
        if (videoType == 2) {
            ofLog(OF_LOG_NOTICE) << "Loading Videos Type MOV " << int(mPlayerType);
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
            inn::VideoWarpRef video = inn::VideoWarp::create(mPlayerType, id);
            video->loadVideo(strVideoNames);
            mVideoWarps.push_back(video);

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
        for (auto& video : mVideoWarps) {
            video->setPaused(true);
        }
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
    std::string soundfile = ofToDataPath(filepath);
  
    //set the following to true if you want to stream the audio data from the disk on demand instead of
    //reading the whole file into memory. Default is false
    player.load(soundfile, true);

    player.setPaused(true);
}

//--------------------------------------------------------------
void ofApp::playerEnded(size_t & id) {
    // This function gets called when the player ends. You can do whatever you need to here.
    // This event happens in the main thread, not in the audio thread.
    cout << "the player's instance " << id << "finished playing" << endl;

}

//--------------------------------------------------------------
void ofApp::update() {

	//main video play
	if (mDrawWarp || mSyncVideosDebug) {
		syncVideos();
	}

    //set the master frame with the current change frame
    //mMasterFrame.set(cur_frame);

   // updateOSC();
}



//--------------------------------------------------------------
void ofApp::syncVideos(){

    if (playNewVideos) {

        int loadeadAll = 0;
        for (auto & video : mVideoWarps) {
			ofLog(OF_LOG_NOTICE) << "Wiating to Load all the Videos "<< loadeadAll;
            if (video->isLoaded()) {
                loadeadAll++;
            }
         }

        //all videos are loaded
        if (loadeadAll  == 4 && player.isLoaded() ) {
			ofLog(OF_LOG_NOTICE) << "Loaded 4 Videos Successfully";
            ofLog(OF_LOG_NOTICE) << "Reset Videos";

            player.setPosition(0.0);
            player.play();
            mPause = false;
           // bool reset = true;
           // resetMovies(reset);
            
			//reset
           // bool play = false;
           // playMovies(play);

           // player.setPaused(false);
            //player.play();

			//go out side of the load video loop
            //cur_frame = 0;
            playNewVideos = false;
        }

    }
    
    //HAP and HD
    if(mPlayerType == 0){
        //if (!mPause) {
            if (player.isPlaying()) {
                if (player.isLoaded()) {

                    //update video
                    float audioPos = player.getPosition();
                    for (auto& video : mVideoWarps) {
                        video->setPosition(audioPos);
                    }


                }
                else {
                    ofLog(OF_LOG_NOTICE) << "Audio player not loaded";
                }
            }
            else {
                ofLog(OF_LOG_NOTICE) << "Audio player not playing";
            }

            //increase 
            if (player.getPosition() >= 1.0) {

                int doneMovies = 0;
                for (auto& video : mVideoWarps) {
                    if (video->isDone()) {
                        doneMovies++;
                    }
                }

                if (doneMovies >= 4) {
                    player.play();
                    player.setPosition(0.0);
                    cout << "Reset Player : " << doneMovies;
                }
            }

       // }
    }else if(mPlayerType == 1){ // HPV
        if (player.isPlaying()) {
        

                //update video frame
            if (player.isLoaded()) {
                float audioPos = player.getPosition();
                for (auto& video : mVideoWarps) {
                    video->setPosition(audioPos);
                }
            }
            else {
                ofLog(OF_LOG_NOTICE) << "Cannot play videos because audio is not loaded";
            }

        
        }
        else {
            ofLog(OF_LOG_NOTICE) << "Audio not  Playing";
        }

        if (player.getPosition() >= 1.0) {
            player.setPosition(0.0);
            ofLog(OF_LOG_NOTICE) << "Starting again";
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
    //ofClear(mBkgColor);
    
    //draw warps
    if(mDrawWarp){
        ofSetColor(255);
        drawWarps();

        ofTexture  tex = mVideoWarps.at(3)->getTexture();
        tex.draw(1920*3, 0, 1920, 1080);
    }
    
    if (mSyncVideosDebug) {
        drawSyncVideos();
    }

 
    if (mDebugImgWarp) {
        ofSetColor(255);
        drawDebugWarps();
    }

    if (!mInitialize) {
        if (ofGetElapsedTimeMillis() > mInitTimer) {
            ofLog(OF_LOG_NOTICE) << "Start movie ";

            playNewVideos = true;
            mInitialize = true;
            player.setPaused(false);

            mDrawWarp.set(true);
            mDebugImgWarp.set(false);
            mSyncVideosDebug.set(false);
        }

    }
    
 
 
    //draw gui
    ofSetColor(255);
    drawGui();
}

void ofApp::drawDisplay(ofEventArgs& args) {
   // ofClear(0);

    //draw warps
  if (mDebugImgWarp) { //wird 
   //    ofSetColor(0);
        ofSetColor(255);
        //ofTexture  tex = mDebugImgDisplay.getTexture();
//tex.draw(1920*3, 0, 1920, 1080);
    }else if(mDrawWarp){
        ofSetColor(255);
        ofTexture  tex = mVideoWarps.at(3)->getTexture();
        tex.draw(0, 0, 1920, 1080);
    }

}
//--------------------------------------------------------------
void ofApp::setupGui(){
    //params
    parameters.setName("Param");
    parameters.add(mBkgColor.set("bkg Color", ofColor(0, 0, 0)));
    parameters.add(mWarpSave.set("Warp Save", false));
    parameters.add(mPlayMovie.set("Toggle Pause", false));
    parameters.add(mResetMovie.set("Reset Movies", false));
    parameters.add(mDebugImgWarp.set("Debug Warp", true));
    parameters.add(mSyncVideosDebug.set("Sync Debug", false));
    parameters.add(mDrawWarp.set("Draw Warp", false));
    parameters.add(mMasterFrame.set("Master Frame", 0, 0, mMinFrame));
  
    parameters.add(player.volume);
    
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
    //mDebugWarp.addListener(this, &ofApp::debugMovie);
    mMasterFrame.addListener(this, &ofApp::frameSlider);
    ////mSyncVideosDebug.addListener(this, &ofApp::syncVideosDebug);
    
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
    mResetMovie.set(true);
    
    mGui.loadFromFile("settings.xml");
}
//--------------------------------------------------------------
void ofApp::debugMovie(bool& value) {

   // mDebugMode = value;
}
//--------------------------------------------------------------
//void ofApp::syncVideosDebug(bool& value) {
//    mSyncVideosDebug.set(value);
//}

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

    if (player.isLoaded()) {
        player.setPositionMS(0);
        player.play();
    }
    
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
    if (status == false){

        if (player.isLoaded()) {
            player.setPaused(false);
        }
    }
    else {
        if (player.isLoaded()) {
          player.setPaused(true);
        }
    }

	if (value == false) {
		ofLog(OF_LOG_NOTICE) << "Play MOVIE " << status;
	}
	else {
		ofLog(OF_LOG_NOTICE) << "Pause MOVIE " << status;
	}

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
        //cur_frame = player.ge
        ofDrawBitmapString("fps: "+to_string(ofGetFrameRate()), 10, 15);
       // ofDrawBitmapString("Frame: "+to_string(cur_frame)+" - "+to_string(mMinFrame), 10, 30);
        
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
        if (i == 3) continue;
        ofTexture  tex =  video->getTexture();
        if(tex.isAllocated()){
            mWarpMapping->draw(tex, i);
            i++;
        }
    }
}

//--------------------------------------------------
void ofApp::drawDebugWarps() {
    int i = 0;
    for (auto& video : mVideoWarps) {
        
        ofTexture  tex = mDebugImgs.at(i).getTexture();
        if (tex.isAllocated()) {
            if (i == 3) {
                tex.draw(1920 * 3, 0, 1920, 1080);
            }
            else {
                mWarpMapping->draw(tex, i);
            }
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

    if (key == '1') {

        playNewVideos = true;
    
        player.setPaused(false);
       
        mDrawWarp.set(true);
        mDebugImgWarp.set(false);
        mSyncVideosDebug.set(false);
        ofLog(OF_LOG_NOTICE) << "start videos mode";
    }


    if (key == 'd'){

        player.setPaused(true);

        mDrawWarp.set(false);
        mDebugImgWarp.set(true);
        mSyncVideosDebug.set(false);
        ofLog(OF_LOG_NOTICE) << "debug mode";
    }


 
    //audio
    if (key == ' ') {
        mInitialize = true;
        ofLog(OF_LOG_NOTICE) << "stop init mode";
    }
    
    if (key == 'e') {
        player.setPosition(0.99);
        ofLog(OF_LOG_NOTICE) << "end  mode";
    }
    if (key == 'g') {
        mDrawGUI = !mDrawGUI;
        ofLog(OF_LOG_NOTICE) << "gui";
    }
    
    //save warps
    if (key == 's') {
        mWarpMapping->saveWarp();
    }
    
    if(key == 'm'){
      //  mGui.saveToFile("settings.xml");
    }
    
    if (key == 'z') {
      //  bool play = false;
      //  playMovies(play);
    }

    if(key == 'x'){
      //  bool play = true;
     //   playMovies(play);
    }

    if (key == 'r') {
       // bool reset = true;
      //  resetMovies(reset);
    }
    
}
//--------------------------------------------------------------
void ofApp::updateOSC() {
    while (receiver.hasWaitingMessages()) {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        if (m.getAddress() == "/time") {
            float time = m.getArgAsFloat(0);

            player.stop();

            for (auto& video : mVideoWarps) {
                video->setPosition(time);
            }

            player.setPosition(time);


            ofLog(OF_LOG_NOTICE) << "updated time video: " << time;
        }

        // check for mouse moved message
        if (m.getAddress() == "/av") {
            int id = m.getArgAsInt32(0);
            if (id == 0) {
                std::string avfile = "idle_00.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
            }
            else if (id == 1) {
                std::string avfile = "cai_00.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
            }
            else if (id == 2) {
                std::string avfile = "gld_00.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
            }
            else if (id == 3) {
                std::string avfile = "gld_01.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
            }
            else if (id == 4) {
                std::string avfile = "phc_00.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
            }
            else if (id == 5) {
                std::string avfile = "phc_01.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
            }
            else if (id == 6) {
                std::string avfile = "intro_00.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
            }
            else if (id == 7) {
                std::string avfile = "trade_00.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
            }
            else if (id == 8) {
                std::string avfile = "indoor_00.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
            }
            else if (id == 9) {
                std::string avfile = "festivity_00.json";
                loadAV(avfile);
                initVideos();

                ofLog(OF_LOG_NOTICE) << "loaded new AV :" << avfile;
                playNewVideos = true;
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

        if (m.getAddress() == "/reset") {
            bool reset = true;
            resetMovies(reset);
        }
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
    for (auto & movie : mVideoWarps) {
        movie->close();
    }

    //HPV::DestroyHPVEngine();
   // mWarpMapping->saveWarp();

    player.unload();
    stream.close();
}
