//
//  Colorizer.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//  Single colors and color settings to use in the scene
//  color schemes base on https://color.adobe.com/explore/?filter=most-popular&time=month

#ifndef Colorizer_hpp
#define Colorizer_hpp

#include <stdio.h>
#include "ofMain.h"

class Colorizer
{
public:
    enum PRIMARY_COLOR
    {
        BLACK = 0,
        WHITE,
        RED,
        ORANGE,
        GREEN,
        CYAN,
        BLUE,
        PINK,
        PURPLE
    };
    
    Colorizer();
    ~Colorizer();
    
    
    ofColor &getPrimColor(int id);
    
    
private:
    ofColor         primaryColors[9] = {ofColor(0,0,0),ofColor(255,255,255),ofColor(255,0,0),ofColor(255,128,0),ofColor(0,255,0),ofColor(0,128,255),ofColor(0,0,255),ofColor(255,0,255),ofColor(128,0,255)};
    
};

#endif /* Colorizer_hpp */
