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
    for (int i = 0; i < _patEditButtons.size(); i++)
    {
        delete _patEditButtons[i];
        delete _patLiveButtons[i];
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
    freqA->setup(ofRectangle(x,y+80,60,60),ofVec2f(0.1,15),0,true);
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
    freqB->setup(ofRectangle(x,y+80,60,60),ofVec2f(0.1,15),0,true);
    freqB->setResolution(100);
    freqB->setName("freq");
    _slidersB[2] = freqB;
    RotarySlider *dirB = new RotarySlider();
    dirB->setup(ofRectangle(x+80,y+80,60,60),ofVec2f(0,3),0,true);
    dirB->setResolution(20);
    dirB->setName("dir");
    _slidersB[3] = dirB;
    
    //Create SegmentSelector
    segmenselectorA.setup(ofRectangle(_drawArea.getX(),_drawArea.getY()+430,200,200));
    segmenselectorB.setup(ofRectangle(_drawArea.getX()+280,_drawArea.getY()+430,200,200));

    // Create colorselectorS
    colorselectorA.setup(ofRectangle(_drawArea.getX(),_drawArea.getY()+150,200,200));
    colorselectorB.setup(ofRectangle(_drawArea.getX()+220,_drawArea.getY()+150,200,200));
    
    //create patern select buttons for edit and live with double edit size
    w = 70;
    h = 20;
    for (int i = 0; i < 16; i++)
    {
        string name = "Edit_" + ofToString(i);
        x = 750 + ((i%4)*w*1.1);
        y = 500 + ((i / 4) * h * 1.1);
        Button *e = new Button(i,ofRectangle(x,y,w,h),name,true);
        ofAddListener(e->buttonIDPressed, this, &AnimatorGUI::editButtonPressed);
        _patEditButtons.push_back(e);
        name = "Pattern_" + ofToString(i);
        x = 800 + ((i%4)*w*2.8);
        y = 150 + ((i / 4) * h * 1.1);
        Button *l = new Button(i,ofRectangle(x,y,w*2.6,h),name,true);
        ofAddListener(l->buttonIDPressed, this, &AnimatorGUI::liveButtonPressed);
        _patLiveButtons.push_back(l);
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
    ofTranslate(800,300);
    ofScale(800, 200);
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
    
    // SegmentSelector
    segmenselectorA.draw();
    segmenselectorB.draw();
    
    // the patron selector
    for (int i = 0; i < _patEditButtons.size(); i++)
    {
        //set the color based on the mode
        _patEditButtons[i]->draw();
        _patLiveButtons[i]->draw();
    }
}

void AnimatorGUI::mousePressed(ofMouseEventArgs &args)
{
}
void AnimatorGUI::editButtonPressed(int &id)
{
    for (int i=0; i < _patEditButtons.size(); i++)
    {
        if(i == id) continue;
        _patEditButtons[i]->getState() = false;
    }
    ofNotifyEvent(patronEDIT,id);
}

void AnimatorGUI::liveButtonPressed(int &id)
{
    for (int i=0; i < _patLiveButtons.size(); i++)
    {
        if(i == id) continue;
        _patLiveButtons[i]->getState() = false;
    }
    ofNotifyEvent(patronPLAY,id);
}

//}
