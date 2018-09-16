//
//  LightControl.hpp
//  LedKarussell
//
//  Created by Machiel Veltkamp on 07/09/18.
//
//

#ifndef LightControl_hpp
#define LightControl_hpp


#include <stdio.h>
#include "ofMain.h"
#include "Button.hpp"
#include "ofxXmlSettings.h"
#include "ofxArtnetProtocol.h"


class LightControl{
    
    
    struct Node
    {
        //artnet node maybe as seperate class
        string                  ip;
        int                     nodeID;
        ofxArtnetProtocol       artnet;
        string                  name;
        u_int8_t                universe[512]; //8 full dm universes
    };
    
    
    
    struct Command
    {
        string name;
        int id;
        vector<int> channels;
        vector<int> values;
        Button* button;
    };
    
    
public:
    LightControl();
    ~LightControl();
    
    void loadCommands();
    void setup(ofRectangle area);
    void draw();
    void sendCommand(int &id);
    void sendTest();
    
private:
    vector<Command>     _commands;
    ofRectangle         drawArea;
    Node                node;
    int                 _buttonsize[2] = {80,25};
    int                 _padding;
    
    

};
 

#endif /* LightControl_hpp */
