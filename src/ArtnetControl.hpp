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
    

    void update();
    void drawGui();
    void specialFunction(int id);
    void guiAnimationPressed(int &buttonid);
    void guiEnablePressed(int &buttonid);

    void loadNodes(); //load the artnet controler nodes from an xml
    
    Node        *getNode(int id){return _nodes[id];};
    Segment     *getPreSegment(int id){return _preSegments[id];};
    Segment     *getLiveSegment(int id){return _liveSegments[id];};
    
private:
    void setAnimationLive(int id){_liveAnimator->setAnimation(id);};
    void setAnimationPreview(int id){_preAnimator->setAnimation(id);};

    void setEnableLive(int id){_liveAnimator->setEnableMode(id);};
    void setEnablePreview(int id){_preAnimator->setEnableMode(id);};

    void sendToNodes();
    void clearNodes();
    
    void writeSegmentsToImage(vector<Segment*> seg, ofImage& img);
    
    MidiControl             *_MC;
    vector<Node*>           _nodes;
    vector<Segment*>        _preSegments;
    vector<Segment*>        _liveSegments;
    LedAnimator             *_preAnimator;
    LedAnimator             *_liveAnimator;
    
    AnimatorGUI             *_GUI;
    
    ofImage                 _preIMG;
    ofImage                 _liveIMG;
};

#endif /* ArtnetControl_hpp */
