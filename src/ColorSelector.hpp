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
    // triggerd by events
    void setColorA(string &s);
    void setColorB(string &s);
    void setColorIDs(int ids[]);
    void colorSwap(string &s);
    ofColor &getColorFromID(int id){return _selectedColors[id];};
    // FIXME: would rather sent the color as color array or int arra but that is not possible?
    //ofEvent<string>   colorChosen;
    
    vector <ofColor> colors;
    ofEvent<bool> colorPressed;
    int *getSelectedColorIDs(){return _selectedColorIDs;};
    
private:
    ofRectangle drawArea;
    
    vector <Button> buttonsA;
    vector <Button> buttonsB;
    Button colorSwapButton;
    
    int tilesize;
    int numCollumns;
    int padding;
    
    //ofColor colorA;
    //ofColor colorB;
    ofColor         _selectedColors[2] = {ofColor(255),ofColor(255)};
    int             _selectedColorIDs[2] = {0,0};
    bool            _pressed;


};
#endif /* ColorSelector_hpp */
