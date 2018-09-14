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
    void setColorFromIDs(int colorIDa,int colorIDb);
    void setColorA(string &s);
    void setColorB(string &s);
    void setColorIDs(int *ids);
    void colorSwap(string &s);
//    ofColor &getColorFromID(int id){return _selectedColors[id];};
    ofColor &getColorFromID(int id){return colors[id];};

    // FIXME: would rather sent the color as color array or int arra but that is not possible?
    //ofEvent<string>   colorChosen;
    
    void mousePressed(ofMouseEventArgs &args);
    
    vector <ofColor> colors;
    ofEvent<bool> colorPressed;
    
    int *getSelectedColorIDs(){return _selectedColorIDs;};
    void addListeners();
    void removeListeners();
    
    Button &getColorButtonA(int id){return buttonsA[id];}
    Button &getColorButtonB(int id){return buttonsA[id];}
    
private:
    ofRectangle drawArea;
    
    vector <Button> buttonsA;
    vector <Button> buttonsB;
    Button colorSwapButton;
    
    int tilesize;
    int numCollumns;
    int padding;
    
    ofColor         _selectedColors[2] = {ofColor(255),ofColor(255)};
    int             _selectedColorIDs[2] = {0,0};
    bool            _pressed;


};
#endif /* ColorSelector_hpp */
