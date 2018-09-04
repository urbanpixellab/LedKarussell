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
}


void AnimatorGUI::createAnimationGUI(int animationCount)
{
    //create animation buttons
    int w = 50;
    int h = 30;
    for (int i = 0; i < animationCount; i++)
    {
        Button butA;
        Button butB;
        butA.id = i;
        butA.name = "test";
        butA.area = ofRectangle(_drawArea.x + (i * (w + 5)),_drawArea.y,w,h);
        butA.color = ofColor(188,188,190);
        butA.toggle = true;
        butA.isPressed = false;
        _curveAButtons.push_back(butA);

        butB.id = i;
        butB.name = "test";
        butB.area = ofRectangle(_drawArea.x + (i * (w + 5)),_drawArea.y + h*1.5,w,h);
        butB.color = ofColor(188,188,190);
        butB.toggle = true;
        butB.isPressed = false;
        _curveBButtons.push_back(butB);
    }
    
    // Create colorselectorS
    colorselectorA.setup(ofRectangle(_drawArea.getX(),_drawArea.getY()+100,200,200));
    colorselectorB.setup(ofRectangle(_drawArea.getX()+220,_drawArea.getY()+100,200,200));

    
    //set the initial things, like blackout
    newACurve(0);
    newBCurve(0);
    //create patern select buttons;
    w = 70;
    h = 20;
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

void AnimatorGUI::draw(ofImage &pre, ofImage &live)
{
    ofSetColor(255);
    ofDrawBitmapString("Curve Select", _drawArea.x, _drawArea.y);
    for (int i = 0; i < _curveAButtons.size(); i++)
    {
        ofSetColor(_curveAButtons[i].color);
        if (_curveAButtons[i].isPressed)
        {
            ofSetColor(255,0,0);
        }
        ofDrawRectangle(_curveAButtons[i].area);

        ofSetColor(_curveBButtons[i].color);
        if (_curveBButtons[i].isPressed)
        {
            ofSetColor(255,0,0);
        }
        ofDrawRectangle(_curveBButtons[i].area);
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
        if(_isPlay == i) ofSetColor(255,0,0);
        if (_isEdit == i) ofSetColor(0,0,255);
        if (_isPlayStepped == i) ofSetColor(255,0,0);
        ofDrawRectangle(_patSelButtons[i].area);
        ofSetColor(255);
        ofDrawBitmapString("Pl Ed Ps", _patSelButtons[i].area.x, _patSelButtons[i].area.getBottom());
        // the button is 1/3 for play direct 1/3 for edit and 1/3 for play the next beat like in ableton
    }
}

void AnimatorGUI::newACurve(int id)
{
    for (int i = 0; i < _curveAButtons.size(); i++)
    {
        _curveAButtons[i].isPressed = false;
    }
    _curveAButtons[id].isPressed = true;
    ofNotifyEvent(curveAPressed, id);
}

void AnimatorGUI::newBCurve(int id)
{
    for (int i = 0; i < _curveBButtons.size(); i++)
    {
        _curveBButtons[i].isPressed = false;
    }
    _curveBButtons[id].isPressed = true;
    ofNotifyEvent(curveBPressed, id);
}

/*
void AnimatorGUI::setColor(string &s){
    
    vector<string> arguments = ofSplitString(s, ",");
   
    cout << " Got an event" << ofToString(arguments[0]) << " " << ofToString(arguments[1]) << endl;
}
*/

void AnimatorGUI::mousePressed(ofMouseEventArgs &args)
{
    //if (!_drawArea.inside(args.x, args.y)) return;
    //check animations
    for (int i = 0; i < _curveAButtons.size(); i++)
    {
        if(_curveAButtons[i].area.inside(args.x, args.y))
        {
            // reset all programm buttons and enable this one
            newACurve(i);
            return;
        }
    }

    for (int i = 0; i < _curveBButtons.size(); i++)
    {
        if(_curveBButtons[i].area.inside(args.x, args.y))
        {
            // reset all programm buttons and enable this one
            newBCurve(i);
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
