#include "ofMain.h"
#include "ofApp.h"
#include "config.h"

struct Display{
	glm::vec2 pos;
	glm::vec2 size;
	std::string alias;
	int id;
	int numScreens;
	int decorated;
}display;

//========================================================================
int main( ){
    
    //write json and config files
    ofJson js;
    std::string configFile = "config.json";
    ofFile file(configFile);
    
    glm::vec2  windowSize;
    glm::vec2  windwoPos;
    bool decorated = false;
	
	std::vector<Display> displays;
	std::string orientation = "landscape";
    
    int numScreens = 1;
    
    if (file.exists()) {
       // ofLog(OF_LOG_NOTICE) << " Reading Config File " << configFile;
        file >> js;
        
        windwoPos.x = js["position"]["x"];
        windwoPos.y = js["position"]["y"];

		std::string format = js["video"]["format"];
		orientation = std::string(js["video"]["orientation"]);

		std::string ip = js["network"]["ip"];
		int port = js["network"]["port"];

		ofLog(OF_LOG_NOTICE) << "Position: " << windwoPos.x<< ", " << windwoPos.y;
		ofLog(OF_LOG_NOTICE) << "Network: " <<ip<< "  " << port;
		ofLog(OF_LOG_NOTICE) << "Video Format " << format << " Orientation: " << orientation;


		//load individual windows
		
		int i = 0;
		for (auto & windows : js["window"]) {
			if (!windows.empty()) {
				Display d;
				d.size.x = windows["width"];
				d.size.y = windows["height"];
				d.decorated = windows["decorated"];
				d.numScreens = windows["numscreen"];
				d.alias = std::string(windows["alias"]);
				d.id = windows["id"];

				ofLog(OF_LOG_NOTICE) << "Found  Window " << d.alias << " " << i;
				ofLog(OF_LOG_NOTICE) << "Num Screens: " << d.numScreens << " size: " << d.size.x << ", " << displays[i].size.y;

				displays.push_back(d);
			}
		}

        
        
       // SystemVars::getInstance().numDisplays = numScreens;
        

        
    }else{
        ofLog(OF_LOG_NOTICE)<<"ERROR Reading Config File"<<std::endl;
        decorated  = false;
        windowSize = glm::vec2(1920, 1080);
        windwoPos  = glm::vec2(0, 0 );
    }

	//create windwos
	for (auto & d : displays){
		ofGLFWWindowSettings settings;
		settings.setGLVersion(4, 1);
		settings.doubleBuffering = true;
		int sizeX = 1920;
		int sizeY = 1080;
		if (orientation == "portrait") {
			if (d.alias != "main") {
				sizeX = displays[i].size.y * displays[i].numScreens;
				sizeY = displays[i].size.x;
				settings.setSize(sizeX, sizeY);
			}
			else {

			}

			int posX = 1920 + 0;
			int posY = 0;
			if (i >= 1) {
				posX = 1920 + sizeX * (i-1);
				posY = 0;
			}
			if (d.alias == "main") {
				posX = 0;
				posY = 0;
			}

			ofLog(OF_LOG_NOTICE) << "Window Size: " << sizeX << ", " << sizeY;
			ofLog(OF_LOG_NOTICE) << "Window Location: " << posX << ", " << posY;

			settings.setPosition(glm::vec2(posX, posY));
		}
		else {
			int x = displays[i].size.x * displays[i].numScreens;
			int y = displays[i].size.y;
			settings.setSize(x, y);
		}

		settings.decorated = decorated;
		settings.resizable = false;
		ofCreateWindow(settings);
	}
    //3 front videos

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
