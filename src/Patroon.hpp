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
#include "Button.hpp"

class Patroon
{
public:
    
    //Patroon(int id,int curveA,int curveB,float freqA,float freqB,float dirA, float dirB,int timeA, int timeB,int colorA, int colorB,int colorC, int colorD);
    
    Patroon(int id,int curveA,int curveB,float freqA,float freqB,float dirA, float dirB,int timeA, int timeB,int colorA, int colorB,int colorC, int colorD, int curvePhaseA, int curvePhaseB, float freqPhaseA, float freqPhaseB);
    ~Patroon();
    
    void clear();
    void nextStep();
    void resetStep(){_step = 0;};
    int &getID(){return _id;};
    int *getCurve(int id){return &_curve[id];};
    void toggleVisibility(bool mode);
    bool *getSeqStepA(int id){return _sequenceA[id];};
    bool *getSeqStepB(int id){return _sequenceB[id];};
    
    int *getFreq(int id){return &_freq[id];};
    int *getDir(int id){return &_dir[id];};
    int *getTime(int id){return &_time[id];};
    int *getPhase(int id){return &_curvePhase[id];};
    float *getPhaseFreq(int id){return &_freqPhase[id];};
    
    void setSeqA(int step, int row, bool value){_sequenceA[step][row] = value;};
    void setSeqB(int step, int row, bool value){_sequenceB[step][row] = value;};
    
    void setCurve(int id,int value){_curve[id] = value;};
    
    vector<int> getColorIDs();
    int &getColorID(int id){return _color[id];};
    
    void setColors(int * array,int length);
    void setSeqSel(int layer,int step,int *selection,int length);
    
    // DEBUG
    void printPatroon();
    void printMatrix();
    

private:
    int             _id;
    int             _curve[2] = {0,0};
    int             _freq[2] = {1,1};
    int             _dir[2] = {0,0};
    int             _time[2] = {0,0};
    int             _color[4] = {0,0,0,0};
    bool            _sequenceA[8][14];
    bool            _sequenceB[8][14];
    int             _curvePhase[2] = {0,0};
    float           _freqPhase[2] = {0,0};
    int             _step;
    

};
#endif /* Patroon_hpp */
