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

    for (int i = 0; i < SLIDERS; i++)
    {
        delete _slidersA[i];
    }
    for (int i = 0; i < SLIDERS; i++)
    {
        delete _slidersB[i];
    }
}

void AnimatorGUI::createAnimationGUI(int animationCount)
{
    //create curve buttons
    int w = 50;
    int h = 30;
    // sliders
    int x = _drawArea.getX();
    int y = _drawArea.getY();
    
    RotarySlider *curveA = new RotarySlider();
    curveA->setup(ofRectangle(x,y,60,60),ofVec2f(0,animationCount-1),0,true);
    curveA->setResolution(20);
    curveA->setName("curve");
    _slidersA[0] = curveA;
    RotarySlider *timeA = new RotarySlider();
    timeA->setup(ofRectangle(x+80,y,60,60),ofVec2f(0,4),0,true);
    timeA->setResolution(20);
    timeA->setName("multi");
    _slidersA[1] = timeA;
    RotarySlider *freqA = new RotarySlider();
    freqA->setup(ofRectangle(x,y+80,60,60),ofVec2f(0.1,25),0,true);
    freqA->setResolution(100);
    freqA->setName("freq");
    _slidersA[2] = freqA;
    RotarySlider *dirA = new RotarySlider();
    dirA->setup(ofRectangle(x+80,y+80,60,60),ofVec2f(0,3),0,true);
    dirA->setResolution(20);
    dirA->setName("dir");
    _slidersA[3] = dirA;

    x = _drawArea.getX()+220;
    RotarySlider *curveB = new RotarySlider();
    curveB->setup(ofRectangle(x,y,60,60),ofVec2f(0,animationCount-1),0,true);
    curveB->setResolution(20);
    curveB->setName("curve");
    _slidersB[0] = curveB;
    RotarySlider *timeB = new RotarySlider();
    timeB->setup(ofRectangle(x+80,y,60,60),ofVec2f(0,4),0,true);
    timeB->setResolution(20);
    timeB->setName("multi");
    _slidersB[1] = timeB;
    RotarySlider *freqB = new RotarySlider();
    freqB->setup(ofRectangle(x,y+80,60,60),ofVec2f(0.1,25),0,true);
    freqB->setResolution(100);
    freqB->setName("freq");
    _slidersB[2] = freqB;
    RotarySlider *dirB = new RotarySlider();
    dirB->setup(ofRectangle(x+80,y+80,60,60),ofVec2f(0,3),0,true);
    dirB->setResolution(20);
    dirB->setName("dir");
    _slidersB[3] = dirB;

    
    
    // Create colorselectorS
    colorselectorA.setup(ofRectangle(_drawArea.getX(),_drawArea.getY()+150,200,200));
    colorselectorB.setup(ofRectangle(_drawArea.getX()+220,_drawArea.getY()+150,200,200));
    
    //create patern select buttons;
    w = 70;
    h = 20;
    for (int i = 0; i < 16; i++)
    {
        Button b;
        b.id = i;
        b.name = "patron" + ofToString(i);
        int x = 650 + ((i%4)*w*1.1);
        int y = 300 + ((i / 4) * h * 1.1);
        b.area = ofRectangle(x,y,w,h);
        b.toggle = true;
        _patSelButtons.push_back(b);
    }

}

void AnimatorGUI::draw(ofImage &pre, ofImage &live)
{
    ofSetColor(255);
    pre.draw(_drawArea.x, _drawArea.y + 220,200,100);
    live.draw(_drawArea.x + 220, _drawArea.y + 220,200,100);
    
    //draw it under the preview windows
    ofPushMatrix();
    ofTranslate(_drawArea.x,_drawArea.getBottom() -100);
    ofScale(ofGetWidth()/3, 100);
    pre.bind();
    _realStructure.draw();
    pre.unbind();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(_drawArea.x + 400,_drawArea.getBottom() -100);
    ofScale(ofGetWidth()/3, 100);
    live.bind();
    _realStructure.draw();
    live.unbind();
    ofPopMatrix();
    //sliders
    for (int i = 0; i < SLIDERS; i++)
    {
        _slidersA[i]->draw();
        _slidersB[i]->draw();
    }
    
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

/*
void AnimatorGUI::setColor(string &s){
    
    vector<string> arguments = ofSplitString(s, ",");
   
    cout << " Got an event" << ofToString(arguments[0]) << " " << ofToString(arguments[1]) << endl;
}
*/


void AnimatorGUI::mousePressed(ofMouseEventArgs &args)
{

    //if (!_drawArea.inside(args.x, args.y)) return;
    
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
                _isPlay = i;
                // reset all other players
                for (int j = 0; j < _patSelButtons.size(); j++)
                {
                    if(j == i) continue;
                    _patSelButtons[j].isPressed = false;
                }
                ofNotifyEvent(patronPLAY,i);
            }
            else if (value < 0.66)
            {
                _isEdit = i;
                ofNotifyEvent(patronEDIT,i);
            }
            else
            {
                _isPlayStepped = i;
                for (int j = 0; j < _patSelButtons.size(); j++)
                {
                    if(j == i) continue;
                    _patSelButtons[j].isPressed = false;
                }
                ofNotifyEvent(patronPLAYSTEPPED,i);
            }
            
            return;
        }
    }
    
}
