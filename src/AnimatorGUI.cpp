//
//  AnimatorGUI.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 07/08/18.
//
//

#include "AnimatorGUI.hpp"

AnimatorGUI::AnimatorGUI(ofRectangle area):_drawArea(area)
{
    ofAddListener(ofEvents().mousePressed, this, &AnimatorGUI::mousePressed);
}
AnimatorGUI::~AnimatorGUI()
{
    ofRemoveListener(ofEvents().mousePressed, this, &AnimatorGUI::mousePressed);
}

void AnimatorGUI::createAnimationGUI(int count)
{
    //create animation buttons
    int w = 50;
    int h = 30;
    for (int i = 0; i < count; i++)
    {
        Button b;
        b.id = i;
        b.name = "test";
        b.area = ofRectangle(_drawArea.x + (i * (w + 5)),_drawArea.y,w,h);
        b.color = ofColor(188,188,190);
        b.toggle = true;
        b.isPressed = false;
        _buttons.push_back(b);
    }
}

void AnimatorGUI::draw()
{
    for (int i = 0; i < _buttons.size(); i++)
    {
        ofSetColor(_buttons[i].color);
        if (_buttons[i].isPressed)
        {
            ofSetColor(255,0,0);
        }
        ofDrawRectangle(_buttons[i].area);
    }
}

void AnimatorGUI::newAnimation(int id)
{
    for (int i = 0; i < _buttons.size(); i++)
    {
        _buttons[i].isPressed = false;
    }
    _buttons[id].isPressed = true;
    ofNotifyEvent(buttonPressed, id);
}

void AnimatorGUI::mousePressed(ofMouseEventArgs &args)
{
    if (!_drawArea.inside(args.x, args.y)) return;
    for (int i = 0; i < _buttons.size(); i++)
    {
        if(_buttons[i].area.inside(args.x, args.y))
        {
            // reset all programm buttons and enable this one
            newAnimation(i);
            return;
        }
    }
    
}
