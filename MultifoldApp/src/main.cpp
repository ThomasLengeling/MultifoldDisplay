#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
#include "config.h"

#include "WindowVideoApp.h"

struct Display{
	glm::vec2 pos;
	glm::vec2 size;
	std::string type;
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
	int displayId;

	ofColor colors[] = {ofColor(200), ofColor(255), ofColor(150)};
    
    if (file.exists()) {
       // ofLog(OF_LOG_NOTICE) << " Reading Config File " << configFile;
        file >> js;
        
        windwoPos.x = js["position"]["x"];
        windwoPos.y = js["position"]["y"];

		std::string format = js["video"]["format"].get<std::string>();
		orientation = js["video"]["orientation"].get<std::string>();

		std::string ip = js["network"]["ip"].get<std::string>();
		int port = js["network"]["port0"];

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
				d.type = windows["type"].get<std::string>();
				d.alias = windows["alias"].get<std::string>();
				d.id = windows["id"];
				d.bkgColor = colors[i];
				d.posdisplay = windows["posx"];

				ofLog(OF_LOG_NOTICE) << "Found  Window " << d.alias << " " <<d.id<<" "<< i;
				ofLog(OF_LOG_NOTICE) << "Num Screens: " << d.numScreens << " size: " << d.size.x << ", " << d.size.y;

				if (d.type == "individual") {
					for (int i = 0; i < d.numScreens; i++) {
						displays.push_back(d);
					}
					displayId = d.id;
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
			if (d.type == "main") {
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
			if (d.type == "individual") {
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
					d.videoWindow = ofCreateWindow(settings);
					d.videoWindow->setVerticalSync(false);
					

					ofLog(OF_LOG_NOTICE) << "Finishing creating Window: " << i << " " << 9 - i << std::endl;

					i++;
				}
			}
		}
	}

	//set data path
	ofLog(OF_LOG_NOTICE) << "Create Videos ";
	ofSetDataPathRoot("C:/Users/Bizon/Desktop/App/data");

	//video name files:
	std::map<int, std::string> videoNamesMap;
	std::map<int, std::string> sequenceName;
	std::map<int, std::string> soundNameMap;

	std::string videoStr = "video.json";
	ofFile videoFile(videoStr);
	ofJson jsVideo;
	if (videoFile.exists()) {
		ofLog(OF_LOG_NOTICE) << " Reading Video File Names ";
		videoFile >> jsVideo;
		int j = 0;

		//read sequence
		for (auto& sequence : jsVideo["sequence"]) {
			std::string name = sequence["name"];
			int id = sequence["id"];
			ofLog(OF_LOG_NOTICE) << " Loading video Sequence: " << name << " " << id << std::endl;
			sequenceName.insert(std::pair<int, std::string>(id, name));
		}

		//load videos names
		for (auto & vName : jsVideo["videos"]) {
			std::string name = vName["name"];
			int id = vName["id"];  
			videoNamesMap.insert(std::pair<int, std::string>(id, name));
		}
		//load sound names
		for (auto & vAudios : jsVideo["audios"]) {
			std::string name = vAudios["name"];
			int id = vAudios["id"];
			soundNameMap.insert(std::pair<int, std::string>(id, name));
		}
	}
	//
	for (auto & files : videoNamesMap) {
		ofLog(OF_LOG_NOTICE) << files.first <<" "<<files.second<<std::endl;
	}

	ofLog(OF_LOG_NOTICE) << "Creating Windows Events " << std::endl;

	shared_ptr<CommonState> commonState(new CommonState);
	commonState->mCurrentSeqName = sequenceName[0];
	commonState->mSequenceId = 0;
	commonState->mId = displayId;

	std::vector<std::string> tmpNames;
	for (auto& name : sequenceName) {
		tmpNames.push_back(name.second);
	}
	commonState->mSeqNames = tmpNames;

	shared_ptr<ofApp> mainApp(new ofApp);
	mainApp->mCommon = commonState;

	for (auto& seqName : sequenceName) {
		for (auto& soundName : soundNameMap) {
			std::string path = "video/" + seqName.second + "/" + seqName.second + "_" + soundName.second;
			ofLog(OF_LOG_NOTICE) << "Adding sound file: " << " " << path;
			mainApp->addSoundPath(path);
		}
	}

	//run main Window
	ofRunApp(mainWindow, mainApp);

	ofLog(OF_LOG_NOTICE) << "Create Displays with videos "<<std::endl;
	//create Displays with Videos
	int j = 0;
	for (auto& d : displays) {
		if (d.type != "main") {
			if (j < d.numScreens) {
				ofLog(OF_LOG_NOTICE) <<std::endl<< "Display ... "<< j;
				shared_ptr<WindowVideoApp> videoApp(new WindowVideoApp);
				videoApp->mCommon = commonState;
				videoApp->setId(j);
				videoApp->setBackground(colors[0]); 
				//videoApp->setVideoName(videoNamesMap[j]);

				//add several sequences depending on how many sequences are available
				
				videoApp->mCommon->mCurrentSeqName = sequenceName[0];
				videoApp->mCommon->mSequenceId = 0;
				videoApp->mCommon->mAlias = d.alias;
				videoApp->mCommon->mId = d.id;

				//add all the names types to each display
				for (auto& seqNam : sequenceName) {
					std::string seqName = seqNam.second;
					
					if (d.id == 0) {  //displays 1-3
						std::string path = "video/" + seqName + "/" + seqName + "_" + videoNamesMap[j];
						ofLog(OF_LOG_NOTICE) << "Adding file: " << seqNam.first << " " << seqNam.second << " " << j << " " << path;
						videoApp->addSequence(path);
					}
					if (d.id == 1) { //diplays 4-6
						std::string path = "video/" + seqName + "/" + seqName + "_" + videoNamesMap[j + 3];
						ofLog(OF_LOG_NOTICE) << "Adding file: " << seqNam.first << " " << seqNam.second << " " << j + 3 << " " << path;
						videoApp->addSequence(path);
					}
					if (d.id == 2) { //displays 7-9
						std::string path = "video/" + seqName + "/" + seqName + "_" + videoNamesMap[j + 6];
						ofLog(OF_LOG_NOTICE) << "Adding file: " << seqNam.first << " " << seqNam.second << " " << j + 6 << " " << path;
						videoApp->addSequence(path);
					}
				}
				
				ofRunApp(d.videoWindow, videoApp);
				j++;
			}
		}
	}

    ofRunMainLoop();

	ofLog(OF_LOG_NOTICE) << "Finishing Windows Creating " << std::endl;
}
