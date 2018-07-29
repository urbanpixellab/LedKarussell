//
//  Clock.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 25/07/18.
//
//

#include "Clock.hpp"

Clock::Clock()
{
    _dT = 0;
    _actValue = 0;
    _beatLength = 500* 1000;//120bpm
    _lastBeat = ofGetElapsedTimeMicros();
    _nextBeat = _lastBeat + _beatLength;
    _beatScale = 1;//normal
    _isBeat = false;
}
Clock::~Clock(){}

void Clock::update()
{
    int now = ofGetElapsedTimeMicros();
    if (now == _nextBeat)
    {
        //it is a new beat
//        _lastBeat = _nextBeat;
        _nextBeat = now + (_beatLength*_beatScale);
        ofSetBackgroundColor(ofRandom(255), ofRandom(255), ofRandom(255));
        _dT = 1;
        _actValue = _dT; /// gets later manipulated by the time and the beat scale faktor
        //cout << "beat" << endl;
        _nextBeat = now;
        _isBeat = true;
        cout << "next beat" << endl;
        
    }
    else if(now > _nextBeat)
    {
        //in beat
        _lastBeat = _nextBeat;
        _dT = (now - _lastBeat) / _beatLength;
        if(_dT >= 1.0)_dT = 1;//limit it to one
        _actValue = _dT; /// gets later manipulated by the time and the beat scale faktor
        _nextBeat = _lastBeat+  (_beatLength*_beatScale);
        _isBeat = true;
        cout << "next beat" << endl;
    }
    else
    {
        //in beat
        _dT = (now - _lastBeat) / _beatLength;
        if(_dT >= 1.0)_dT = 1;//limit it to one
        _actValue = _dT; /// gets later manipulated by the time and the beat scale faktor
        _isBeat = false;
    }
    //cout << _dT << endl;
}
