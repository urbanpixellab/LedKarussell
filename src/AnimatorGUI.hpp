//
//  AnimatorGUI.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 07/08/18.
//
//

#ifndef AnimatorGUI_hpp
#define AnimatorGUI_hpp

#include <stdio.h>
#include "ofMain.h"

class AnimatorGUI
{
public:
    struct Button
    {
        int id;
        string name;
        ofRectangle area;
        ofColor color;
        bool toggle;
        bool isPressed;
    };
    
    AnimatorGUI(ofRectangle area);
    ~AnimatorGUI();
    
    void createAnimationGUI(int count);
    void draw();
    
    void mousePressed(ofMouseEventArgs &args);
    ofEvent<int>    buttonPressed;
    
private:
    void newAnimation(int id);
    
    ofRectangle         _drawArea;
    vector<Button>      _buttons;
};

#endif /* AnimatorGUI_hpp */
