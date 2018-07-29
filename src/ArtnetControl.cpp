//
//  ArtnetControl.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//
//

#include "ArtnetControl.hpp"

ArtnetControl::ArtnetControl(MidiControl *mc,SceneControl *live):_MC(mc),_Scene(live)
{
    loadNodes();
}

ArtnetControl::~ArtnetControl()
{
    // blackout all
    clearNodes();
}

void ArtnetControl::clearNodes()
{
    for(int i = 0;i < _nodes.size();i++)
    {
        delete _nodes[i];
    }
    _nodes.clear();
}

void ArtnetControl::loadNodes()
{
    clearNodes();
    //load nodes from xml
    Node *n = new Node();
    n->_ip = "192.168.12.200";
    n->_artnet.begin("192.168.12.200");
    //fill universes black
    for (int u = 0; u < 8; u++)
    {
        for (int l = 0; l < 450; l++)
        {
            n->_universes[u][l] = 0;
        }
    }
    _nodes.push_back(n);
}

void ArtnetControl::update()
{
    // update artnet based on the selected sce or use the hottbutton function
    // which is overriding the actual state
    // also use the delta time and next beat from the mmidi controler
    // to run through the step sequences from the on off function
}

void ArtnetControl::sendToNodes()
{
    //basic sending
    //int universe = 0;
    //int chnCount = 450;//33 leds * 3
    //artnet.send(universe1,universe,chnCount);

}

void ArtnetControl::specialFunction(int id)
{
    if(id == 0)
    {
        //flash everything in white
    }
    else if(id  == 1)
    {
        //invert the output colors
    }
}
