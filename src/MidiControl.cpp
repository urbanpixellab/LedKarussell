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
    _tapButton_old = ofRectangle(0,0,50,50);
    
    _tapButton.setup(ofRectangle(0,0,50,50), "BPM", false);
    ofAddListener(_tapButton.buttonPressed, this, &MidiControl::newTapEvent);
    
    //link every controler to a differenet port
    _midiTeensy.listPorts(); // via instance
    //_midiControl.openPort(2);
    _midiTeensy.openPort("Teensy MIDI");
    _midiNanoKontrol2.openPort("nanoKONTROL2 SLIDER/KNOB");
    //midiIn.openPort("IAC Pure Data In");	// by name
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    _midiTeensy.ignoreTypes(false, false, false);
    _midiNanoKontrol2.ignoreTypes(false, false, false);
    
    // add ofApp as a listener
    _midiTeensy.addListener(this);
    _midiNanoKontrol2.addListener(this);
    
    // print received messages to the console
    //_midiTeensy.setVerbose(true);

}

MidiControl::~MidiControl()
{
    _midiTeensy.closePort();
    _midiTeensy.removeListener(this);
    _midiNanoKontrol2.closePort();
    _midiNanoKontrol2.removeListener(this);

    ofRemoveListener(_tapButton.buttonPressed, this, &MidiControl::newTapEvent);
}

void MidiControl::drawGUI()
{
    // Draws the BPM Button
    //tap button and bpm or step counter must been implemented
    ofSetColor(255*_clk.getDeltaTime(),0,0);
    //ofDrawRectangle(_tapButton);
    _tapButton.draw();
    ofDrawBitmapString(_clk.getBPM() + " BPM", 20,70);
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

int &MidiControl::getControlValue(int id)
{
    if(id < 0 || id >= 25)
        return _control[id];
}

void MidiControl::newTapEvent(string &s)
{
    _clk.tap();
}


void MidiControl::newMidiMessage(ofxMidiMessage& msg) {
    
    // make a copy of the latest message
    _midiMessage = msg;
    //convert this to note on and off and control change
    //cout << "pitch: " << _midiMessage.pitch << "velocity: " << _midiMessage.velocity << "control: " << _midiMessage.control  << "value: " << _midiMessage.value << endl;

    int type = (int)_midiMessage.bytes[0];
    cout << type << endl;
    if(type == 146) //note on
    {
        int noteID = (int)_midiMessage.bytes[1];
        _data[noteID] = 127;
        ofNotifyEvent(noteON, noteID);
        //if (noteID == 51) _clk.tap();
        cout << "NOTE " << noteID << " ON" << endl;
    }
    else if(type == 130) //note off
    {
        int noteID = (int)_midiMessage.bytes[1];
        _data[noteID] = 0;
        ofNotifyEvent(noteOFF, noteID);
        cout << "NOTE " << noteID << " OFF" << endl;
    }
    else if(type == 178 || type == 176)//control change 178 teensy and 176 nano
    {
        //control change
        int ccID = (int)_midiMessage.bytes[1];
        _control[ccID] = (int)_midiMessage.bytes[2];
        ofNotifyEvent(controlChange, ccID);
        cout << "Control Change " << ccID << ":" << _control[ccID] << endl;

    }

}
