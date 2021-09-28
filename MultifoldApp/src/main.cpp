#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
#include "config.h"

#include "WindowVideoApp.h"

struct Display{
	glm::vec2 pos;
	glm::vec2 size;
	std::string alias;
	ofColor bkgColor;
	int id;
	int numScreens;
	int decorated;
	shared_ptr<ofAppBaseWindow> videoWindow;
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

	ofColor colors[] = {ofColor(200), ofColor(255), ofColor(150)};
    
    if (file.exists()) {
       // ofLog(OF_LOG_NOTICE) << " Reading Config File " << configFile;
        file >> js;
        
        windwoPos.x = js["position"]["x"];
        windwoPos.y = js["position"]["y"];

		std::string format = js["video"]["format"].get<std::string>();
		orientation = js["video"]["orientation"].get<std::string>();

		std::string ip = js["network"]["ip"].get<std::string>();
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
				d.alias = windows["alias"].get<std::string>();
				d.id = windows["id"];
				d.bkgColor = colors[i];

				ofLog(OF_LOG_NOTICE) << "Found  Window " << d.alias << " " << i;
				ofLog(OF_LOG_NOTICE) << "Num Screens: " << d.numScreens << " size: " << d.size.x << ", " << d.size.y;

				displays.push_back(d);
				i++;
			}
		}

        
        
       // SystemVars::getInstance().numDisplays = numScreens;
        

        
    }else{
        ofLog(OF_LOG_NOTICE)<<"ERROR Reading Config File"<<std::endl;
        decorated  = false;
        windowSize = glm::vec2(1920, 1080);
        windwoPos  = glm::vec2(0, 0 );
    }

	//main windows
	ofLog(OF_LOG_NOTICE) << "Creating Main Window";
	shared_ptr<ofAppBaseWindow> mainWindow;
	if (!displays.empty()) {
		for (auto& d : displays) {
			if (d.alias == "main") {
				ofGLFWWindowSettings settings;
				settings.setSize(d.size.x, d.size.y);
				settings.setPosition(glm::vec2(0, 0));
				settings.resizable = false;
				mainWindow = ofCreateWindow(settings);
			}
		}
	}

	//create windwos
	int i = 0;
	if (!displays.empty() ){
		for (auto& d : displays) {
			ofGLFWWindowSettings settings;

			//dont create a window if its the main layout
			if (d.alias == "main") {
				ofLog(OF_LOG_NOTICE) << "Continue to Main" << i << std::endl;
				i++;
				continue;
			}

			ofLog(OF_LOG_NOTICE) << "Creating window: " << i<<"....";
			
			settings.setGLVersion(4, 1);
			settings.doubleBuffering = true;
			int sizeX = 1920;
			int sizeY = 1080;
			//1920 x 1080
			
			if (orientation == "portrait") {
				if (d.alias != "main") {
					sizeX = d.size.y * d.numScreens;
					sizeY = d.size.x;
					settings.setSize(sizeX, sizeY);
				}

				int posX = 1920 + sizeX * (i);
				int posY = 0;


				ofLog(OF_LOG_NOTICE) << "Window Size: " << sizeX << ", " << sizeY;
				ofLog(OF_LOG_NOTICE) << "Window Location: " << posX << ", " << posY;

				settings.setPosition(glm::vec2(posX, posY));

			}
			else {
					int x = displays[i].size.x * displays[i].numScreens;
					int y = displays[i].size.y;
					settings.setSize(x, y);
			}
			
			ofLog(OF_LOG_NOTICE) << "...";
			settings.decorated = d.decorated;
			settings.resizable = false;
			d.videoWindow = ofCreateWindow(settings);
			d.videoWindow->setVerticalSync(false);

			ofLog(OF_LOG_NOTICE) << "Finishing creating video: " << i << std::endl;
			i++;
		}
	}
	else {
		//if the json failed to read a display settings then create simple main window
		ofGLFWWindowSettings settings;
		settings.setSize(800, 800);
		settings.setPosition(glm::vec2(0, 0));
		settings.resizable = false;
		mainWindow = ofCreateWindow(settings);
	}
    //3 front videos

    //shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    ofLog(OF_LOG_NOTICE) << "Creating Windows Events " <<std::endl;


	shared_ptr<ofApp> mainApp(new ofApp);

	//windows
	shared_ptr<WindowVideoApp> leftApp(new WindowVideoApp);
	shared_ptr<WindowVideoApp> centerApp(new WindowVideoApp);
	shared_ptr<WindowVideoApp> rightApp(new WindowVideoApp);


	ofRunApp(mainWindow, mainApp);


	int j = 0;
	for (auto& d : displays) {
		if (d.alias != "main") {
			shared_ptr<WindowVideoApp> videoApp(new WindowVideoApp);
			videoApp->setBackground(colors[j]);
			ofRunApp(d.videoWindow, videoApp);
			j++;
		}
	}



    ofRunMainLoop();

	ofLog(OF_LOG_NOTICE) << "Finishing Windows Creating " << std::endl;
}
