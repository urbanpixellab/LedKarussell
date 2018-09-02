//
//  Patroon.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 01/09/18.
//
//

#ifndef Patroon_hpp
#define Patroon_hpp

#include <stdio.h>
#include "ofMain.h"

class Patroon
{
public:
    Patroon(int id,int curveA,int curveB,float freqA,float freqB,float dirA, float dirB,int timeA, int timeB,int colorA, int colorB,int colorC, int colorD);
    ~Patroon();
    
    int *getCurve(int id){return &_curve[id];};
    
    int *getSeqA(int id){return _sequenceA[id];};
    int *getSeqB(int id){return _sequenceB[id];};
    
private:
    int             _id;
    int             _curve[2] = {0,0};
    float           _freq[2] = {1,1};
    int             _dir[2] = {0,0};
    int             _time[2] = {0,0};
    int             _color[4] = {0,0,0,0};
    int             _sequenceA[8][13];
    int             _sequenceB[8][13];
    
};
/*
	<layerA>
 <curve>0</curve>
 <cFreq>1</cFreq>
 <cDir>1</cDir>
 <cTime>1</cTime>
 <colorA>0</colorA>
 <colorB>1</colorB>
 <step0>2,3</step0>
 <step1>2,3</step1>
 <step2>2,3</step2>
 <step3>2,3</step3>
 <step4>2,3</step4>
 <step5>2,3</step5>
 <step6>2,3</step6>
 <step7>2,3</step7>
	</layerA>
*/


#endif /* Patroon_hpp */
