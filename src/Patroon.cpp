//
//  Patroon.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 01/09/18.
//
//

#include "Patroon.hpp"

/*
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
 */

Patroon::Patroon(int id,int curveA,int curveB,float freqA,float freqB,float dirA, float dirB,int timeA, int timeB,int colorA, int colorB,int colorC, int colorD, int curvePhaseA, int curvePhaseB, float freqPhaseA, float freqPhaseB)
{
    _id = id;
    
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
    
    _curvePhase[0] = curvePhaseA;
    _curvePhase[1] = curvePhaseB;
    
    _freqPhase[0] = freqPhaseA;
    _freqPhase[1] = freqPhaseB;
    
    
    clear();

}

Patroon::~Patroon(){}

void Patroon::clear()
{
    for (int step = 0; step < 8; step++)
    {
        for (int row = 0; row < 14; row++)
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

void Patroon::printPatroon()
{
    cout << "Patroon id: " << _id << endl;
    cout << "curve: " << _curve[0] << "," << _curve[1] << endl;
    cout << "freq: " << _freq[0] << "," << _freq[1] <<endl;
    cout << "dir: " << _dir[0] << "," << _dir[1] << endl;
    cout << "time: " << _time[0] << "," << _time[1] << endl;
    cout << "colour: " << _color[0] << "," << _color[1] << "," << _color[2] << "," << _color[3] << endl;
    cout << "phaseCurve: " << _curvePhase[0] << "," << _curvePhase[1] << endl;
    cout << "phasefreq: " << _freqPhase[0] << "," << _freqPhase[1] << endl;
    cout << "-------------------------------------" << endl;
}

void Patroon::printMatrix() {
    cout << "Segment Matrix PatternA:" << endl;
    for(int i = 0; i < 14;i++ )
    {
        cout << i << " --> ";
        for(int j = 0; j < 8;j++ )
        {
           cout << _sequenceA[j][i] << " ";
        }
        cout << endl;
    }
    
    cout << "Segment Matrix PatternB:" << endl;
    for(int i = 0; i < 14;i++ )
    {
        cout << i << " --> ";
        for(int j = 0; j < 8;j++ )
        {
            cout << _sequenceB[j][i] << " ";
        }
        cout << endl;
    }
}





