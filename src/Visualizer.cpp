//
//  Visualizer.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//
//

#include "Visualizer.hpp"

Visualizer::Visualizer()
{}

Visualizer::~Visualizer(){}

void Visualizer::initialize()
{
    _fbo.allocate(320,160);
    _fbo.begin();
    ofClear(0,0,0);
    _fbo.end();
}

void Visualizer::updateTexture(ofTexture &tex)
{
    //if we use a shader then we use this one for  the mesh
}

void Visualizer::updateTexture(int &array)
{
    //if we use functions than we draw thes to a texture
}

void Visualizer::draw()
{
    ofPushMatrix();
    ofTranslate(_drawarea.x, _drawarea.y);
    _fbo.begin();
    ofClear(0,0,0);
    //_texture.bind();
    _geometry.draw();
    //_texture.unbind();
    _fbo.end();
    ofPopMatrix();
}
