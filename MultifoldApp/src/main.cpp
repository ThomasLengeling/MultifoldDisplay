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
	int posdisplay; // 0 screen pos(0, 0), 1 screen pos (1920, 0)
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
		for (auto& windows : js["window"]) {
			if (!windows.empty()) {
				Display d;
				d.size.x = windows["width"];
				d.size.y = windows["height"];
				d.decorated = windows["decorated"];
				d.numScreens = windows["numscreen"];
				d.alias = windows["alias"].get<std::string>();
				d.id = windows["id"];
				d.bkgColor = colors[i];
				d.posdisplay = windows["posx"];

				ofLog(OF_LOG_NOTICE) << "Found  Window " << d.alias << " " << i;
				ofLog(OF_LOG_NOTICE) << "Num Screens: " << d.numScreens << " size: " << d.size.x << ", " << d.size.y;

				if (d.alias == "individual") {
					for (int i = 0; i < d.numScreens; i++) {
						displays.push_back(d);
					}
				}
				else {
					displays.push_back(d);
				}
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

	//Creating windows 
	ofLog(OF_LOG_NOTICE) << "Creating Windows....." << std::endl;
	shared_ptr<ofAppBaseWindow> mainWindow;
	int i = 0;
	if (!displays.empty()) {
		for (auto& d : displays) {
			if (d.alias == "main") {
				ofLog(OF_LOG_NOTICE) << "Creating Main Window"; 
				ofGLFWWindowSettings settings;
				settings.setSize(d.size.x, d.size.y);
				settings.setPosition(glm::vec2(d.posdisplay, 0));
				settings.resizable = false;
				settings.decorated = d.decorated;
				settings.setGLVersion(4, 1);

				mainWindow = ofCreateWindow(settings);
				mainWindow->setVerticalSync(true);

			}
			ofLog(OF_LOG_NOTICE) << " " << std::endl;
			if (d.alias == "individual") {
				if (i < d.numScreens) {
					ofLog(OF_LOG_NOTICE) << "Creating inidivual windows: " << i << "....";
					ofGLFWWindowSettings settings;
					settings.setGLVersion(4, 1);

					int sizeX = d.size.y;
					int sizeY = d.size.x;
					settings.setSize(sizeX, sizeY);

					int posX = d.posdisplay + sizeX * (i);
					int posY = 0;
					settings.setPosition(glm::vec2(posX, posY));
					//settings.setPosition(glm::vec2(0, 0));

					ofLog(OF_LOG_NOTICE) << "Window Size: " << sizeX << ", " << sizeY;
					ofLog(OF_LOG_NOTICE) << "Window Location: " << posX << ", " << posY;

					ofLog(OF_LOG_NOTICE) << " <<<<< ";
					
					//settings.doubleBuffering = true;
					settings.decorated = d.decorated;
					settings.resizable = false;

					//settings.shareContextWith = mainWindow;
					//settings.windowMode = OF_FULLSCREEN;
					//settings.monitor = 3;
				
					//settings.monitor = i;
					//settings.multiMonitorFullScreen = true;
					d.videoWindow = ofCreateWindow(settings);
					d.videoWindow->setVerticalSync(false);
					

					ofLog(OF_LOG_NOTICE) << "Finishing creating Window: " << i << " " << 9 - i << std::endl;

					i++;
				}
			}
		}
	}

    
    ofLog(OF_LOG_NOTICE) << "Creating Windows Events " <<std::endl;


	shared_ptr<ofApp> mainApp(new ofApp);
	ofRunApp(mainWindow, mainApp);


	int j = 0;
	for (auto& d : displays) {
		if (d.alias != "main") {
			if (j < d.numScreens) {
				shared_ptr<WindowVideoApp> videoApp(new WindowVideoApp);
				videoApp->setId(j);
				videoApp->setBackground(colors[0]);
				ofRunApp(d.videoWindow, videoApp);
				j++;
			}
		}
	}

    ofRunMainLoop();

	ofLog(OF_LOG_NOTICE) << "Finishing Windows Creating " << std::endl;
}
