//
//  Button.hpp
//  TextureGenerator
//
//  Created by Enrico Becker on 30/08/16.
//
//

#ifndef Button_hpp
#define Button_hpp

#include <stdio.h>
# include "ofMain.h"

class Button
{
public:
    Button();
    Button(int id,ofRectangle area, string name, bool toogle);
    ~Button();
    
    void setup(ofRectangle area, string name, bool toogle);
    void setup(ofRectangle area, string name, bool toogle, ofVec2f offset);
    void draw();
    void drawRedPulsing(float &dt);
    
    void mousePressed(ofMouseEventArgs &arg);
    bool &getState(){return isPressed;};
    bool &getIstoggle(){return isToggle;};
    void setState(bool state){ isPressed = state; };

    void activateListener();
    void deactivateListener();
    void setColors(ofColor _nonActive, ofColor _active, ofColor text);
    
    ofEvent<string>   buttonPressed;
    ofEvent<int>      buttonIDPressed;
    void pressedControler();
    ofRectangle &getArea(){return drawArea;};
    
private:
    int             id;
    ofVec2f         mouseOffset;
    ofRectangle     drawArea;
    string          name;
    bool            isToggle;
    bool            isPressed;
    float           pressTimeout;
    
    ofColor nonActive;
    ofColor Active;
    ofColor textColor;

    
};

#endif /* Button_hpp */
