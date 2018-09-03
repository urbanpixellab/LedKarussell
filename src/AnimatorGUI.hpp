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
#include "ColorSelector.hpp"

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
    void createAnimationGUI(int animationCount);
    void draw(ofImage &pre, ofImage &live);
    
    ofMesh &getMesh(){return _realStructure;}; //here we can work on the mesh
    
    void mousePressed(ofMouseEventArgs &args);
    ofEvent<int>    curvePressed;
    
    void setColor(string &c);
    
    ColorSelector colorselectorA;
    ColorSelector colorselectorB;

private:
    void newCurve(int id);

    ofRectangle         _drawArea;
    vector<Button>      _curveButtons;
    
    ofMesh              _realStructure;// the mesh to paint on
    
};

#endif /* AnimatorGUI_hpp */
