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
        
        SystemVars::getInstance().numDisplays = numScreens;
        
        decorated    = (js["window"]["decorated"] == 1 ? true : false);
        
        ofLog(OF_LOG_NOTICE)<<"Size: "<< windowSize.x<<" "<<windowSize.y<<" "<<decorated<<std::endl;

        ofLog(OF_LOG_NOTICE) << "Num Screens: " << numScreens << std::endl;

		//final installation
		// type = 1 final, single video
		// type = 0 testing, videos can be change dynamically.
		//int installationType = js["installation"]["type"];

        
    }else{
        ofLog(OF_LOG_NOTICE)<<"ERROR Reading Config File"<<std::endl;
        decorated  = false;
        windowSize = glm::vec2(1920, 1080);
        windwoPos  = glm::vec2(0, 0 );
    }
    
    //3 front videos
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4,1);
    //settings.multiMonitorFullScreen = true;
    settings.doubleBuffering = true;
    settings.setSize(windowSize.x * 4, windowSize.y);
    settings.setPosition(glm::vec2(-1920 * 4, 0));
    settings.decorated = decorated;
    //settings.resizable = false;
    ofCreateWindow(settings);
    //shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    ofLog(OF_LOG_NOTICE) << "3 HD Warp created Window Size: " <<ofGetWindowWidth()<<" "<<ofGetWindowHeight()<<std::endl;

    //4k video for table display
    //3840, 2160
    //1920 x 108
    settings.setGLVersion(4, 1);
    settings.setSize(1920, 1080);
    settings.setPosition(glm::vec2(-1920, 0));
    settings.decorated = false;
    settings.resizable = false;
    // uncomment next line to share main's OpenGL resources with gui
   // settings.shareContextWith = mainWindow;
   // shared_ptr<ofAppBaseWindow> largeWindow = ofCreateWindow(settings);

   // shared_ptr<ofApp> mainApp(new ofApp);
    //mainApp->setupVideo();
   // ofAddListener(largeWindow->events().draw,mainApp.get(),&ofApp::drawDisplay);

   
    //mainApp->setupGui();
   ofRunApp(new ofApp());
    //ofRunApp(mainWindow, mainApp);
    //ofRunMainLoop();
}
