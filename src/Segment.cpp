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
}

Segment::~Segment(){}

void Segment::setSegment(Node *n,int universe,int begin,int end)
{
    
}

void Segment::setArray(char *array, int length) // array is 3 for rgb
{
    //mapp the input array to the node array
    u_int8_t mappedValues = (_end - _begin) / 3;
    
    for (int led = _begin; led < _end; led+3){
        _node->universes[_universe][led] = array[led];
    }
}
