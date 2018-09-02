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
