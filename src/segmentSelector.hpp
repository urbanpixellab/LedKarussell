//
//  segmentSelector.hpp
//  LedKarussell
//
//  Created by Machiel Veltkamp on 02/09/18.
//
//

#ifndef segmentSelector_hpp
#define segmentSelector_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Button.hpp"
#include "Patroon.hpp"
#include "MidiControl.hpp"

#define SEQUENCESTEPS 8
#define NUMBERSELECTIONS 14

class segmentSelector{
    
public:
    
    segmentSelector();
    ~segmentSelector();
    
    struct Selection
    {
        string name;//the name for selection
        bool sequence [SEQUENCESTEPS] = { false,false,false,false,false,false,false,false };
        Button*  stepbuttons[SEQUENCESTEPS];
    };

    
    void setup(ofRectangle area);
    void draw();
    void mousePressed(ofMouseEventArgs &args);
    void setSequence(int i,bool sequence[]);
    bool *getSequence(int segmentID){ return selections[segmentID]->sequence; };
    ofEvent<bool> segmentPressed;
    void clearStepsOfSegments();
    

private:
    int numSelections;
    int spacingVert;
    int fontSize;
    
    ofRectangle drawArea;
    vector<Selection*> selections;
    ofTrueTypeFont	verdana14;
    bool            _pressed;
};

#endif /* segmentSelector_hpp */
