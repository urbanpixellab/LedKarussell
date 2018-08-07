//
//  Segment.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 06/08/18.
//
//

#include "Segment.hpp"

Segment::Segment(int universe,int begin,int end, Node * node):_universe(universe),_begin(begin),_end(end),_node(node)
{
    _begin *= 3;
    _end *= 3;
    _mid = _begin + ((_end - _begin) / 2);
    _funcSelect = 0;
}

Segment::~Segment(){}

void Segment::setSegment(Node *n,int universe,int begin,int end)
{
    // for the mappin
    // select a node and the universe to slide from begin to end
}

void Segment::setArrayByArray(u_int8_t *arrayIn) // 450 valuesarray is 3 for rgb
{
    //mapp the input array to the node array
    
    
    for (int led = _begin; led < _end; led+3)
    {
        _node->universes[_universe][led] = arrayIn[led];
    }
}

void Segment::setArrayByFunction(int select, float dt,int colorSelect) // set it by drawing function is 3 for rgb with color select
{
    //mapp the input array to the node array
    //can be stay in another class easier to edit!!
    if(_end - _begin < 0) // check for direction, must been checked not sur !!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        u_int8_t tmp = _begin;
        _begin = _end;
        _end = _begin;
    }
    
    int length = (_end - _begin)/3;
    switch (_funcSelect= FUNCTIONS::INVERT)
    {
        case FUNCTIONS::BLINK:
        {
            //blink halfstep
            u_int8_t ledvalue[length];
            for (int led = _begin; led < _end; led++)
            {
                if(dt < 0.5) _node->universes[_universe][led] = 255;
                else _node->universes[_universe][led] = 0;
            }
            break;
        }
        case FUNCTIONS::SIN01:
        {
            u_int8_t ledvalue[length];
            
            for (int led = _begin; led < _end; led++)
            {
                _node->universes[_universe][led] = colorSelect*sin(dt* TWO_PI + led);
            }
            break;
        }
            
        case FUNCTIONS::SIN02:
        {
            u_int8_t ledvalue[length];
            
            for (int led = _begin; led < _end; led++)
            {
                _node->universes[_universe][led] = 255*(led / float(length)) * (0.5 * sin(dt*TWO_PI+_universe) + 0.5);//colorSelect*sin(dt* TWO_PI + led+_universe);
            }
            break;
        }
            
        case FUNCTIONS::WHITE_FLASH:
        {
            u_int8_t ledvalue[length];
            
            for (int led = _begin; led < _end; led++)
            {
                
                _node->universes[_universe][led] = 255 * (1.0 - dt);
            }
            break;
        }
            
        case FUNCTIONS::INVERT:
        {
            u_int8_t ledvalue[length];
            
            for (int led = _begin; led < _end; led++)
            {
                _node->universes[_universe][led] = 255 - sin(ofGetElapsedTimef() +_universe) * 255;// = 255*(led / float(length)) * (0.5 * sin(dt*TWO_PI+_universe) + ;//colorSelect*sin(dt* TWO_PI + led+_universe);
            }
            break;
        }
            
        default:
            break;
    }
/*
*/
    //blink halfstep
    
}
