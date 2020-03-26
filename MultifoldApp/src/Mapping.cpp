#include "mapping.h"

namespace inn {

//-----------------------------------------------------------------------------------
void Mapping::setupWarp(int width, int height) {
    ofLog(OF_LOG_NOTICE) <<"Creating Warp: ";
    projectionWidth = width;
    projectionHeight = height;
    
    //   Load warp settings from file if one exists.
    this->warpController.loadSettings("warp.json");
    
    for (auto i = 0; i < this->warpController.getNumWarps(); ++i) {
        auto warp = this->warpController.getWarp(i);
        warp->setSize(projectionWidth, projectionHeight);
    }
    
    ofLog(OF_LOG_NOTICE)<<"Warp Size: "<< projectionWidth << " " << projectionHeight;
    
    if (this->warpController.getWarps().empty() ||  mNumDisplays !=  this->warpController.getNumWarps()) {
        // Otherwise create warps from scratch.
        ofLog(OF_LOG_NOTICE)<<"JSON Empty Displays: "<<mNumDisplays;
        
        for (int i = 0; i < mNumDisplays; i++) {
            shared_ptr<ofxWarpBase> warp;
            warp = this->warpController.buildWarp<ofxWarpPerspectiveBilinear>();
            warp->setSize(projectionWidth, projectionHeight);
            warp->setEdges(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            warp->setBrightness(1.0);
            warp->setGamma(1.0);
            warp->setExponent(2.0);
            warp->setLuminance(0.5);
            //this->warpController.addWarp(warp);
            ofLog(OF_LOG_NOTICE)<<"Created warp "<<projectionWidth<<" "<<projectionWidth;
        }
    }
    ofLog(OF_LOG_NOTICE)<<"Num Waps: "<<this->warpController.getNumWarps();
    
    this->useBeginEnd = false;
    
    ofAddListener(ofEvents().keyPressed, this, &Mapping::keyAction);
    
    // GUI
    for (int i = 0; i < mNumDisplays; i++) {
        ParamMapRef paramWarp = ParamMap::create();
        paramScreens.push_back(paramWarp);
    }
    
    //paramets
    for (int i = 0; i < mNumDisplays; i++) {
        parameters.setName("Mapping "+std::to_string(i));
        parameters.add(paramScreens.at(i)->gamma.set("gamma " + std::to_string(i), 1, ofColor(0), ofColor(255)));
        parameters.add(paramScreens.at(i)->luminance.set("luminance " + std::to_string(i), 1, ofColor(0), ofColor(255)));
        parameters.add(paramScreens.at(i)->brightness.set("brightness " + std::to_string(i), 0.8877, 0.0, 1.0));
        parameters.add(paramScreens.at(i)->exponent.set("exponent " + std::to_string(i), 2.0, 0.0, 3.0));
        
        //Edges left, top, right, bottom
        parameters.add(paramScreens.at(i)->edgeLeft.set("edge_left " + std::to_string(i), 0.0, 0.0, 1.0));
        parameters.add(paramScreens.at(i)->edgeTop.set("edge_top " + std::to_string(i), 0.0, 0.0, 1.0));
        parameters.add(paramScreens.at(i)->edgeRight.set("edge_right " + std::to_string(i), 0.0, 0.0, 1.0));
        parameters.add(paramScreens.at(i)->edgeBottom.set("edge_bottom " + std::to_string(i), 0.0, 0.0, 1.0));
        
        //listeners
        paramScreens.at(i)->gammaListener = paramScreens.at(i)->gamma.newListener([this, i](ofColor &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(i);
            warp->setGamma(glm::vec3((float)v.r / 255.0, (float)v.g / 255.0, (float)v.b / 255.0));
        });
        
        paramScreens.at(i)->exponentListener = paramScreens.at(i)->exponent.newListener([this, i](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(i);
            warp->setExponent(v);
        });
        
        paramScreens.at(i)->brightnessListener = paramScreens.at(i)->brightness.newListener([this, i](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(i);
            warp->setBrightness(v);
            std::cout<< warp->getBrightness()<<std::endl;
        });
        
        paramScreens.at(i)->luminanceListener = paramScreens.at(i)->luminance.newListener([this, i](ofColor &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(i);
            warp->setLuminance(glm::vec3( (float)v.r/255.0, (float)v.g / 255.0, (float)v.b / 255.0));
        });
        
        //EDGES left, top, right, bottom
       paramScreens.at(i)->edgeLeftListener = paramScreens.at(i)->edgeLeft.newListener([this, i](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(i);
            glm::vec4 edges = warp->getEdges();
            warp->setEdges(v, edges.y, edges.z, edges.w);
        });
        
        paramScreens.at(i)->edgeTopListener = paramScreens.at(i)->edgeTop.newListener([this, i](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(i);
            glm::vec4 edges = warp->getEdges();
            warp->setEdges(edges.x, v, edges.z, edges.w);
        });
        
        paramScreens.at(i)->edgeRightListener = paramScreens.at(i)->edgeRight.newListener([this, i](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(i);
            glm::vec4 edges = warp->getEdges();
            warp->setEdges(edges.x, edges.y, v, edges.w);
        });
        
        paramScreens.at(i)->edgeBottomListener = paramScreens.at(i)->edgeBottom.newListener([this, i](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(i);
            glm::vec4 edges = warp->getEdges();
            warp->setEdges(edges.x, edges.y, edges.z, v);
        });
    }
    
    
}

//--------------------------------------------------------------
void Mapping::draw(ofTexture &texture) {
    if (texture.isAllocated()) {
        for (auto i = 0; i < this->warpController.getNumWarps(); ++i) {
            auto warp = this->warpController.getWarp(i);
            warp->draw(texture);
        }
    } else {
       ofLog(OF_LOG_NOTICE) << "not allocated";
    }
}

//--------------------------------------------------------------
void Mapping::draw(ofTexture & texture, int i){
    if (texture.isAllocated()) {
        auto warp = this->warpController.getWarp(i);
        warp->draw(texture);
    } else {
        ofLog(OF_LOG_NOTICE) << "not allocated tex " <<i;
    }
}

//--------------------------------------------------------------
void Mapping::saveConfiguration() {
    this->warpController.saveSettings("settings_warp.json");
}

//--------------------------------------------------------------
void Mapping::keyAction(ofKeyEventArgs &eventArgs) {
    if (eventArgs.key == 'a') {
        ofLog(OF_LOG_NOTICE) << " Key Warp ";
    }
}

void Mapping::exitEvent(){
    this->warpController.saveSettings("warp.json");
}


} // namespace inn

