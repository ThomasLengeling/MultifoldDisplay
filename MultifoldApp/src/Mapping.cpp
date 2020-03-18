#include "mapping.h"

namespace inn {

//-----------------------------------------------------------------------------------
void Mapping::setupWarp(int width, int height) {
    std::cout <<"Creating Warp: "<<std::endl;
    projectionWidth = width;
    projectionHeight = height;
    
    //   Load warp settings from file if one exists.
    this->warpController.loadSettings("settings_warp.json");
    
    for (auto i = 0; i < this->warpController.getNumWarps(); ++i) {
        auto warp = this->warpController.getWarp(i);
        warp->setSize(projectionWidth, projectionHeight);
    }
    
    std::cout <<"Warp Size: "<< projectionWidth << " " << projectionHeight << std::endl;
    
    if (this->warpController.getWarps().empty() ||  numDisplays !=  this->warpController.getNumWarps()) {
        // Otherwise create warps from scratch.
        for (int i = 0; i < numDisplays; i++) {
            shared_ptr<ofxWarpBase> warp;
            warp = this->warpController.buildWarp<ofxWarpPerspectiveBilinear>();
            warp->setSize(projectionWidth, projectionHeight);
            warp->setEdges(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            warp->setBrightness(1.0);
            warp->setGamma(1.0);
            warp->setExponent(2.0);
            warp->setLuminance(0.5);
            std::cout<<"Created warp "<<projectionWidth<<" "<<projectionWidth<<std::endl;
        }
    }
    
    this->useBeginEnd = false;
    
    ofAddListener(ofEvents().keyPressed, this, &Mapping::keyAction);
    
    // GUI
    for (int i = 0; i < numDisplays; i++) {
        ParamMapRef paramWarp = ParamMap::create();
        paramScreens.push_back(paramWarp);
    }
    
    //paramets
    for (int i = 0; i < numDisplays; i++) {
        parameters.setName("Mapping "+std::to_string(mId));
        parameters.add(paramWarp->gamma.set("gamma " + std::to_string(mId), 1, ofColor(0), ofColor(255)));
        parameters.add(paramWarp->luminance.set("luminance " + std::to_string(mId), 1, ofColor(0), ofColor(255)));
        parameters.add(paramWarp->brightness.set("brightness " + std::to_string(mId), 0.8877, 0.0, 1.0));
        parameters.add(paramWarp->exponent.set("exponent " + std::to_string(mId), 2.0, 0.0, 3.0));
        
        //Edges left, top, right, bottom
        parameters.add(paramWarp->edgeLeft.set("edge_left " + std::to_string(mId), 0.0, 0.0, 1.0));
        parameters.add(paramWarp->edgeTop.set("edge_top " + std::to_string(mId), 0.0, 0.0, 1.0));
        parameters.add(paramWarp->edgeRight.set("edge_right " + std::to_string(mId), 0.0, 0.0, 1.0));
        parameters.add(paramWarp->edgeBottom.set("edge_bottom " + std::to_string(mId), 0.0, 0.0, 1.0));
        
        
        paramWarp->gammaListener = paramWarp->gamma.newListener([this](ofColor &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(0);
            warp->setGamma(glm::vec3((float)v.r / 255.0, (float)v.g / 255.0, (float)v.b / 255.0));
        });
        
        paramWarp->exponentListener = paramWarp->exponent.newListener([this](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(0);
            warp->setExponent(v);
        });
        
        paramWarp->brightnessListener = paramWarp->brightness.newListener([this](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(0);
            warp->setBrightness(v);
            std::cout<< warp->getBrightness()<<std::endl;
        });
        
        paramWarp->luminanceListener = paramWarp->luminance.newListener([this](ofColor &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(0);
            warp->setLuminance(glm::vec3( (float)v.r/255.0, (float)v.g / 255.0, (float)v.b / 255.0));
        });
        
        //EDGES left, top, right, bottom
        paramWarp->edgeLeftListener = paramWarp->edgeLeft.newListener([this](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(0);
            glm::vec4 edges = warp->getEdges();
            warp->setEdges(v, edges.y, edges.z, edges.w);
        });
        
        paramWarp->edgeTopListener = paramWarp->edgeTop.newListener([this](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(0);
            glm::vec4 edges = warp->getEdges();
            warp->setEdges(edges.x, v, edges.z, edges.w);
        });
        
        paramWarp->edgeRightListener = paramWarp->edgeRight.newListener([this](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(0);
            glm::vec4 edges = warp->getEdges();
            warp->setEdges(edges.x, edges.y, v, edges.w);
        });
        
        paramWarp->edgeBottomListener = paramWarp->edgeBottom.newListener([this](float &v) {
            shared_ptr<ofxWarpBase> warp = this->warpController.getWarp(0);
            glm::vec4 edges = warp->getEdges();
            warp->setEdges(edges.x, edges.y, edges.z, v);
        });
    }
    
    
}

//--------------------------------------------------------------
void Mapping::draw(ofTexture &texture) {
    if (texture.isAllocated()) {
        auto warp = this->warpController.getWarp(0);
        warp->draw(texture);
    } else {
        std::cout << "not allocated" << std::endl;
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

} // namespace inn
