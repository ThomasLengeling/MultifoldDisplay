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
    
    int numScreens = 1;
    
    if (file.exists()) {
       // ofLog(OF_LOG_NOTICE) << " Reading Config File " << configFile;
        file >> js;
        
        windwoPos.x = js["position"]["x"];
        windwoPos.y = js["position"]["y"];
        
        windowSize.x = js["window"]["width"];
        windowSize.y = js["window"]["height"];
        
        numScreens   = js["window"]["numScreen"];
        decorated    = (js["window"]["decorated"] == 1 ? true : false);
        
        //ofLog(OF_LOG_NOTICE)<<"Size: "<< windowSize.x<<" "<<windowSize.y<<" "<<decorated<<std::endl;
        
    }else{
        //ofLog(OF_LOG_NOTICE)<<"ERROR Reading Config File"<<std::endl;
        decorated  = false;
        windowSize = glm::vec2(1920 * numScreens, 1080);
        windwoPos  = glm::vec2(0, 0 );
    }
    
    //ofGLWindowSettings
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4,1);
    
    settings.setSize(windowSize.x * numScreens, windowSize.y);
    settings.setPosition(windwoPos);
    settings.decorated = decorated;
    settings.resizable = false;
    ofCreateWindow(settings);
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
