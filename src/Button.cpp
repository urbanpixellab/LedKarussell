//
//  Button.cpp
//  TextureGenerator
//
//  Created by Enrico Becker on 30/08/16.
//
//

#include "Button.hpp"

Button::Button()
{
    activateListener();
    drawArea = ofRectangle(10,10,10,10);
    name = "unknown";
    nonActive = ofColor(255);
    Active = ofColor(255,0,0);
    textColor = ofColor(0);
}
Button::~Button()
{
    deactivateListener();
}

void Button::setup(ofRectangle area, string n, bool toogle)
{
    setup(area, n, toogle, ofVec2f(0,0));
}

void Button::setup(ofRectangle area, string n, bool toogle, ofVec2f offset)
{
    drawArea = area;
    name = n;
    isPressed = false;
    isToggle = toogle;
    mouseOffset = offset;
}

void Button::setColors(ofColor nonactive, ofColor active, ofColor text){
    nonActive = nonactive;
    Active = active;
    textColor = text;
}

void Button::draw()
{
    if (isPressed) ofSetColor(Active);
    else ofSetColor(nonActive);
    
    ofFill();
    ofDrawRectRounded(2,2,drawArea.getWidth() - 4,drawArea.getHeight() - 4, 4);
    ofSetColor(textColor);
    ofDrawBitmapString(name, 6, 14);
}

void Button::pressedControler()
{
    isPressed = !isPressed;
    ofNotifyEvent(buttonPressed, name);
}

void Button::mousePressed(ofMouseEventArgs &arg)
{
    if(!drawArea.inside(arg.x - mouseOffset.x, arg.y - mouseOffset.y)) return;
    if (isToggle)
    {
        isPressed = !isPressed;
    }
    else
    {
        isPressed = false;
        //pressTimeout = ofGetElapsedTimef() + 0.05;
    }
    ofNotifyEvent(buttonPressed, name);
}

void Button::activateListener()
{
    ofAddListener(ofEvents().mousePressed, this, &Button::mousePressed);
}

void Button::deactivateListener()
{
    ofRemoveListener(ofEvents().mousePressed, this, &Button::mousePressed);
}
