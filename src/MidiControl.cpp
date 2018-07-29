//
//  MidiControl.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 25/07/18.
//
//

#include "MidiControl.hpp"

MidiControl::MidiControl()
{
    for (int i = 0; i < 127; i++)
    {
        _data[i] = 0;
    }
}

MidiControl::~MidiControl()
{
    
}

void MidiControl::update()
{
    //first the midi notes and store them in an array
    // then update clock
    _clk.update();

}

int &MidiControl::getNoteValue(int id)
{
    if(id < 0 || id >= 127)
    return _data[id];
}