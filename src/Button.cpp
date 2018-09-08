//
//  Button.cpp
//  TextureGenerator
//
//  Created by Enrico Becker on 30/08/16.
//
//

// FIXME: use a loaded font instead of drawbitmapstring for more coontrol
#include "Button.hpp"

Button::Button()
{
    activateListener();
    drawArea = ofRectangle(10,10,10,10);
    name = "unknown";
    nonActive = ofColor(255);
    Active = ofColor(255,0,0);
    textColor = ofColor(0);
    isPressed = false;
    _verdana.load("verdana.ttf", 10, true, true);
}

Button::Button(int id,ofRectangle area, string name, bool toogle):id(id), drawArea(area),name(name),isToggle(toogle)
{
    activateListener();
    nonActive = ofColor(255);
    Active = ofColor(255,0,0);
    textColor = ofColor(0);
    isPressed = false;
    _verdana.load("verdana.ttf", 10, true, true);
}

Button::~Button()
{
//    deactivateListener();
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

void  Button::drawTextCentered(string text, ofRectangle area)
{
    ofRectangle r = _verdana.getStringBoundingBox(text, 0,0);
    int x = area.x + area.getWidth()/2 - r.getWidth()/2;
    int y = area.y + area.getHeight()/2 + r.getHeight()/2;
    _verdana.drawString(name, x, y);
}

void Button::draw()
{
    if (isPressed) ofSetColor(Active);
    else ofSetColor(nonActive);
    ofFill();
    ofDrawRectRounded(drawArea.x + 2,drawArea.y + 2,drawArea.getWidth() - 4,drawArea.getHeight() - 4, 4);
    ofSetColor(textColor);
    drawTextCentered(name, drawArea);    
}

void Button::drawRedPulsing(float &dt)
{
    ofSetColor(Active.r * (1-dt),Active.g * (1-dt),Active.b * (1-dt));
    ofFill();
    ofDrawRectRounded(drawArea.x + 2,drawArea.y + 2,drawArea.getWidth() - 4,drawArea.getHeight() - 4, 4);
    ofSetColor(textColor);
    ofDrawBitmapString(name, drawArea.x + 6, drawArea.y + 14);
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
        pressTimeout = ofGetElapsedTimef() + 0.05;
    }
    ofNotifyEvent(buttonPressed, name);
    ofNotifyEvent(buttonIDPressed, id);
}

void Button::activateListener()
{
    ofAddListener(ofEvents().mousePressed, this, &Button::mousePressed);
}

void Button::deactivateListener()
{
    ofRemoveListener(ofEvents().mousePressed, this, &Button::mousePressed);
}
