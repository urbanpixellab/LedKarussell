//
//  Clock.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 25/07/18.
//
//

#ifndef Clock_hpp
#define Clock_hpp

#include <stdio.h>
#include "ofMain.h"


class Clock
{
public:
    Clock();
    ~Clock();
    
    void update();
    void tap();
    string calculateBPM(u_int64_t *data,int length);
    
    float &getValue(){return _actValue;};
    float &getDeltaTime(){return _dT;}; // 0...1
    bool &getBeat(){return _isBeat;};
    string &getBPM(){return _bpm;};

    
    
private:
    float       _dT;//the deltatime in the selected beats/size
    float       _actValue; /// the actual value
    float       _beatLength; //in ms
    float       _beatScale;//sclaes the lenbth of the beat 1/8 1/4 1/2 1 2 4 8 16
    u_int64_t   _lastBeat;// the time in millisec when last beat
    u_int64_t   _nextBeat;// the time in millisec when next beat
    bool        _isBeat;
    string      _bpm;

    ///tap
    int         _tapCount;
    u_int64_t   _lastTapTime;
    u_int64_t   _timer[2];
    
};

#endif /* Clock_hpp */
