#include "ofApp.h"

static int64_t prev_frame = 0;
static int64_t cur_frame = 0;
static uint8_t offset = 1;

//--------------------------------------------------------------
void ofApp::setup(){

    ofLog(OF_LOG_NOTICE) << "Starting App" << std::endl;
    ofSetLogLevel("Logging items", OF_LOG_VERBOSE);

    //change path
    ofSetDataPathRoot("C:/Users/Bizon/Desktop/App/data/");
    ofPath = ofFilePath::getAbsolutePath(ofToDataPath("C:/Users/Bizon/Desktop/App/data/"));
    ofLog(OF_LOG_NOTICE) << " OF data path is: " << ofPath << endl;
    
    ofSetFrameRate(30);
    ofBackground(0);
    ofDisableArbTex();
 
    //Video
    HPV::InitHPVEngine();

    //GUI
    setupGui();
    
    //load UDP information
    setupUDP();

    //OSC
    setupOSC();

    //audio
    setupAudioSystem();
    
    //commom state
    setupCommonState();

    ofResetElapsedTimeCounter();
    mInitTimer = 14000;//seconds
    mStartVideoLoop = false;

    ofLog(OF_LOG_NOTICE) << "Finishing setup";
    ofLog(OF_LOG_NOTICE) << "Size" << ofGetWindowWidth() << " " << ofGetWindowHeight();
}


//--------------------------------------------------------------
void ofApp::update() {

    //set the master frame with the current change frame
    //mMasterFrame.set(cur_frame);


    if (mMasterUDP) {
        float audioPos = player.getPosition();
        mMasterAudio.set(audioPos);

        if (mStartVideoLoop) {
            sendAudioPosUDP(audioPos);
        }
    }

    //if receiving udp then is a salve
    if (mSlaveUDP) {
        updateUDP();
    }

    //update osc
   // updateOSC();


    //updateUDP(float audioPos);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    drawGui();
  
}

//--------------------------------------------------------------
void ofApp::sendAudioPosUDP(float audioPos) {
    
    //
    mCommon->mAudioPos = audioPos;

        //update udp
    string msg = "t ";
    msg += to_string(audioPos);
    udpSendLeft.Send(msg.c_str(), msg.length());
    udpSendCenter.Send(msg.c_str(), msg.length());
}
//--------------------------------------------------------------
void ofApp::updateUDP() {

    char udpMessage[1000];
    udpReceiver.Receive(udpMessage, 1000);
    string message = udpMessage;
    if (message != "") {

        //start
        if (message[0] == 'a') {
            mCommon->startVideo = true;
        }
        if (message[0] == 's') {
            //send UDP start
            mCommon->startVideo = false;
            player.setPosition(0.0);
            player.setPaused(true);
        }
        // send time
        if (message[0] == 't') {
            auto smsg = string_split(message);
            mCommon->mAudioPos = std::stof(smsg[1]);
            mMasterAudio.set(mCommon->mAudioPos);
        }
        //load new video
        if (message[0] == 'n') {
            ofLog(OF_LOG_NOTICE) << "Slave new video sequence ";
            auto smsg = string_split(message);
            mCommon->mSequenceId = std::stof(smsg[1]);

            loadSequence(mCommon->mSequenceId);
        }
    }

}

//--------------------------------------------------------------
void ofApp::setupCommonState() {
    mCommon->startVideo = false;
}

//--------------------------------------------------------------
void ofApp::setupUDP() {
    ofLog(OF_LOG_NOTICE) << "Loading UDP info ";
    ofJson js;
    std::string configFile = "network.json";
    ofFile file(configFile);

    if (file.exists()) {
        // ofLog(OF_LOG_NOTICE) << " Reading Config File " << configFile;
        file >> js;

        std::string ip = js["network"]["ip"].get<std::string>();
        int port0 = js["network"]["port0"]; //center
        int port1 = js["network"]["port1"]; //left

        ofLog(OF_LOG_NOTICE) << "Network: " << port0 << "  " << port1 << " " << std::endl;

        if (mCommon->mId == 0) {
            ofLog(OF_LOG_NOTICE) << "Set Master UDP ";
            mMasterUDP = true;
            mSlaveUDP = false;
            mUDPPortCenter = port0; //center
            mUDPPortLeft = port1;  //left
        }else{
            ofLog(OF_LOG_NOTICE) << "Set Slave UDP " << mCommon->mId;
            mMasterUDP = false;
            mSlaveUDP = true;
            //Center
            if (mCommon->mId == 1) {
                mUDPPortReceiver = port0;
            }
            //Left
            if (mCommon->mId == 2) {
                mUDPPortReceiver = port1;
            }
        }
    }
    else {
        ofLog(OF_LOG_NOTICE) << "Cannot Load UDP info ";
    }

    //Reivers
    if (mSlaveUDP) {
        ofLog(OF_LOG_NOTICE) << "Setting Receiver Port:  " << mUDPPortReceiver << std::endl;
        ofxUDPSettings settings;
        settings.receiveOn(mUDPPortReceiver);
        settings.blocking = false;
        udpReceiver.Setup(settings);
    }

    //sender Master
    if (mMasterUDP) {
        ofLog(OF_LOG_NOTICE) << "Setting Master Sender Ports: " << mUDPPortCenter << " " << mUDPPortLeft << std::endl;
        ofxUDPSettings settings;
        settings.sendTo("127.0.0.1", mUDPPortCenter);
        settings.blocking = false;
        udpSendCenter.Setup(settings);

        settings.sendTo("127.0.0.1", mUDPPortLeft);
        settings.blocking = false;
        udpSendLeft.Setup(settings);
    }

}

//--------------------------------------------------------------
void ofApp::setupOSC() {
    mPort = 32000;
    ofLog(OF_LOG_NOTICE) << "Listening for OSC messages on port " << mPort;
    receiver.setup(mPort);
}

//--------------------------------------------------------------
void ofApp::loadAudio(int seqId) {

    //audio path;
    std::string audioPath = "";

    //load audio
    ofLog(OF_LOG_NOTICE) << "Loading sound.." << std::endl;
    audioPath = mAudioPaths.at(seqId);

    std::string audioDataPath = ofToDataPath(audioPath);

    //set the following to true if you want to stream the audio data from the disk on demand instead of
    //reading the whole file into memory. Default is false
    player.load(audioDataPath, true);

    player.setPaused(true);
}
//--------------------------------------------------------------
void ofApp::setupAudioSystem() {

    loadAudio(mCommon->mSequenceId);

    ofxSoundUtils::printOutputSoundDevices();
    auto outDevices = ofxSoundUtils::getOutputSoundDevices();

    int outDeviceIndex = ofxSoundUtils::getOutputSoundDevices().size() - 1;
    ofLog(OF_LOG_NOTICE) << ofxSoundUtils::getSoundDeviceString(outDevices[outDeviceIndex], false, true) << endl;

    // audio setup for testing audio file stream 
    ofSoundStreamSettings soundSettings;
    soundSettings.numInputChannels = 0;
    soundSettings.numOutputChannels = 2;
    soundSettings.sampleRate = player.getSoundFile().getSampleRate();
    soundSettings.setOutDevice(outDevices[outDeviceIndex]);

    ofLog(OF_LOG_NOTICE) << " Sample Rate Sound File " << std::endl;
    ofLog(OF_LOG_NOTICE) << player.getSoundFile().getSampleRate() << std::endl;

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

    ofLog(OF_LOG_NOTICE) << "Finishing setup Audio";
}

//--------------------------------------------------------------
void ofApp::setupGui(){
    //params
    parameters.setName("Param");
    parameters.add(mPlayVideos.set("Toggle Play", false));
    parameters.add(mResetVideos.set("Reset Videos", false));
    parameters.add(mMasterAudio.set("Master Audio", 0, 0, 1.0));
    parameters.add(player.volume);
  
    //add listeners
    mPlayVideos.addListener(this, &ofApp::playVideos);
    mResetVideos.addListener(this, &ofApp::resetVideos);
    mMasterAudio.addListener(this, &ofApp::audioSlider);
    
    //setup gui
    mGui.setup(parameters);

    //mGui.setSize(300, 200);
    mGui.setPosition(20, 70);
    
    //default values
    mDrawGUI = true;
    
    mGui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::audioSlider(float & value){
    float audioPosition = float(value);

    player.setPosition(value);
    //ofLog(OF_LOG_NOTICE) << "New Audio Position: "<< value;

    //send new position to the slaves
    if (mMasterUDP) {
        sendAudioPosUDP(0.0);
    }
       
    //update frame
    //cur_frame = framePosition;
    //ofLog(OF_LOG_NOTICE) <<framePosition;
}
//--------------------------------------------------------------
void ofApp::resetVideos(bool & value){
    
    ofLog(OF_LOG_NOTICE) << "RESET MOVIE "<<value;
}
//--------------------------------------------------------------
void ofApp::playVideos(bool & value){
  

}

//--------------------------------------------------------------
void ofApp::playerEnded(size_t& id) {
    // This function gets called when the player ends. You can do whatever you need to here.
    // This event happens in the main thread, not in the audio thread.
    cout << "the player's instance " << id << "finished playing" << endl;

}

//--------------------------------------------------------------
void ofApp::drawGui() {
    if (mDrawGUI) {
        ofSetColor(255);
        ofDrawBitmapString("fps: " + to_string(ofGetFrameRate()), 10, 15);
        ofDrawBitmapString("Frame: " + to_string(cur_frame) + " - " + to_string(mMinFrame), 10, 30);

        ofDrawBitmapString(ofPath, 10, 400);
        ofDrawBitmapString("Current Sequence Name: "+mCommon->mCurrentSeqName, 10, 250);

        int i = 0;
        for (auto names : mCommon->mSeqNames) {
            ofDrawBitmapString(names, 10, 280 + i*20);
            i++;
        }

        if (mMasterUDP) {
            ofSetColor(255, 0, 0);
            ofDrawBitmapString("Master "+to_string(mCommon->mId), 350, 100);

            ofDrawRectangle(355, 120, 30, 30);
        }
        if (mSlaveUDP) {
            ofSetColor(255);
            ofDrawBitmapString("Slave " + to_string(mUDPPortReceiver)+"   "+to_string(mCommon->mId), 350, 100);   
        }
        mGui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::stopAudio() {
    if (mMasterUDP) {
        ofLog(OF_LOG_NOTICE) << "Send Master Stop";

        //reset audio pos
        player.setPosition(0.0);
        player.setPaused(true);

        //start video loop
        mStartVideoLoop = false;

        //send UDP start
        mCommon->startVideo = false;

        string message = "s";
        udpSendLeft.Send(message.c_str(), message.length());
        udpSendCenter.Send(message.c_str(), message.length());

        mMasterAudio.set(0.0);
        sendAudioPosUDP(0.0);

    }
}


//--------------------------------------------------------------
void ofApp::loadSequence(int id) {
    if (id < mCommon->mSeqNames.size()) {
        ofLog(OF_LOG_NOTICE) << "Loading new sequence ";

        mCommon->mSequenceId = id;
        loadAudio(mCommon->mSequenceId);
        //update names
        mCommon->mCurrentSeqName = mCommon->mSeqNames.at(mCommon->mSequenceId);
        player.volume.setName(mCommon->mCurrentSeqName);

        //send to activate video 
        mStartVideoLoop = false;
        mCommon->startVideo = false;

        std::fill(mCommon->vNewVideos.begin(), mCommon->vNewVideos.end(), true);

        //sned to activate video 1
        //stop load playnew
        if (mMasterUDP) {

            string message = "n 0";
            udpSendLeft.Send(message.c_str(), message.length());
            udpSendCenter.Send(message.c_str(), message.length());
        }
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == '1') {
        loadSequence(0);
    }
    if (key == '2') {
        loadSequence(1);
    }
    if (key == '3') {
        loadSequence(2);
    }
    if (key == '4') {
        loadSequence(3);
    }
    if (key == '5') {
        loadSequence(4);
    }




    //audio
    if (key == ' ') {

        mCommon->startVideo = true;

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
    
    //send start
    if (key == 'a') {
        if (mMasterUDP) {
            ofLog(OF_LOG_NOTICE) << "send Master";

            //reset audio pos
            player.setPaused(false);
            player.setPosition(0.0);
            player.play();

            //start video loop
            mStartVideoLoop = true;

            //send UDP start
            mCommon->startVideo = true;
            string message = "a";
            udpSendLeft.Send(message.c_str(), message.length());
            udpSendCenter.Send(message.c_str(), message.length());
        }
    }
    if (key == 's') {
        stopAudio();
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

            //for (auto& video : mVideoWarps) {
            //    video->setPosition(time);
            //}

            player.setPosition(time);


            ofLog(OF_LOG_NOTICE) << "updated time video: " << time;
        }

        if (m.getAddress() == "/stop") {
            bool play = true;
            playVideos(play);
        }

        if (m.getAddress() == "/play") {
            bool play = false;
            playVideos(play);
        }

        if (m.getAddress() == "/reset") {
            bool reset = true;
            resetVideos(reset);
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

    HPV::DestroyHPVEngine();

    player.unload();
    stream.close();
}

//--------------------------------------------------------------
std::vector<std::string> ofApp::string_split(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    for (std::string s; iss >> s; )
        result.push_back(s);
    return result;
}