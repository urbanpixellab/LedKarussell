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

    _midiIn.listPorts(); // via instance
    //ofxMidiIn::listPorts(); // via static as well
    
    // open port by number (you may need to change this)
    _midiIn.openPort(2);
    //midiIn.openPort("IAC Pure Data In");	// by name
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
    
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    _midiIn.ignoreTypes(false, false, false);
    
    // add ofApp as a listener
    _midiIn.addListener(this);
    
    // print received messages to the console
    _midiIn.setVerbose(true);

}

MidiControl::~MidiControl()
{
    _midiIn.closePort();
    _midiIn.removeListener(this);

    ofRemoveListener(ofEvents().mousePressed, this, &MidiControl::mousePressed);
}

void MidiControl::drawGUI()
{
    //tap button and bpm or step counter must been implemented
    ofSetColor(255*_clk.getDeltaTime(),0,0);
    ofDrawRectangle(_tapButton);
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

void MidiControl::mousePressed(ofMouseEventArgs &args)
{
    if (_tapButton.inside(args.x,args.y))
    {
        _clk.tap();
    }
}

void MidiControl::newMidiMessage(ofxMidiMessage& msg) {
    
    // make a copy of the latest message
    _midiMessage = msg;
    //convert this to note on and off and control change
    //cout << "pitch: " << _midiMessage.pitch << "velocity: " << _midiMessage.velocity << "control: " << _midiMessage.control  << "value: " << _midiMessage.value << endl;

    int type = (int)_midiMessage.bytes[0];
    //cout << "status "<< ofxMidiMessage::getStatusString(_midiMessage.status) << endl;
    
    if(type == 146) //note on
    {
        int noteID = (int)_midiMessage.bytes[1];
        _data[noteID] = 127;
        ofNotifyEvent(noteON, noteID);
        //directly mapped to clock
        if (noteID == 51) _clk.tap();
    }
    else if(type == 130) //note off
    {
        int noteID = (int)_midiMessage.bytes[1];
        _data[noteID] = 0;
        ofNotifyEvent(noteOFF, noteID);
    }
    else if(type == 178)
    {
        //control change
        int ccID = (int)_midiMessage.bytes[1];
        _control[ccID] = (int)_midiMessage.bytes[2];
        ofNotifyEvent(controlChange, ccID);
    }

}