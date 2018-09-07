//
//  MidiControl.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 25/07/18.
//
//

#ifndef MidiControl_hpp
#define MidiControl_hpp

#include <stdio.h>
#include "Clock.hpp"
#include "ofxMidi.h"
#include "Button.hpp"

class MidiControl:public ofxMidiListener
{
public:
    
    MidiControl();
    ~MidiControl();
    
    void update();//update first the midi notes and store them in an array
    void drawGUI();
    int &getNoteValue(int id);
    int &getControlValue(int id);
    float &getDt(){return _clk.getDeltaTime();}; // get the delta time
    bool &getBeat(){return _clk.getBeat();};
    float &getDtMulti(int multi){return _clk.getDeltaMulti(multi);}; // get the delta time
    
    void tapControler();
    void newTapEvent(string &name); //tap bpm
    void resetTapCount();
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    ofEvent<int>    noteON;
    ofEvent<int>    noteOFF;
    ofEvent<int>    controlChange;
    
    // Position Gui
    int position[2] = {20,50};
    int collumnSize[2] = {45,800};
    int Padding;
    
    

private:
    stringstream text;//should been removed

    ofxMidiIn       _midiTeensy; // this is now one controler,
    ofxMidiIn       _midiNanoKontrol2; // this is now one controler,
    //combining the nanokontrol and the ownbuil can be done, but we have to map the own ones cc to not used ones by the nanokontrol
    ofxMidiMessage  _midiMessage;

    
    int             _data[127];// every midi note is one array position
    int             _control[127];// control chenge values
    Clock           _clk;
    
    Button          _tapButton;
    
};

//pitch = note
//velocity = velocity of note

//control = control change(knob)
//value  = value
/*
 nanokontrol  uses only cc used control channel
 0-7   tm
 16-23 tm
 32-39 tm
 48-55 tm
 64-71 tm
 58 59 tm
 46
 60-62 tm
 41-45 tm
 */

#endif /* MidiControl_hpp */
