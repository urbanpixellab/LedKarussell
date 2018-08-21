//
//  LedAnimator.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 07/08/18.
//  this class holds all animation vor preview and live as two instances
//  the segments updating thei value here by function and step sequencer
//  and led mapping

#ifndef LedAnimator_hpp
#define LedAnimator_hpp

#include <stdio.h>
#include "ofMain.h"
#include "MidiControl.hpp"
#include "Colorizer.hpp"
#include "ofxXmlSettings.h"


class LedAnimator
{
public:
    struct Selection
    {
        string name;//the name for button
        vector<int> items;// the selected items
    };
    
    
    enum CURVE
    {
        BLACKOUT = 0,
        WHITEOUT,
        RAMP,
        TRI,
        SAW,
        RECT,
        SINE,
        CURVE_COUNT
    };
    
    
    LedAnimator(MidiControl *mc);
    ~LedAnimator();

    void drawToArray(int drawFunction,u_int8_t * selectionArrays,int &length,ofVec3f colorFunctionSelect);//dt is already existing
    // plus later 3 offset and frequency parameter
    
private:
    
    int                 _aniSelect;
    MidiControl         *_MC;
    Colorizer           _col;
    //niew selector
    vector<Selection>   _selections;//ok buttons for
};

#endif /* LedAnimator_hpp */
