//
//  LedAnimator.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 07/08/18.
//
//

#include "LedAnimator.hpp"

LedAnimator::LedAnimator(MidiControl * mc): _MC(mc)
{
    _step = 0;
    _maxStep = 16;
    _aniSelect = 0;
}
LedAnimator::~LedAnimator(){}

void LedAnimator::addStep()
{
    cout << _step << endl;
    _step++;
    if(_step >= _maxStep) _step = 0;
}

void LedAnimator::drawGui()
{
    
}

void LedAnimator::animationToArray(int id,u_int8_t * array,int length,int colorSelect)
{
    switch (_aniSelect)
    {
        case ANIMATIONS::BLACK:
            blackout(id, array, length);
            break;

        case ANIMATIONS::WHITE:
            whiteout(id, array, length);
            break;
            
        case ANIMATIONS::FADE_IN:
            fade(id, array, length,false);
            break;
            
        case ANIMATIONS::FADE_OUT:
            fade(id, array, length,true);
            break;
            
        default: // default is black
            blackout(id, array, length);
            break;
    }
}


void LedAnimator::blackout(int &id,u_int8_t * array,int &length)
{
    for (int i = 0; i < length; i++)
    {
        array[i] = 0;
    }
}

void LedAnimator::whiteout(int &id,u_int8_t * array,int &length)
{
    for (int i = 0; i < length; i++)
    {
        array[i] = 255;
    }
}


void LedAnimator::fade(int &id,u_int8_t * array,int &length,bool direction)
{
    for (int i = 0; i < length; i++)
    {
        array[i] = abs(direction-_MC->getDt()) * 255.;
    }
}
