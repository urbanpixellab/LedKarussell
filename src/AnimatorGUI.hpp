//
//  AnimatorGUI.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 07/08/18.
//
//

#ifndef AnimatorGUI_hpp
#define AnimatorGUI_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ColorSelector.hpp"
#include "Button.hpp"
#include "RotarySlider.hpp"
#include "segmentSelector.hpp"
#include "LightControl.hpp"
#include "MidiControl.hpp"


#define SLIDERS 6

//#define COLLUMNEDIT1 0
//#define COLLUMNEDIT2 300
#define COLLUMNLIVE 700

class AnimatorGUI
{
public:
    
    AnimatorGUI(ofRectangle area, MidiControl *mc);
    ~AnimatorGUI();
    
    void init();
    void createAnimationGUI(int animationCount);
    void update();
    void draw(ofImage &pre, ofImage &live);
    
    ofMesh &getMesh(){return _realStructure;}; //here we can work on the mesh
    
    void mousePressed(ofMouseEventArgs &args);
    ofEvent<int>    curveAPressed;
    ofEvent<int>    curveBPressed;
    ofEvent<int>    patronPLAY;
    ofEvent<int>    patronEDIT;
    ofEvent<int>    patronPLAYSTEPPED;
    ofEvent<bool>   colorSelectPressed;
    
    ofEvent<int>    postFxPressed;
    
//    void setColor(string &c);
    
    ColorSelector colorselectorA;
    ColorSelector colorselectorB;
    ColorSelector colorselectorLive;

    ColorSelector &getColorselectorA(){return colorselectorA;};
    ColorSelector &getColorselectorB(){return colorselectorB;};
    ColorSelector &getColorselectorLive(){return colorselectorLive;};

    float getSlidersAMapped(int id){return _slidersA[id]->getValueMapped();};
    float getSlidersBMapped(int id){return _slidersB[id]->getValueMapped();};

    RotarySlider *getCurveSlidersA(){return _slidersA[0];};
    RotarySlider *getCurveSlidersB(){return _slidersB[0];};
    RotarySlider *getTimeSliderA(){return _slidersA[1];}
    RotarySlider *getTimeSliderB(){return _slidersB[1];}
    RotarySlider *getFreqSliderA(){return _slidersA[2];}
    RotarySlider *getFreqSliderB(){return _slidersB[2];}
    RotarySlider *getDirSliderA(){return _slidersA[3];}
    RotarySlider *getDirSliderB(){return _slidersB[3];}
    RotarySlider *getPhaseCurveSliderA(){return _slidersA[4];}
    RotarySlider *getPhaseCurveSliderB(){return _slidersB[4];}
    RotarySlider *getPhaseFreqSliderA(){return _slidersA[5];}
    RotarySlider *getPhaseFreqSliderB(){return _slidersB[5];}
    
    segmentSelector segmenselectorA;
    segmentSelector segmenselectorB;
    
    segmentSelector &getSegmenselectorA(){return segmenselectorA;};
    segmentSelector &getSegmenselectorB(){return segmenselectorB;};
    
    LightControl lightcontrol;
    
    void editButtonPressed(int &id);
    void liveButtonPressed(int &id);
    
    // needed for set initial state of buttons
    void setEditButtonState(int &id, bool state);
    void setLiveButtonState(int &id, bool state);
    
    //Positioning GUI
    int CollumnEdit1;
    int CollumnEdit2;
    int Collumnwidth;
    int Padding;
    int RowsEdit[5] = {50,200,280,380,520};
    int RowsLive[5] = {50,130,340,620,740};
    
    Button *getPostEffectButton(int id){return _postEffectsButtons[id];};
    RotarySlider &getMasterBrightnessKnob(){return _masterBrightness;};
    Button &getAutoButton(){return _autoPattern;};
    
    
    //midi
    void NoteOn(int &id);
    void NoteOff(int &id);
    
private:

    ofRectangle         _drawArea;
    ofMesh              _realStructure;// the mesh to paint on
    vector<Button*>     _patLiveButtons;//the buttons to select a patron for playing or edit selectie
    vector<Button*>     _patEditButtons;//the buttons to select a patron for playing or edit selectie
    
    int                 _isPlay;
    int                 _isEdit;
    int                 _isPlayStepped;
    ofTrueTypeFont      _verdana;
    
    RotarySlider*   _slidersA[SLIDERS];
    RotarySlider*   _slidersB[SLIDERS];
    
    vector<Button*> _postEffectsButtons;
    
    RotarySlider    _masterBrightness;
    
    MidiControl     *_MC;
    Button          _autoPattern;
    int             _autoCounter;
    int             _maxSteps;
    bool            _oldAutoState;
    
    int             _MidiSequenzerButtons[16] = {36,37,42,53,59,38,50,52,44,46,47,48,45,57,56,43};//36,37,42,53,59,38,50,52,44,46,47,48,45,57,56,43
    int             _MidiSelector[6] = {39,41,58,55,40,54};//left to right
    int             _midiSelect;

    
};

#endif /* AnimatorGUI_hpp */
