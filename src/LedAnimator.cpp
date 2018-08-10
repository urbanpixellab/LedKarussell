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
    _enableMode = 1;
}
LedAnimator::~LedAnimator(){}

void LedAnimator::setSegmentSize(int size)
{
    _enables.clear();
    for (int i = 0; i < size; i++)
    {
        _enables.push_back(false);
    }
}

void LedAnimator::setEnableMode(int mode)
{
    _enableMode = mode;
    _step = 0;
    //set the lenth of the step sequence only by usefull ones otherwise it is 16
    switch (_enableMode)
    {
        case ENABLES::KNIT_RIDER:
            _maxStep = _enables.size();
            break;
            
        default:
            _maxStep = 16;
            break;
    }
    updateEnable();
}

void LedAnimator::updateEnable()
{
    switch (_enableMode)
    {
        case ENABLES::ALL_OFF:
            disableAll();
            break;
            
        case ENABLES::ALL_ON:
            enableAll();
            break;
            
        case ENABLES::RANDOM:
            enableRandom();
            break;
            
        case ENABLES::KNIT_RIDER:
            knitRider();
            break;
            
        default:
            break;
    }
}

void LedAnimator::addStep()
{
    _step++;
    if(_step >= _maxStep) _step = 0;
    // also update the enable states
    updateEnable();
}

void LedAnimator::drawGui()
{
    
}

void LedAnimator::animationToArray(int id,u_int8_t * array,int length,int colorSelect)
{
    //_col.getPrimColor(colorSelect);
    // what is the best way to combine it
    //check the enable state
    if(_enables[id] == false)
    {
        blackout(id, array, length);
        return;
    }
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
            
        case ANIMATIONS::SMOOTH_FADE:
            smoothFade(id, array, length,true);
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

void LedAnimator::smoothFade(int &id,u_int8_t * array,int &length,bool direction)
{
    for (int i = 0; i < length; i++)
    {
        array[i] = (0.5 + sin(_MC->getDt()* TWO_PI) * 0.5) * 255.;
    }
}

/////////////////enabler maybe a seperate class

void LedAnimator::enableAll()
{
    for (int i = 0; i < _enables.size(); i++)
    {
        _enables[i] = true;
    }
}

void LedAnimator::disableAll()
{
    for (int i = 0; i < _enables.size(); i++)
    {
        _enables[i] = false;
    }
}

void LedAnimator::enableRandom()
{
    for (int i = 0; i < _enables.size(); i++)
    {
        if(ofRandom(10) < 5) _enables[i] = false;
        else _enables[i] = true;
        cout << i << " " << _enables[i]<< endl;
    }
}

void LedAnimator::knitRider()
{
    for (int i = 0; i < _enables.size(); i++)
    {
        _enables[i] = false;
        if(i == _step) _enables[i] = true;
    }
}



