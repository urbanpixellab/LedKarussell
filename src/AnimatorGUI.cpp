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
    newCurve(0);
    //create patern select buttons;
    int w = 70;
    int h = 20;
    for (int i = 0; i < 16; i++)
    {
        Button b;
        b.id = i;
        b.name = "patron" + ofToString(i);
        int x = 600 + ((i%4)*w*1.1);
        int y = 300 + ((i / 4) * h * 1.1);
        b.area = ofRectangle(x,y,w,h);
        b.toggle = true;
        _patSelButtons.push_back(b);
    }
}


void AnimatorGUI::createAnimationGUI(int animationCount)
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
        _curveButtons.push_back(b);
    }
    
    // Create colorselectorS
    colorselectorA.setup(ofRectangle(_drawArea.getX(),_drawArea.getY()+50,200,200));
    colorselectorB.setup(ofRectangle(_drawArea.getX()+220,_drawArea.getY()+50,200,200));

}

void AnimatorGUI::draw(ofImage &pre, ofImage &live)
{
    ofSetColor(255);
    ofDrawBitmapString("Curve Select", _drawArea.x, _drawArea.y);
    for (int i = 0; i < _curveButtons.size(); i++)
    {
        ofSetColor(_curveButtons[i].color);
        if (_curveButtons[i].isPressed)
        {
            ofSetColor(255,0,0);
        }
        ofDrawRectangle(_curveButtons[i].area);
    }
    
    ofSetColor(255);
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

    ofPushMatrix();
    ofTranslate(_drawArea.x + 350,_drawArea.getBottom() -200);
    ofScale(ofGetWidth()/3, 100);
    live.bind();
    _realStructure.draw();
    live.unbind();
    ofPopMatrix();
    
    // Color Selector
    colorselectorA.draw();
    colorselectorB.draw();
    
    // the patron selector
    for (int i = 0; i < _patSelButtons.size(); i++)
    {
        //set the color based on the mode
        if(_isPlay == i) ofSetColor(128,0,0);
        if (_isEdit == i) ofSetColor(0,0,128);
        if (_isPlayStepped == i) ofSetColor(128,128,128);
        ofDrawRectangle(_patSelButtons[i].area);
        ofSetColor(255);
        ofDrawBitmapString("Pl Ed Ps", _patSelButtons[i].area.x, _patSelButtons[i].area.getBottom());
        // the button is 1/3 for play direct 1/3 for edit and 1/3 for play the next beat like in ableton
    }
}

void AnimatorGUI::newCurve(int id)
{
    for (int i = 0; i < _curveButtons.size(); i++)
    {
        _curveButtons[i].isPressed = false;
    }
    _curveButtons[id].isPressed = true;
    ofNotifyEvent(curvePressed, id);
}

void AnimatorGUI::setColor(string &s){
    
    vector<string> arguments = ofSplitString(s, ",");
   
    cout << " Got an event" << ofToString(arguments[0]) << " " << ofToString(arguments[1]) << endl;
}

void AnimatorGUI::mousePressed(ofMouseEventArgs &args)
{
    //if (!_drawArea.inside(args.x, args.y)) return;
    //check animations
    for (int i = 0; i < _curveButtons.size(); i++)
    {
        if(_curveButtons[i].area.inside(args.x, args.y))
        {
            // reset all programm buttons and enable this one
            newCurve(i);
            return;
        }
    }
    //check patron select buttons
    //tofix check states, not proper working
    for (int i = 0; i < _patSelButtons.size(); i++)
    {
        if(_patSelButtons[i].area.inside(args.x, args.y))
        {
            // reset all programm buttons and enable this one
            //first third
            ofRectangle r = _patSelButtons[i].area;
            float value = ofMap(args.x,r.x,r.x+r.width,0.,1.);
            if (value < 0.33)
            {
                ofNotifyEvent(patronPLAY,i);
                _isPlay = i;
                //a
            }
            else if (value < 0.66)
            {
                ofNotifyEvent(patronEDIT,i);
                _isEdit = i;
            }
            else
            {
                ofNotifyEvent(patronPLAYSTEPPED,i);
                _isPlayStepped = i;
            }
            
            return;
        }
    }
    
}
