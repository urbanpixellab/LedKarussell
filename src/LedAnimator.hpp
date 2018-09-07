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
#include "ofxXmlSettings.h"


class LedAnimator
{
public:
    
    
    enum CURVE
    {
        BLACKOUT = 0,
        WHITEOUT,
        RAMP,
        SAW,
        TRI,
        RECT,
        SINE,
        CURVE_COUNT
    };
    
    enum SEGMENT_DRAW
    {
        FORWARD = 0,    //draw from left to right
        REVERSE,        //draw from rigth to left
        OUTSIDE,        // draw from the ouside to the mid
        MID,            // draw from the mid to the outside
        SEGMENT_DRAW_COUNT
    };
    
    
    LedAnimator(MidiControl *mc);
    ~LedAnimator();

    void drawToArray(int drawFunction,int dir,int time,float freq,u_int8_t * selectionArrays,int &length,ofColor &a,ofColor &b);//dt is already existing
    void addToArray(int drawFunction,int dir,int time,float freq,u_int8_t * selectionArrays,int &length,ofColor &a,ofColor &b);//dt is already existing
    // plus later 3 offset and frequency parameter
//    int getSelectionMax(){return _selections.size();};
    
private:
    
    void reverseArray(float *array, int length);
    void BinnenBuitenArray(float *array, int length);
    void BuitenBinnenArray(float *array, int length);
    
    
    int                 _aniSelect;
    MidiControl         *_MC;
    //niew selector
};

#endif /* LedAnimator_hpp */
