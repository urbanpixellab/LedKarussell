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
    void doLedAnimation(Patroon * pattern,LedAnimator * animator,vector<Segment*> segments,int &step);
    void drawGui();
    void fillWithBackgroundColor(ofColor & color,vector<Segment*> segments);
    void specialFunction(int id);
    //more listeners
    void PlayPatronPressed(int & id);
    void EditPatronPressed(int & id);
    void PlaySteppedPatronPressed(int & id);
    
    void loadNodes(); //load the artnet controler nodes from an xml
    //add key listener for test commands
    void keyPressed(ofKeyEventArgs &key);
    
    Node        *getNode(int id){return _nodes[id];};
    Segment     *getPreSegment(int id){return _preSegments[id];};
    Segment     *getLiveSegment(int id){return _liveSegments[id];};
    
    void curveAPressed(int & id){_editPatroon->setCurve(0, id);};
    void curveBPressed(int & id){_editPatroon->setCurve(1, id);};
    
    void colorPressed(bool &pressed);
    void segmentSelectPressed(bool &pressed);
    void sliderChanged(bool & value);
    int getNumStepsSequence(){ return _numStepsInSequnce; }
    
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
    int                     _curveLive;
    
    vector<Selection>       _selections;//ok buttons for
    vector<Patroon>         _patronen;
    Patroon                 *_editPatroon;//this is directly writing to the patroon
    Patroon                 *_livePatroon;//this is directly writing to the patroon
    
    int                     _step;
    int                     _numStepsInSequnce;
    float                   _freqences[16] = {0,125.25,.5,.75,1,2,3,4,5,6,7,8,12,16,20,24}; //length goes to gui
};

#endif /* ArtnetControl_hpp */
