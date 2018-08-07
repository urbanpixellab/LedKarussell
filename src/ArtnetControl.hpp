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
#include "ofxXmlSettings.h"
#include "Segment.hpp"
#include "LedAnimator.hpp"
#include "AnimatorGUI.hpp"

class ArtnetControl
{
public:
    
    struct Node
    {
        //artnet node maybe as seperate class
        string                  ip;
        int                     nodeID;
        ofxArtnetProtocol       artnet;
        //artnet.begin("192.168.12.200");
        u_int8_t                universes[8][512]; //8 full dm universes
        //u_int8_t                _universes[8][450]; //3*150 led per stripe max
    };
    

    
    ArtnetControl(MidiControl *mc);
    ~ArtnetControl();
    
    void setAnimation(int id);
    
    void update();
    void drawGui();
    void specialFunction(int id);
    void guiPressed(int &buttonid);
    
    void loadNodes(); //load the artnet controler nodes from an xml
    
    Node        *getNode(int id){return _nodes[id];};
    Segment     *getPreSegment(int id){return _preSegments[id];};
    Segment     *getLiveSegment(int id){return _liveSegments[id];};
    
private:
    void sendToNodes();
    void clearNodes();
    
    MidiControl             *_MC;
    vector<Node*>           _nodes;
    vector<Segment*>        _preSegments;
    vector<Segment*>        _liveSegments;
    LedAnimator             *_preAnimator;
    LedAnimator             *_LiveAnimator;
    
    AnimatorGUI             *_GUI;
};

#endif /* ArtnetControl_hpp */
