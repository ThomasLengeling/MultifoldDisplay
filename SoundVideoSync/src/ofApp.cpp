#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    HPV::InitHPVEngine(true);
    HPV::AddEventListener(this, &ofApp::onHPVEvent);

    hpvPlayer01.init(HPV::NewPlayer());
    hpvPlayer02.init(HPV::NewPlayer());
    hpvPlayer03.init(HPV::NewPlayer());
    hpvPlayer04.init(HPV::NewPlayer());
    hpvPlayer05.init(HPV::NewPlayer());
    hpvPlayer06.init(HPV::NewPlayer());
    hpvPlayer07.init(HPV::NewPlayer());
    hpvPlayer08.init(HPV::NewPlayer());
    hpvPlayer09.init(HPV::NewPlayer());

    hpvPlayer01.load("test.hpv");
    hpvPlayer01.play();
    hpvPlayer01.setPaused(true);
    hpvPlayer01.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer02.load("test.hpv");
    hpvPlayer02.play();
    hpvPlayer02.setPaused(true);
    hpvPlayer02.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer03.load("test.hpv");
    hpvPlayer03.play();
    hpvPlayer03.setPaused(true);
    hpvPlayer03.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer04.load("test.hpv");
    hpvPlayer04.play();
    hpvPlayer04.setPaused(true);
    hpvPlayer04.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer05.load("test.hpv");
    hpvPlayer05.play();
    hpvPlayer05.setPaused(true);
    hpvPlayer05.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer06.load("test.hpv");
    hpvPlayer06.play();
    hpvPlayer06.setPaused(true);
    hpvPlayer06.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer07.load("test.hpv");
    hpvPlayer07.play();
    hpvPlayer07.setPaused(true);
    hpvPlayer07.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer08.load("test.hpv");
    hpvPlayer08.play();
    hpvPlayer08.setPaused(true);
    hpvPlayer08.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer09.load("test.hpv");
    hpvPlayer09.play();
    hpvPlayer09.setPaused(true);
    hpvPlayer09.setLoopState(OF_LOOP_NORMAL);
        
    ofSetVerticalSync(false);
    ofSetFrameRate(60);
        
    
    
    if (soundPlayer.load("test.wav"))
    {
        soundPlayer.play();
        soundPlayer.setLoop(true);
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    float soundPos = soundPlayer.getPosition();
    hpvPlayer01.seekToPos(soundPos);
    hpvPlayer02.seekToPos(soundPos);
    hpvPlayer03.seekToPos(soundPos);
    hpvPlayer04.seekToPos(soundPos);
    hpvPlayer05.seekToPos(soundPos);
    hpvPlayer06.seekToPos(soundPos);
    hpvPlayer07.seekToPos(soundPos);
    hpvPlayer08.seekToPos(soundPos);
    hpvPlayer09.seekToPos(soundPos);
    HPV::Update();    
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(0);
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    hpvPlayer01.draw(0, 0, 200, 200);
    hpvPlayer02.draw(200, 0, 200, 200);
    hpvPlayer03.draw(400, 0, 200, 200);

    hpvPlayer04.draw(0, 200, 200, 200);
    hpvPlayer05.draw(200, 200, 200, 200);
    hpvPlayer06.draw(400, 200, 200, 200);

    hpvPlayer07.draw(0, 400, 200, 200);
    hpvPlayer08.draw(200, 400, 200, 200);
    hpvPlayer09.draw(400, 400, 200, 200);

}

//--------------------------------------------------------------
void ofApp::exit()
{
    hpvPlayer01.close();
    hpvPlayer02.close();
    hpvPlayer03.close();
    hpvPlayer04.close();
    hpvPlayer05.close();
    hpvPlayer06.close();
    hpvPlayer07.close();
    hpvPlayer08.close();
    hpvPlayer09.close();
 
    soundPlayer.unloadSound();
    soundPlayer.stop();
    HPV::DestroyHPVEngine();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == 'r')
    {
        soundPlayer.setPosition(0);
    }
    else if (key == 'f')
    {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::onHPVEvent(const HPVEvent& event)
{
    switch (event.type)
    {
        case HPV::HPVEventType::HPV_EVENT_PLAY:
            cout << "'" << event.player->getFilename() << "': play event" << endl;
            break;
        case HPV::HPVEventType::HPV_EVENT_PAUSE:
            cout << "'" << event.player->getFilename() << "': pause event" << endl;
            break;
        case HPV::HPVEventType::HPV_EVENT_RESUME:
            cout << "'" << event.player->getFilename() << "': resume event" << endl;
            break;
        case HPV::HPVEventType::HPV_EVENT_STOP:
            cout << "'" << event.player->getFilename() << "': stop event" << endl;
            break;
        case HPV::HPVEventType::HPV_EVENT_LOOP:
            cout << "'" << event.player->getFilename() << "': loop event" << endl;
            break;
        case HPV::HPVEventType::HPV_EVENT_NUM_TYPES:
        default:
            break;
    }
}
