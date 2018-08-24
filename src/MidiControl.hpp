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
    
    void mousePressed(ofMouseEventArgs &args);

    void tapControler();
    void newTapEvent(string &name); //tap bpm
    void resetTapCount();
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    ofEvent<int>    noteON;
    ofEvent<int>    noteOFF;
    ofEvent<int>    controlChange;
    

private:
    stringstream text;//should been removed

    ofxMidiIn       _midiIn;
    ofxMidiMessage  _midiMessage;

    
    int             _data[127];// every midi note is one array position
    int             _control[25];// control chenge values
    Clock           _clk;
    
    ofRectangle     _tapButton;
    
};

//pitch = note
//velocity = velocity of note

//control = control change(knob)
//value  = value
#endif /* MidiControl_hpp */
