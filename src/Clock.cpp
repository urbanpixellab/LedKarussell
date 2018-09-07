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
    _bpm = "120";
    _lastBeat = ofGetElapsedTimeMicros();
    _nextBeat = _lastBeat + _beatLength;
    _isBeat = false;
    _tapCount = 0;
    _stepCounter = 0;
    
}
Clock::~Clock(){}

void Clock::update()
{
    int now = ofGetElapsedTimeMicros();
    if (now == _nextBeat)
    {
        //it is a new beat
//        _lastBeat = _nextBeat;
        _nextBeat = now + _beatLength;
        ofSetBackgroundColor(ofRandom(255), ofRandom(255), ofRandom(255));
        _dT = (now - _lastBeat) / _beatLength;
        _actValue = _dT; /// gets later manipulated by the time and the beat scale faktor
        _nextBeat = now;
        _isBeat = true;
        ofNotifyEvent(newStep, _isBeat);
        _stepCounter++;
    }
    else if(now > _nextBeat)
    {
        _lastBeat = _nextBeat;
        _dT = (now - _lastBeat) / _beatLength;
        if(_dT >= 1.0)_dT = 1;//limit it to one
        _actValue = _dT; /// gets later manipulated by the time and the beat scale faktor
        _nextBeat = _lastBeat+ _beatLength;
        _isBeat = true;
        ofNotifyEvent(newStep, _isBeat);
        _stepCounter++;
    }
    else
    {
        _dT = (now - _lastBeat) / _beatLength;
        if(_dT >= 1.0)_dT = 1;//limit it to one
        _actValue = _dT; /// gets later manipulated by the time and the beat scale faktor
        _isBeat = false;
    }
    
    // the other delta times
    //dT = (now - lastBeatTime) / beatDuration; old version
    calculateDeltaTimeMultiplyed();

    if (_stepCounter >= 16) _stepCounter = 0;
    //we use _dt as basement and the stepcounter
    
    //now is in micros not in milli
    if(now >= _lastTapTime) _tapCount = 0; // if no tap after 10 seconds reset the counter
}

void Clock::calculateDeltaTimeMultiplyed()
{
    _multi[0] = fmod(_dT,0.25) * 4; //0.25
    _multi[1] = fmod(_dT,0.5) * 2; //0.5
    _multi[2] = _dT;
    _multi[3] = (fmod(_stepCounter,2) * 0.5) +_dT*0.5; //2
    _multi[4] = (fmod(_stepCounter,4) * 0.25) +_dT*0.25; //4
    _multi[5] = (fmod(_stepCounter,8) * 0.125) +_dT*0.125; //8
    _multi[6] = (fmod(_stepCounter,16) * 0.0625) +_dT*0.0625; //16
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
    int timeout = 5000000;///5 sec then tap is automaticaly reset
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
