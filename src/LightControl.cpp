//
//  LightControl.cpp
//  LedKarussell
//
//  Created by Machiel Veltkamp on 07/09/18.
//
//

#include "LightControl.hpp"



LightControl::LightControl(){
    
    _padding = 10;
    
}

LightControl::~LightControl(){}


void LightControl::setup(ofRectangle area){
    
    // set drawing area
    drawArea = area;
    
    // setup buttons
    loadCommands();
    
    // setup the node
    node.ip = "10.0.0.160";
    node.name = "LightDesk";
    node.nodeID = 0;
    node.artnet.begin(node.ip.c_str());
    //fill universes black
    for (int l = 0; l < 512; l++)
    {
        node.universe[l] = 0;
    }
}

void LightControl::loadCommands(){
    
   
    ofxXmlSettings sel("lightingCommands.xml");
    for (int i = 0; i < sel.getNumTags("command"); i++)
    {
        sel.pushTag("command",i);
        string name = sel.getValue("label", "BL");
        bool toggle = ofToBool(sel.getValue("toggle", "false"));

        sel.pushTag("channels");
        vector<int> channels;
        for (int c = 0; c < sel.getNumTags("channel"); c++)
        {
            channels.push_back(sel.getValue("channel", 0));
        }
        sel.popTag();
        
        sel.pushTag("values");
        vector<int> values;
        for (int c = 0; c < sel.getNumTags("value"); c++)
        {
            values.push_back(sel.getValue("value", 0));
        }
        sel.popTag();
        
        Command c;
        c.name = name;
        c.channels = channels;
        c.values = values;
        
        Button *b = new Button(i,ofRectangle(drawArea.x+i*(_buttonsize[0]+_padding),drawArea.y,_buttonsize[0],_buttonsize[1]),name,toggle);
        ofAddListener(b->buttonIDPressed, this, &LightControl::sendCommand);
        c.button = b;
        
        sel.popTag();
        _commands.push_back(c);
    }

    
}


void LightControl::draw(){
    // Draw the buttons
    for ( auto c : _commands ) {
        c.button->draw();
    }
}

void LightControl::sendCommand(int &id)
{
    bool buttonState =      _commands[id].button->getState();
    bool buttonIsToggle =   _commands[id].button->getIstoggle();
    cout << "send command to lightingdesk: "<< _commands[id].name << " id: " << id << endl;
    
    // Loop through DMX channels
    for (int l = 0; l < 512; l++)
    {
        // if nothing happens send 0
        node.universe[l] = 0;
        
        // loop through the cahneels for the command
        for(int i=0; i < _commands[id].channels.size(); i++)
        {
            // We found a channel of the commad
            if(_commands[id].channels[i] == l)
            {
                // if the command button is a toggle and it is toggled of sent 0
                //cout << "Turn on channel: " << _commands[id].channels[i] << " value: " << _commands[id].values[i] << endl;
                if(buttonIsToggle == true && buttonState == false)
                {
                    node.universe[l] = 0;
                }
                // set the value to the channel
                else
                {
                    node.universe[l] = _commands[id].values[l];
                }
            }
        }
    }
    node.artnet.send(node.universe,1,512);
}

 

void LightControl::sendTest()
{
    /*
    u_int8_t testuniverse[512]; //8 full dm universes
    for (int i = 0; i < 512; i++)
    {
        testuniverse[i] = 255;
    }
    node.artnet.send(testuniverse,1,512);
    cout << "send artnet " << endl;*/
}