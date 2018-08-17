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
    u_int8_t *getPixel(int id);
    void setPixel(int id, int r, int g, int b);
    void setPixel(int id, ofColor color);
    u_int8_t getPixelLength(){return _pixLength;};
    
    int &getBegin(){return _begin;};
    int &getEnd(){return _end;};
    int &getNodeID(){return _nodeID;};
    int &getUniverse(){return _universe;};
    int &getLength(){return _length;};
    bool &getInverse(){return _isInverse;};
    
    
private:
    int             _universe;//maybe more than on if it has more than one unoverse
    int             _begin;
    int             _mid;
    int             _end;
    int             _length; // the used array length
    int             _funcSelect;
    u_int8_t        _values[512];//max size
    u_int8_t        *_pixel[3];
    u_int8_t        _pixLength;
    int             _nodeID;
    bool            _isInverse;//ins it inverse or not, based on the input led
    
};

#endif /* Segment_hpp */
