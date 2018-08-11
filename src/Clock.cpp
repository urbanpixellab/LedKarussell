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
    _beatLength = 500 * 1000;//120bpm
    _lastBeat = ofGetElapsedTimeMicros();
    _nextBeat = _lastBeat + _beatLength;
    _beatScale = 1;//normal
    _isBeat = false;
    _tapCount = 0;
    
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
    }
    else if(now > _nextBeat)
    {
        _lastBeat = _nextBeat;
        _dT = (now - _lastBeat) / _beatLength;
        if(_dT >= 1.0)_dT = 1;//limit it to one
        _actValue = _dT; /// gets later manipulated by the time and the beat scale faktor
        _nextBeat = _lastBeat+  (_beatLength*_beatScale);
        _isBeat = true;
    }
    else
    {
        _dT = (now - _lastBeat) / _beatLength;
        if(_dT >= 1.0)_dT = 1;//limit it to one
        _actValue = _dT; /// gets later manipulated by the time and the beat scale faktor
        _isBeat = false;
    }
}

void Clock::tap()
{
    //do the tap calculation
    u_int64_t now = ofGetElapsedTimeMicros();
    _timer[_tapCount] = now;
    _tapCount++;
    if (_tapCount > 1)
    {
        //we have on full set off data
        _tapCount = 0;
        //calculate bpm and reset timer
        _bpm = calculateBPM(_timer,2);
    }
    int timeout = 10000000;///10 sec then tap is automaticaly reset
    _lastTapTime = now + timeout;
}

string Clock::calculateBPM(u_int64_t *data,int length)
{
    // limit the range to 25-500bpm or it is a fault
    u_int64_t duration = data[1]-data[0];
    float milliDuration = duration/1000.;
    _beatLength = milliDuration;
    //set last beat as last and next as next
    _lastBeat = data[1];
    _nextBeat = _lastBeat + duration;
    _beatLength = duration;
    //60,000 / BPM = one beat in milliseconds
    return ofToString(60000 / milliDuration,1);
}
