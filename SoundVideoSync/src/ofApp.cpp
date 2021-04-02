#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    HPV::InitHPVEngine();
    HPV::AddEventListener(this, &ofApp::onHPVEvent);

    hpvPlayer01.init(HPV::NewPlayer());
    hpvPlayer02.init(HPV::NewPlayer());
    hpvPlayer03.init(HPV::NewPlayer());
    hpvPlayer04.init(HPV::NewPlayer());

    hpvPlayer01.load("png_monitor.hpv");
    hpvPlayer01.play();
    hpvPlayer01.setPaused(true);
    hpvPlayer01.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer02.load("png_right.hpv");
    hpvPlayer02.play();
    hpvPlayer02.setPaused(true);
    hpvPlayer02.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer03.load("png_left.hpv");
    hpvPlayer03.play();
    hpvPlayer03.setPaused(true);
    hpvPlayer03.setLoopState(OF_LOOP_NORMAL);

    hpvPlayer04.load("png_center.hpv");
    hpvPlayer04.play();
    hpvPlayer04.setPaused(true);
    hpvPlayer04.setLoopState(OF_LOOP_NORMAL);
        
    ofSetVerticalSync(false);
    ofSetFrameRate(25);
        
    
    
    if (soundPlayer.load("within_sound.wav"))
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
    HPV::Update();    
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(0);
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    hpvPlayer01.draw(0, 0, 200, 200);
    hpvPlayer02.draw(200, 0, 200, 200);
    hpvPlayer03.draw(200, 200, 200, 200);
    hpvPlayer04.draw(0, 200, 200, 200);

}

//--------------------------------------------------------------
void ofApp::exit()
{
    hpvPlayer01.close();
    hpvPlayer02.close();
    hpvPlayer03.close();
    hpvPlayer04.close();
 

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
