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

vector<int> Patroon::getColorIDs(){
    vector<int> c;
    
    int l = sizeof(_color)/sizeof(_color[0]);
    for(int i=0;i<l;i++)
    {
        c.push_back(_color[i]);
    }
    return c;
}


