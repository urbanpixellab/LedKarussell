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
    _tapButton = ofRectangle(0,0,50,50);
    ofAddListener(ofEvents().mousePressed, this, &MidiControl::mousePressed);
}

MidiControl::~MidiControl()
{
    ofRemoveListener(ofEvents().mousePressed, this, &MidiControl::mousePressed);
}

void MidiControl::drawGUI()
{
    //tap button and bpm or step counter must been implemented
    ofSetColor(255);
    ofDrawRectangle(_tapButton);
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

void MidiControl::mousePressed(ofMouseEventArgs &args)
{
    if (_tapButton.inside(args.x,args.y))
    {
        _clk.tap();
        cout << "tap" << endl;
    }
}
