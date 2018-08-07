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


class LedAnimator
{
public:
    enum ANIMATIONS
    {
        BLACK = 0,
        WHITE,
        FADE_IN,
        FADE_OUT,
        SMOOTH_FADE,
        COUNT
    };
    
    LedAnimator(MidiControl *mc);
    ~LedAnimator();
    
    void setAnimation(int newAnimation){_aniSelect = newAnimation;};
    void drawGui();// d
    void animationToArray(int id,u_int8_t * array,int length,int colorSelect);
    void addStep();
    
private:
    void blackout(int &id,u_int8_t * array,int &length);
    void whiteout(int &id,u_int8_t * array,int &length);
    void fade(int &id,u_int8_t * array,int &length,bool direction);
    void smoothFade(int &id,u_int8_t * array,int &length,bool direction);
    
    int         _aniSelect;
    MidiControl*_MC;
    int         _step;
    int         _maxStep;
    Colorizer   _col;
};

#endif /* LedAnimator_hpp */
