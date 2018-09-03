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
    
    void clear();
    
    void nextStep();
    void resetStep(){_step = 0;};
    
    int *getCurve(int id){return &_curve[id];};
    
    bool *getSeqStepA(int id){return _sequenceA[id];};
    bool *getSeqStepB(int id){return _sequenceB[id];};
    
    void setSeqA(int step, int row, bool value){_sequenceA[step][row] = value;};
    void setSeqB(int step, int row, bool value){_sequenceB[step][row] = value;};
    
    void setSeqAColor(string &c);
    void setSeqBColor(string &c);
    
    vector<int> getColorIDs();
    

    void setSeqSel(int layer,int step,int *selection,int length);

private:
    int             _id;
    int             _curve[2] = {0,0};
    float           _freq[2] = {1,1};
    int             _dir[2] = {0,0};
    int             _time[2] = {0,0};
    int             _color[4] = {0,0,0,0};
    bool            _sequenceA[8][13];
    bool            _sequenceB[8][13];
    
    int             _step;
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
