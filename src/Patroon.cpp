//
//  Patroon.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 01/09/18.
//
//

#include "Patroon.hpp"

Patroon::Patroon(int id,int curveA,int curveB,float freqA,float freqB,float dirA, float dirB,int timeA, int timeB,int colorA, int colorB,int colorC, int colorD): _id(id)
{
    _curve[0] = curveA;
    _curve[1] = curveB;

    _freq[0] = freqA;
    _freq[1] = freqB;

    _dir[0] = dirA;
    _dir[1] = dirB;

    _time[0] = timeA;
    _time[1] = timeB;
    
    _color[0] = colorA;
    _color[1] = colorB;
    _color[2] = colorC;
    _color[3] = colorD;
    
    clear();
    createGui();
    _visible = false;
}

Patroon::~Patroon(){}

void Patroon::clear()
{
    for (int step = 0; step < 8; step++)
    {
        for (int row = 0; row < 13; row++)
        {
            _sequenceA[step][row] = false;
            _sequenceB[step][row] = false;
        }
    }
}

void Patroon::nextStep()
{
    _step++;
    if(_step >= 8)_step = 0;
}

void Patroon::setSeqSel(int layer,int step,int *selection,int length)
{
    if (layer == 0)
    {
        for (int i = 0; i < length; i++)
        {
            _sequenceA[step][selection[i]] = true;
        }
    }
    else if(layer == 1)
    {
        for (int i = 0; i < length; i++)
        {
            _sequenceA[step][selection[i]] = true;
        }
    }
}

void Patroon::drawGui()
{
    if (!_visible)return;
    ofSetColor(255);
    ofDrawBitmapString("Curve Select", 300,300);
    for (int i = 0; i < _curveAButtons.size(); i++)
    {
        ofSetColor(255);
        if (_curveAButtons[i].isPressed)
        {
            ofSetColor(255,0,0);
        }
        ofDrawRectangle(_curveAButtons[i].area);
        
        ofSetColor(255);
        if (_curveBButtons[i].isPressed)
        {
            ofSetColor(255,0,0);
        }
        ofDrawRectangle(_curveBButtons[i].area);
    }
    colorselectorA.draw();
    colorselectorB.draw();

}

void Patroon::setVisible()
{
    bool old = _visible;
    _visible = true;
    if(old != _visible) ofAddListener(ofEvents().mousePressed, this, &Patroon::mousePressed);
}

void Patroon::setInvisible()
{
    bool old = _visible;
    _visible = false;
    if(old != _visible) ofRemoveListener(ofEvents().mousePressed, this, &Patroon::mousePressed);
}

void Patroon::mousePressed(ofMouseEventArgs & args)
{
    //if (!_drawArea.inside(args.x, args.y)) return;
    //check animations
    for (int i = 0; i < _curveAButtons.size(); i++)
    {
        if(_curveAButtons[i].area.inside(args.x, args.y))
        {
            _curve[0] = i;
            return;
        }
    }
    
    for (int i = 0; i < _curveBButtons.size(); i++)
    {
        if(_curveBButtons[i].area.inside(args.x, args.y))
        {
            _curve[1] = i;
            return;
        }
    }
}


void Patroon::setSeqAColor(string &s){
    
    vector<string> a = ofSplitString(s, ",");
    _color[0] = ofToInt(a[0]);
    _color[1] = ofToInt(a[1]);
     cout << " Patroon Got an event " << a.size() << " " << ofToString(a[0]) << " " << ofToString(a[1]) << endl;
}

void Patroon::setSeqBColor(string &s){
    
    vector<string> a = ofSplitString(s, ",");
    _color[2] = ofToInt(a[0]);
    _color[3] = ofToInt(a[1]);

    cout << " Patroon Got an event" << ofToString(a[0]) << " " << ofToString(a[1]) << endl;
}

void Patroon::setColors(int * array,int length)
{
    _color[0]= array[0];
    _color[1]= array[1];
    _color[2]= array[2];
    _color[3]= array[3];
}

vector<int> Patroon::getColorIDs(){
    vector<int> c;
    
    int l = sizeof(_color)/sizeof(_color[0]);
    for(int i=0;i<l;i++)
    {
        c.push_back(_color[i]);
    }
    return c;
}

/////////niew
void Patroon::createGui()
{
    //create curve buttons
    int w = 50;
    int h = 30;
    for (int i = 0; i < 7; i++)
    {
        Button butA;
        Button butB;
        butA.id = i;
        butA.name = "test";
        butA.area = ofRectangle(200 + (i * (w + 5)),200,w,h);
        butA.color = ofColor(188,188,190);
        butA.toggle = true;
        butA.isPressed = false;
        _curveAButtons.push_back(butA);
        
        butB.id = i;
        butB.name = "test";
        butB.area = ofRectangle(200 + (i * (w + 5)),200 + h*1.5,w,h);
        butB.color = ofColor(188,188,190);
        butB.toggle = true;
        butB.isPressed = false;
        _curveBButtons.push_back(butB);
    }
    
    // Create colorselectorS
    colorselectorA.setup(ofRectangle(300,300+100,200,200));
    colorselectorB.setup(ofRectangle(300+220,300+100,200,200));
    
    
    //set the initial things, like blackout
    //newACurve(0);
    //newBCurve(0);
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
