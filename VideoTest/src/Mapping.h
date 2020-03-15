#pragma once

/*
Thomas Sanchez Lengeling Mapping
MIT 2020
*/

#include "ofMain.h"
#include "ofxWarp.h"

#include "ofParameterGroup.h"
#include "ofParameter.h"

namespace inn {

    class Mapping;
    class ParamMap;

    typedef std::shared_ptr<Mapping> MappingRef;
    typedef std::shared_ptr<ParamMap> ParamMapRef;

    class Mapping {
    public:
        Mapping() {}

        //create memory
        static MappingRef create() {return std::make_shared<Mapping>();}

        //setup the warp area
        void setupWarp(int width, int height);

        //draw the warw
        void draw(ofTexture & texture);

        //key events
        void keyAction(ofKeyEventArgs& eventArgs);

        //save configuration of the warp
        void saveConfiguration();

        shared_ptr<ofxWarpBase> getWarp() {return this->warpController.getWarp(0);}

        //seperate texture in to number of screens.
        ofTexture getVideo();

        ///gui
        ofParameterGroup parameters;
        ofParameter<float> brightness;

    private:

        bool useBeginEnd;
        ofxWarpController warpController;
        vector<ofRectangle> srcAreas;
        
        int areaMode;
        string areaName;

        int mPixelBlends;

        int projectionWidth;
        int projectionHeight;

        ParamMapRef paramWarp;
        
        int mId;
    };

//--------------------------------------------------------------
    class ParamMap {
    public:
        ParamMap() {}
        static ParamMapRef create() {
            return std::make_shared<ParamMap>();
        }

        ofParameter<ofColor> gamma;
        ofParameter<ofColor> luminance;
        ofParameter<float>   brightness;
        ofParameter<float>     exponent;

        ofParameter<float> edgeLeft;
        ofParameter<float> edgeRight;
        ofParameter<float> edgeTop;
        ofParameter<float> edgeBottom;

        ofEventListener  gammaListener;
        ofEventListener  luminanceListener;
        ofEventListener  brightnessListener;
        ofEventListener     exponentListener;

        ofEventListener  edgeLeftListener;
        ofEventListener  edgeRightListener;
        ofEventListener  edgeTopListener;
        ofEventListener  edgeBottomListener;
    };

}
