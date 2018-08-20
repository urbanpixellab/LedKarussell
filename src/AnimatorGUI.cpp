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

void AnimatorGUI::init()
{
    //set the initial things, like blackout
    newAnimation(0);
    newEnable(0);
    
}


void AnimatorGUI::createAnimationGUI(int animationCount, int enableCount)
{
    //create animation buttons
    int w = 50;
    int h = 30;
    for (int i = 0; i < animationCount; i++)
    {
        Button b;
        b.id = i;
        b.name = "test";
        b.area = ofRectangle(_drawArea.x + (i * (w + 5)),_drawArea.y,w,h);
        b.color = ofColor(188,188,190);
        b.toggle = true;
        b.isPressed = false;
        _animationButtons.push_back(b);
    }
    //create enable buttons
    for (int i = 0; i < enableCount; i++)
    {
        Button b;
        b.id = i;
        b.name = "test";
        b.area = ofRectangle(_drawArea.x + (i * (w + 5)),_drawArea.y + 2*h,w,h);
        b.color = ofColor(188,188,190);
        b.toggle = true;
        b.isPressed = false;
        _enableButtons.push_back(b);
    }
}

void AnimatorGUI::draw(ofImage &pre, ofImage &live)
{
    ofSetColor(255);
    ofDrawBitmapString("Animation Select", _drawArea.x, _drawArea.y);
    ofDrawBitmapString("Enable Select", _drawArea.x, _drawArea.y + 60);
    for (int i = 0; i < _animationButtons.size(); i++)
    {
        ofSetColor(_animationButtons[i].color);
        if (_animationButtons[i].isPressed)
        {
            ofSetColor(255,0,0);
        }
        ofDrawRectangle(_animationButtons[i].area);
    }
    
    ofSetColor(255);
    for (int i = 0; i < _enableButtons.size(); i++)
    {
        ofSetColor(_enableButtons[i].color);
        if (_enableButtons[i].isPressed)
        {
            ofSetColor(255,0,0);
        }
        ofDrawRectangle(_enableButtons[i].area);
    }
    ofSetColor(255);
    pre.draw(_drawArea.x, _drawArea.y + 200,170,100);
    live.draw(_drawArea.x + 180, _drawArea.y + 200,170,100);
    
    //draw it under the preview windows
    ofPushMatrix();
    ofTranslate(_drawArea.x,_drawArea.getBottom() -200);
    ofScale(ofGetWidth()/3, 100);
    pre.bind();
    _realStructure.draw();
    pre.unbind();
    ofPopMatrix();
}

void AnimatorGUI::newAnimation(int id)
{
    for (int i = 0; i < _animationButtons.size(); i++)
    {
        _animationButtons[i].isPressed = false;
    }
    _animationButtons[id].isPressed = true;
    ofNotifyEvent(animationPressed, id);
}

void AnimatorGUI::newEnable(int id)
{
    for (int i = 0; i < _enableButtons.size(); i++)
    {
        _enableButtons[i].isPressed = false;
    }
    _enableButtons[id].isPressed = true;
    ofNotifyEvent(enablePressed, id);
}

void AnimatorGUI::mousePressed(ofMouseEventArgs &args)
{
    if (!_drawArea.inside(args.x, args.y)) return;
    //check animations
    for (int i = 0; i < _animationButtons.size(); i++)
    {
        if(_animationButtons[i].area.inside(args.x, args.y))
        {
            // reset all programm buttons and enable this one
            newAnimation(i);
            return;
        }
    }
    //check enables
    for (int i = 0; i < _enableButtons.size(); i++)
    {
        if(_enableButtons[i].area.inside(args.x, args.y))
        {
            // reset all programm buttons and enable this one
            newEnable(i);
            return;
        }
    }

}
