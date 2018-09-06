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

#define SEQUENCESTEPS 8

class segmentSelector{
    
public:
    
    segmentSelector();
    ~segmentSelector();
    
    struct Selection
    {
        string name;//the name for selection
        int beats [8] = { 0,0,0,0,0,0,0,0 };
        Button*  stepbuttons[SEQUENCESTEPS];
    };

    
    void setup(ofRectangle area);
    void draw();

private:
    int numSelections;
    int spacingVert;
    int fontSize;
    
    ofRectangle drawArea;
    vector<vector<Button> > sequenceButtons;
    vector<Selection> selections;
    ofTrueTypeFont	verdana14;
};

#endif /* segmentSelector_hpp */
