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
        ANIMATION_COUNT
    };
    
    enum ENABLES
    {
        ALL_OFF = 0,
        ALL_ON,
        RANDOM,
        KNIT_RIDER,
        ENABLE_COUNT
    };
    
    LedAnimator(MidiControl *mc);
    ~LedAnimator();
    void setSegmentSize(int size); // used for enable disable petterns
    void setAnimation(int newAnimation){_aniSelect = newAnimation;};
    void setEnableMode(int mode);
    void updateEnable();
    void drawGui();// d
    void animationToArray(int id,u_int8_t * array,int length,int colorSelect);
    void addStep();
    
private:
    void blackout(int &id,u_int8_t * array,int &length);
    void whiteout(int &id,u_int8_t * array,int &length);
    void fade(int &id,u_int8_t * array,int &length,bool direction);
    void smoothFade(int &id,u_int8_t * array,int &length,bool direction);
    
    void enableAll();
    void disableAll();
    void enableRandom();
    void knitRider();

    int                 _aniSelect;
    MidiControl         *_MC;
    int                 _step;
    int                 _maxStep;
    Colorizer           _col;
    vector<bool>        _enables;
    int                 _enableMode;
};

#endif /* LedAnimator_hpp */
