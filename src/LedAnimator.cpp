 //
//  LedAnimator.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 07/08/18.
//
//

#include "LedAnimator.hpp"

LedAnimator::LedAnimator(MidiControl * mc,float *masterBright): _MC(mc),_masterBrightness(masterBright)
{
    //only have to been called once;
    // is working on array getting in and function select + parameters like direction and color
    
    _aniSelect = 0; //old
}

LedAnimator::~LedAnimator()
{
}

//////////////////////niew

void LedAnimator::drawToArray(int drawFunction,int dir,int time,float freq,u_int8_t * selectionArrays,int &begin,int &length,ofColor &a,ofColor &b,float &shift)
{
    int blendMode = 0;// direct
    // frequency moet op en draaiknop !!!
    float len = (length/3); // convert to pixel position
    float values[int(len)];
    //implement the color function
    switch (drawFunction) {
        case CURVE::BLACKOUT:
        {
            //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < len; i++) values[i] = 1;
            break;
        }
        case CURVE::WHITEOUT:
        {
            
        //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < len; i++) values[i] = 0;
            break;
        }
        case CURVE::RECT://add frequency in repetition
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float delta = len/freq;
                float value = 0;
                float timeShift = _MC->getDtMulti(time)*delta;
                if(fmod(i+delta+timeShift+shift,delta) > delta/2) value = 1;
                values[i] = value;
            }
            
            break;
        }
        case CURVE::RAMP:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                values[i] = fmod(((shift + i/len) + _MC->getDtMulti(time))*freq,1);
            }
            
            break;
        }
        case CURVE::SAW:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                values[i] = 1.0 - fmod(((shift + i/len) + _MC->getDtMulti(time))*freq,1);
            }
            
            break;
        }
        case CURVE::TRI:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                //have some glitch in the middle
                float value = fmod(((shift + i/len) + _MC->getDtMulti(time))*freq,1) * 2.;
                if(value > 1.0) value = 1.0-value;
                values[i] = value;
            }
            
            break;
        }
        case CURVE::SINE:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                values[i] = (0.5+(sin((shift+(i/len))*TWO_PI*freq + _MC->getDtMulti(time) * TWO_PI))*0.5);
            }
            
            break;
        }
            
        case CURVE::COS:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                values[i] = (0.5+(sin((shift+(i/len))*TWO_PI*freq + _MC->getDtMulti(time) * TWO_PI))*0.5);
            }
            
            break;
        }
            
        default:
            break;
    }
    // now we can work on the directions with the array bevore writing to outputs
    switch (dir) {
        case 1:
            reverseArray(values, int(len));
            break;
            
        case 2:
            BinnenBuitenArray(values, int(len));
            break;
            
            
        case 3:
            BuitenBinnenArray(values, int(len));
            break;
            
            
        default:
            break;
    }
    
    
    for (int i = 0; i < len; i++) //reduce to pixel
    {
        values[i] = pow(values[i], 4);
        selectionArrays[begin + (i * 3) + 0] = std::max(int(a.r * values[i]),int(b.r * (1-values[i]))) * *_masterBrightness;
        selectionArrays[begin + (i * 3) + 1] = std::max(int(a.g * values[i]),int(b.g * (1-values[i]))) * *_masterBrightness;
        selectionArrays[begin + (i * 3) + 2] = std::max(int(a.b * values[i]),int(b.b * (1-values[i]))) * *_masterBrightness;
    }
    
}

void LedAnimator::addToArray(int drawFunction,int dir,int time,float freq,u_int8_t * selectionArrays,int &begin,int &length,ofColor &a,ofColor &b,float &shift)
{
    // frequency moet op en draaiknop !!!
    float len = ((length)/3); // convert to pixel position
    float values[int(len)];
    
    //implement the color function
    switch (drawFunction) {
        case CURVE::BLACKOUT:
        {
            for (int i = 0; i < len; i++) values[i] = 1;
            break;
        }
        case CURVE::WHITEOUT:
        {
            for (int i = 0; i < len; i++) values[i] = 0;
            break;
        }
        case CURVE::RECT://add frequency in repetition
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float delta = len/freq;
                float value = 0;
                float timeShift = _MC->getDtMulti(time)*delta;
                if(fmod(i+delta+timeShift+shift,delta) > delta/2) value = 1;
                values[i] = value;
            }
            
            break;
        }
        case CURVE::RAMP:
        {
            for (int i = 0; i < len; i++) values[i] = fmod(((shift + i/len) + _MC->getDtMulti(time))*freq,1);
            
            break;
        }
        case CURVE::SAW:
        {
            for (int i = 0; i < len; i++) values[i] = 1.0 - fmod(((shift + i/len) + _MC->getDtMulti(time))*freq,1);
            
            break;
        }
        case CURVE::TRI:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                //have some glitch in the middle
                float value = fmod(((shift + i/len) + _MC->getDtMulti(time))*freq,1) * 2.;
                if(value > 1.0) value = 1.0-value;
                values[i] = value;
            }
            
            break;
        }
        case CURVE::SINE:
        {
            for (int i = 0; i < len; i++) values[i] = (0.5+(sin((shift+(i/len))*TWO_PI*freq + _MC->getDtMulti(time) * TWO_PI))*0.5);
            break;
        }
            
        case CURVE::COS:
        {
            for (int i = 0; i < len; i++) values[i] = (0.5+(cos((shift+(i/len))*TWO_PI*freq + _MC->getDtMulti(time) * TWO_PI))*0.5);
            break;
        }
            
        default:
            break;
    }
    // now we can work on the directions with the array
    switch (dir) {
        case 1:
            reverseArray(values, int(len));
            break;
            
        case 2:
            BinnenBuitenArray(values, int(len));
            break;
            
            
        case 3:
            BuitenBinnenArray(values, int(len));
            break;
            
            
        default:
            break;
    }
    
    //now write to array
    for (int i = 0; i < len; i++) //reduce to pixel
    {
        values[i] = pow(values[i], 4);
        selectionArrays[begin + (i * 3) + 0] = std::max(int(a.r * values[i]),int(b.r * (1-values[i]))) * *_masterBrightness;
        selectionArrays[begin + (i * 3) + 1] = std::max(int(a.g * values[i]),int(b.g * (1-values[i]))) * *_masterBrightness;
        selectionArrays[begin + (i * 3) + 2] = std::max(int(a.b * values[i]),int(b.b * (1-values[i]))) * *_masterBrightness;

    }
}

void LedAnimator::maxToArray(int drawFunction,int dir,int time,float freq,u_int8_t * selectionArrays,int &begin,int &length,ofColor &a,ofColor &b,float &shift)
{
    // frequency moet op en draaiknop !!!
    float len = (length/3); // convert to pixel position
    float values[int(len)];
    
    //now write to array
    for (int i = 0; i < len; i++) //reduce to pixel
    {
        // compare the existing to the color, the max color is set
        if (a.r > selectionArrays[begin + (i * 3) + 0]) selectionArrays[begin + (i * 3) + 0] = a.r* *_masterBrightness;
        if (a.g > selectionArrays[begin + (i * 3) + 1]) selectionArrays[begin + (i * 3) + 1] = a.g* *_masterBrightness;
        if (a.b > selectionArrays[begin + (i * 3) + 2]) selectionArrays[begin + (i * 3) + 2] = a.b* *_masterBrightness;
    }
}

void LedAnimator::reverseArray(float *array, int length)
{
    float tmp[length];
    for (int i = 0; i < length; i++)
    {
        tmp[i] = array[length-1-i];
    }
    for (int i = 0; i < length; i++)
    {
        array[i] = tmp[i];
    }
}

void LedAnimator::BinnenBuitenArray(float *array, int length)
{
    float tmp[length*2];
    for (int i = 0; i < length; i++)
    {
        tmp[i] = array[i];
    }
    for (int i = 0; i < length; i++)
    {
        tmp[i+length] = array[length-1-i];
    }
    for (int i = 0; i < length; i++)
    {
        array[i] = tmp[i*2];
    }
}
void LedAnimator::BuitenBinnenArray(float *array, int length)
{
    float tmp[length*2];
    for (int i = 0; i < length; i++)
    {
        tmp[i] = array[length-1-i];
    }
    for (int i = 0; i < length; i++)
    {
        tmp[i+length] = array[i];
    }
    for (int i = 0; i < length; i++)
    {
        array[i] = tmp[i*2];
    }
}

void LedAnimator::invert(u_int8_t * selectionArrays,int &begin,int &length)
{
    for (int i = begin; i < length; i++)
    {
        selectionArrays[i] = (255 - selectionArrays[i])* *_masterBrightness;
    }
}

void LedAnimator::blackout(u_int8_t * selectionArrays,int &begin,int &length)
{
    for (int i = begin; i < length; i++)
    {
        selectionArrays[i] = 0;
    }
}
