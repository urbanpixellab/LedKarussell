//
//  Colorizer.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//
//

#include "Colorizer.hpp"

Colorizer::Colorizer()
{
}

Colorizer::~Colorizer(){}

ofColor &Colorizer::getPrimColor(int id)
{
    if (id < 0 || id > 8) id  = 0;
    return primaryColors[id];
}
