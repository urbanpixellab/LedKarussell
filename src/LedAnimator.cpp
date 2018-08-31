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
    //only have to been called once;
    // is working on array getting in and function select + parameters like direction and color
    
    _aniSelect = 0; //old
    //load the selections from xml and create buttons for every selection with the name and id
    ofxXmlSettings sel("selector.xml");
    for (int i = 0; i < sel.getNumTags("select"); i++)
    {
        sel.pushTag("select",i);
        Selection s;
        s.name = sel.getValue("name", "untitled");
        string segments = sel.getValue("segment", "");
        vector<string> segmentSelection = ofSplitString(segments, ",");
        for (int seg = 0; seg < segmentSelection.size(); seg++)
        {
            s.items.push_back(ofToInt(segmentSelection[seg]));
        }
        _selections.push_back(s);
        sel.popTag();
    }
}

LedAnimator::~LedAnimator()
{
}

//////////////////////niew

void LedAnimator::drawToArray(int drawFunction,int drawMode,float freq,u_int8_t * selectionArrays,int &length,ofColor &a)
{
    int blendMode = 0;// direct
    // frequency moet op en draaiknop !!!
    float len = (length/3); // convert to pixel position
    //implement the color function
    switch (drawFunction) {
        case CURVE::BLACKOUT:
        {
            //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < length; i++)
            {
                selectionArrays[i] = 0;
            }
            
            break;
        }
        case CURVE::WHITEOUT:
        {
            
        //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < length; i++)
            {
                selectionArrays[i] = 255;
            }
            
            break;
        }
        case CURVE::RECT://add frequency in repetition
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float delta = len/freq;
                float value = 0;
                float shift = _MC->getDt()*delta;
                if(fmod(i+delta+shift,delta) > delta/2) value = 1;
                selectionArrays[(i * 3) + 0] = a.r * value;
                selectionArrays[(i * 3) + 1] = a.g * value;
                selectionArrays[(i * 3) + 2] = a.b * value;
            }
            
            break;
        }
        case CURVE::RAMP:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float value = fmod(((i/len) + _MC->getDt())*freq,1);
                selectionArrays[(i * 3) + 0] = a.r * value;
                selectionArrays[(i * 3) + 1] = a.g * value;
                selectionArrays[(i * 3) + 2] = a.b * value;
            }
            
            break;
        }
        case CURVE::SAW:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float value = 1.0 - fmod(((i/len) + _MC->getDt())*freq,1);
                selectionArrays[(i * 3) + 0] = a.r * value;
                selectionArrays[(i * 3) + 1] = a.g * value;
                selectionArrays[(i * 3) + 2] = a.b * value;
            }
            
            break;
        }
        case CURVE::TRI:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                //have some glitch in the middle
                float value = fmod(((i/len) + _MC->getDt())*freq,1) * 2.;
                if(value > 1.0) value = 1.0-value;
                selectionArrays[(i * 3) + 0] = a.r * value;
                selectionArrays[(i * 3) + 1] = a.g * value;
                selectionArrays[(i * 3) + 2] = a.b * value;
            }
            
            break;
        }
        case CURVE::SINE:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float value = (0.5+(sin((i/len)*TWO_PI*freq + _MC->getDt() * TWO_PI))*0.5);
                
                selectionArrays[(i * 3) + 0] = a.r * value;
                selectionArrays[(i * 3) + 1] = a.g * value;
                selectionArrays[(i * 3) + 2] = a.b * value;
            }
            
            break;
        }
            
        default:
            break;
    }
}

void LedAnimator::addToArray(int drawFunction,int drawMode,float freq,u_int8_t * selectionArrays,int &length,ofColor &a)
{
    // frequency moet op en draaiknop !!!
    float len = (length/3); // convert to pixel position
    //implement the color function
    switch (drawFunction) {
        case CURVE::BLACKOUT:
        {
            //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < length; i++)
            {
                selectionArrays[i] = 0;
            }
            
            break;
        }
        case CURVE::WHITEOUT:
        {
            
            //sinewave on array with freq = 3 on length of input array
            //
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                selectionArrays[(i * 3) + 0] += a.r;
                selectionArrays[(i * 3) + 1] += a.g;
                selectionArrays[(i * 3) + 2] += a.b;
            }
            
            
            break;
        }
        case CURVE::RECT://add frequency in repetition
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float delta = len/freq;
                float value = 0;
                float shift = _MC->getDt()*delta;
                if(fmod(i+delta+shift,delta) > delta/2) value = 1;
                selectionArrays[(i * 3) + 0] += a.r * value;
                selectionArrays[(i * 3) + 1] += a.g * value;
                selectionArrays[(i * 3) + 2] += a.b * value;
            }
            
            break;
        }
        case CURVE::RAMP:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float value = fmod(((i/len) + _MC->getDt())*freq,1);
                selectionArrays[(i * 3) + 0] += a.r * value;
                selectionArrays[(i * 3) + 1] += a.g * value;
                selectionArrays[(i * 3) + 2] += a.b * value;
            }
            
            break;
        }
        case CURVE::SAW:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float value = 1.0 - fmod(((i/len) + _MC->getDt())*freq,1);
                selectionArrays[(i * 3) + 0] += a.r * value;
                selectionArrays[(i * 3) + 1] += a.g * value;
                selectionArrays[(i * 3) + 2] += a.b * value;
            }
            
            break;
        }
        case CURVE::TRI:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                //have some glitch in the middle
                float value = fmod(((i/len) + _MC->getDt())*freq,1) * 2.;
                if(value > 1.0) value = 1.0-value;
                selectionArrays[(i * 3) + 0] += a.r * value;
                selectionArrays[(i * 3) + 1] += a.g * value;
                selectionArrays[(i * 3) + 2] += a.b * value;
            }
            
            break;
        }
        case CURVE::SINE:
        {
            for (int i = 0; i < len; i++) //reduce to pixel
            {
                float value = (0.5+(sin((i/len)*TWO_PI*freq + _MC->getDt() * TWO_PI))*0.5);
                
                selectionArrays[(i * 3) + 0] += a.r * value;
                selectionArrays[(i * 3) + 1] += a.g * value;
                selectionArrays[(i * 3) + 2] += a.b * value;
            }
            
            break;
        }
            
        default:
            break;
    }
}