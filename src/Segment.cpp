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
    _begin *= 3;
    _end *= 3;
    _mid = _begin + ((_end - _begin) / 2);
    _funcSelect = 0;
    for (int i = 0; i < 512; i++)
    {
        _values[i] = 0;//black
    }
}

Segment::~Segment(){}

//every segment hold his position inside the nodes and universe including his array
//this is later coppied to the nodes
//
