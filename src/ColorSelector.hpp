//
//  ColorSelector.hpp
//  LedKarussell
//
//  Created by Machiel Veltkamp on 02/09/18.
//
//

#ifndef ColorSelector_hpp
#define ColorSelector_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Button.hpp"

class ColorSelector{

public:

    ColorSelector();
    ~ColorSelector();
    
    void setup(ofRectangle area);
    void loadColors();
    void draw();
    void setColorA(string &s);
    void setColorB(string &s);
    void colorSwap(string &s);

private:
    ofRectangle drawArea;
    vector <ofColor> colors;
    vector <Button> buttonsA;
    vector <Button> buttonsB;
    Button colorSwapButton;
    
    int tilesize;
    int numCollumns;
    int padding;
    
    ofColor colorA;
    ofColor colorB;


};
#endif /* ColorSelector_hpp */
