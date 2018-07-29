//
//  Visualizer.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//  take the outputmapping as a texture
//  and stick them on simple planes which

#ifndef Visualizer_hpp
#define Visualizer_hpp

#include <stdio.h>
#include "ofMain.h"

class Visualizer
{
public:
    Visualizer();
    ~Visualizer();

    void initialize();
    void updateTexture(ofTexture &tex);
    void updateTexture(int &array);
    void draw();
    
private:
    ofRectangle         _drawarea;
    ofMesh              _geometry;
    ofFbo               _fbo;
    ofTexture           _texture;
};


#endif /* Visualizer_hpp */
