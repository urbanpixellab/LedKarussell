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
    struct Node
    {
        //artnet node maybe as seperate class
        string                  ip;
        ofxArtnetProtocol       artnet;
        //artnet.begin("192.168.12.200");
        u_int8_t                universes[8][512]; //8 full dm universes
        //u_int8_t                _universes[8][450]; //3*150 led per stripe max
    };

    
    Segment(int universe,int begin,int end, Node * n);
    ~Segment();
    
    void setSegment(Node *n,int universe,int begin = 0,int end = 0);
    void setArray(char *array, int length = 450);// 150 * 3 length
    
private:
    int     _universe;
    int     _begin;
    int     _mid;
    int     _end;
    Node    *_node;// the note to write on
    
};

#endif /* Segment_hpp */
