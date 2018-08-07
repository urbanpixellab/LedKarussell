//
//  Segment.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 06/08/18.
//  every segment is on line
//  mapped to the node and pixels from begin to end
//  we can then change the mapping from top to bottom
//  or inverse ore from binnen neer buiten

#ifndef Segment_hpp
#define Segment_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxArtnetProtocol.h"

class Segment
{
public:
    Segment(int nodeID,int universe,int begin,int end,int segmentID);
    ~Segment();
    
    u_int8_t *getArray(){return _values;};
    int &getBegin(){return _begin;};
    int &getEnd(){return _end;};
    int &getNodeID(){return _nodeID;};
    int &getUniverse(){return _universe;};
    
    
private:
    int             _universe;//maybe more than on if it has more than one unoverse
    int             _begin;
    int             _mid;
    int             _end;
    int             _funcSelect;
    u_int8_t        _values[512];//max size
    int             _nodeID;
    
};

#endif /* Segment_hpp */
