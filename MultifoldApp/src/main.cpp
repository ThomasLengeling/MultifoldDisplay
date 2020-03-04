#include "ofMain.h"
#include "ofApp.h"
#include "config.h"

//========================================================================
int main( ){
    
    //write json and config files
    ofJson js;
    std::string configFile = "config.json";
    ofFile file(configFile);
    
    glm::vec2  windowSize;
    glm::vec2  windwoPos;
    bool decorated = false;
    
    if (file.exists()) {
        ofLog(OF_LOG_NOTICE) << " Reading Config File " << configFile;
        file >> js;
        
        windwoPos.x = js["position"]["x"];
        windwoPos.y = js["position"]["y"];
        
        windwoPos.x = js["window"]["width"];
        windwoPos.y = js["window"]["height"];
        
        decorated = (js["window"]["decorated"] == 1 ? true : false);
        
        std::cout<<"size: "<< windwoPos.x<<" "<<windwoPos.y<<" "<<decorated<<std::endl;
        
    }else{
        std::cout<<"ERROR Reading Config File"<<std::endl;
        decorated  = false;
        windowSize = glm::vec2(1920, 1080);
        windwoPos  = glm::vec2(0, 0 );
    }
    
    //ofGLWindowSettings
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4,0);
    
    settings.setSize(windwoPos.x, windwoPos.y);
    settings.setPosition(windwoPos);
    settings.decorated = decorated;
    ofCreateWindow(settings);
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
