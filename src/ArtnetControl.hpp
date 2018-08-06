//
//  ArtnetControl.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//
//

#ifndef ArtnetControl_hpp
#define ArtnetControl_hpp

#include <stdio.h>
//#include "ofxArtnetProtocol.h"
#include "SceneControl.hpp"
#include "MidiControl.hpp"
#include "ofxXmlSettings.h"
#include "Segment.hpp"

class ArtnetControl
{
public:
    /*
    struct Node
    {
        //artnet node maybe as seperate class
        string                  _ip;
        ofxArtnetProtocol       _artnet;
        //artnet.begin("192.168.12.200");
        u_int8_t                _universes[8][512]; //8 full dm universes
        //u_int8_t                _universes[8][450]; //3*150 led per stripe max
    };
     */
    
    ArtnetControl(MidiControl *mc,SceneControl *live);
    ~ArtnetControl();
    
    void update();
    void sendToNodes();
    
    void specialFunction(int id);
    
    void loadNodes(); //load the artnet controler nodes from an xml
    
    
private:
    void clearNodes();
    
    MidiControl             *_MC;
    SceneControl            *_Scene; // the actual output scene
    vector<Segment::Node*>  _nodes;
    vector<Segment*>        _segments;
};

#endif /* ArtnetControl_hpp */
