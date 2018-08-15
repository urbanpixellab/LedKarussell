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
    ~Button();
    
    void setup(ofRectangle area, string name, bool toogle);
    void setup(ofRectangle area, string name, bool toogle, ofVec2f offset);
    void draw();
    
    void mousePressed(ofMouseEventArgs &arg);
    bool &getState(){return isPressed;};

    void activateListener();
    void deactivateListener();
    
    ofEvent<string>   buttonPressed;
    void pressedControler();
    ofRectangle &getArea(){return drawArea;};
    
    ofColor nonActive;
    ofColor Active;
    ofColor textColor;

private:
    void            updateFbo(bool isPressed);
    
    ofVec2f         mouseOffset;
    ofFbo           fbo;
    ofRectangle     drawArea;
    string          name;
    bool            isToggle;
    bool            isPressed;
    float           pressTimeout;
    
};

#endif /* Button_hpp */
