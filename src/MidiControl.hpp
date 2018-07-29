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

class MidiControl
{
public:
    MidiControl();
    ~MidiControl();
    
    void update();//update first the midi notes and store them in an array
    int &getNoteValue(int id);
    float &getDt(){return _clk.getDeltaTime();}; // get the delta time
    bool &getBeat(){return _clk.getBeat();};

private:
    int         _data[127];// every midi note is one array position
    Clock       _clk;
    
    
};
#endif /* MidiControl_hpp */
