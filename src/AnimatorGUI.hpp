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
    
    void init();
    void createAnimationGUI(int animationCount,int enableCount);
    void draw(ofImage &pre, ofImage &live);
    
    void mousePressed(ofMouseEventArgs &args);
    ofEvent<int>    animationPressed;
    ofEvent<int>    enablePressed;

private:
    void newAnimation(int id);
    void newEnable(int id);

    ofRectangle         _drawArea;
    vector<Button>      _animationButtons;
    vector<Button>      _enableButtons;
    
};

#endif /* AnimatorGUI_hpp */
