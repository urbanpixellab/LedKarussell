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
    fbo.allocate(10,10);
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
    fbo.allocate(area.width, area.height);
    drawArea = area;
    name = n;
    isPressed = false;
    isToggle = toogle;
    updateFbo(false);
    mouseOffset = offset;
}

void Button::updateFbo(bool isPressed)
{
    fbo.begin();
    ofClear(0, 0, 0);
    if (isPressed) ofSetColor(Active);
    else ofSetColor(nonActive);
    
    ofFill();
    ofDrawRectRounded(2,2,fbo.getWidth() - 4,fbo.getHeight() - 4, 4);
    ofSetColor(textColor);
    ofDrawBitmapString(name, 6, 14);
    fbo.end();
}

void Button::setColors(ofColor nonactive, ofColor active, ofColor text){
    nonActive = nonactive;
    Active = active;
    textColor = text;
    updateFbo(false);
}

void Button::draw()
{
    if (isPressed == true && ofGetElapsedTimef() > pressTimeout && isToggle == false){
        isPressed = false;
        updateFbo(isPressed);
    }
    fbo.draw(drawArea.x, drawArea.y);
}

void Button::pressedControler()
{
    isPressed = !isPressed;
    updateFbo(isPressed);
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
        
        isPressed = true;
        pressTimeout = ofGetElapsedTimef() + 0.05;
    }
    updateFbo(isPressed);
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
