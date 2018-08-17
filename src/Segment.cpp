//
//  Segment.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 06/08/18.
//
//

#include "Segment.hpp"

Segment::Segment(int nodeID,int universe,int beginLed,int endLed,int segmentID):_universe(universe),_begin(beginLed),_end(endLed),_nodeID(nodeID)
{
    _isInverse= false;
    if(_begin<_end) _isInverse= true;
    _pixLength = _end - _begin;
    _begin *= 3;
    _end *= 3;
    _length = (_end - _begin);
    _mid = _begin + ((_end - _begin) / 2);
    _funcSelect = 0;
    for (int i = 0; i < 512; i++)
    {
        _values[i] = 0;//black
    }
    _pixel[0] = &_values[0];
    _pixel[1] = &_values[1];
    _pixel[2] = &_values[2];
}

Segment::~Segment(){}

//every segment hold his position inside the nodes and universe including his array
//this is later coppied to the nodes
//

u_int8_t *Segment::getPixel(int id)
{
    //check range
    if(id >= _pixLength) return *_pixel;
    _pixel[0] = &_values[(id * 3) + 0];
    _pixel[1] = &_values[(id * 3) + 1];
    _pixel[2] = &_values[(id * 3) + 2];
    return *_pixel;
};

void Segment::setPixel(int id,int r,int g,int b)
{
    _values[(id*3) + 0] = r;
    _values[(id*3) + 1] = g;
    _values[(id*3) + 2] = b;
}

void Segment::setPixel(int id,ofColor color)
{
    if(id*3 > _length) return;
    _values[(id*3) + 0] = color.r;
    _values[(id*3) + 1] = color.g;
    _values[(id*3) + 2] = color.b;
}

