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
#include "Patroon.hpp"

class ArtnetControl
{
public:
    struct Selection
    {
        string name;//the name for button
        vector<int> items;// the selected items
    };

    
    
    struct Node
    {
        //artnet node maybe as seperate class
        string                  ip;
        int                     nodeID;
        ofxArtnetProtocol       artnet;
        string                  name;
        //artnet.begin("192.168.12.200");
        u_int8_t                universes[8][512]; //8 full dm universes
        //u_int8_t                _universes[8][450]; //3*150 led per stripe max
    };
    
    ArtnetControl(MidiControl *mc);
    ~ArtnetControl();
    
    void loadPatroon();
    void savePatroon();
    
    void update();
    void drawGui();
    void fillAllBackgroundColor(ofColor & color);
    void specialFunction(int id);
    void guiCurvePressed(int &buttonid);

    void loadNodes(); //load the artnet controler nodes from an xml
    //add key listener for test commands
    void keyPressed(ofKeyEventArgs &key);
    
    Node        *getNode(int id){return _nodes[id];};
    Segment     *getPreSegment(int id){return _preSegments[id];};
    Segment     *getLiveSegment(int id){return _liveSegments[id];};
    
private:

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
    
    int                     _test;
    //niew
    int                     _curvePreview;
    int                     _curveLive;
    
    vector<Selection>       _selections;//ok buttons for
    vector<Patroon>         _patronen;
    Patroon                 *_editPatroon;//this is directly writing to the output
};

#endif /* ArtnetControl_hpp */
