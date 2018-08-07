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
    for (int i = 0; i < 1; i++)
    {
        Segment::Node *n = new Segment::Node();
        string ip = "192.168.12."+ ofToString(100+i);
        n->ip = ip;
        n->artnet.begin(ip.c_str());
        //fill universes black
        for (int u = 0; u < 8; u++)
        {
            for (int l = 0; l < 512; l++)
            {
                n->universes[u][l] = 0;
            }
        }
        _nodes.push_back(n);
    }
    // now load the segments
    
    _segments.clear();
    for (int i = 0; i < 34; i++)
    {
        //int universe,int begin,int end, Node * node
        //or settings based on an xml from the settings
        Segment *newSeg = new Segment(0,0,150,_nodes[0]);
        _segments.push_back(newSeg);
    }
}

void ArtnetControl::update()
{
    // update artnet based on the selected sce or use the hottbutton function
    // which is overriding the actual state
    // also use the delta time and next beat from the mmidi controler
    // to run through the step sequences from the on off function

    //fill all nodes by segment
    u_int8_t rgb[450] = {0};
    for (int i = 0; i < 450; i++)
    {
        rgb[i] = ofRandom(255);
    }
    
    for (int i = 0; i < _segments.size(); i++)
    {
        //writeSegment(the id of the segment from left to right,char[150]); these are the max per stripe
        //_segments[i]->setArrayByArray(rgb);
        //or
        _segments[i]->setArrayByFunction(0, _MC->getDt(), i);//color select
        
    }
}

void ArtnetControl::sendToNodes()
{
    //basic sending
    //int universe = 0;
    //int chnCount = 450;//33 leds * 3
    //artnet.send(universe1,universe,chnCount);

    //send all nodes
    for (int i = 0; i < _nodes.size(); i++)
    {
        
        for (int j = 0; j < 8; j++)
        {
            _nodes[i]->artnet.send(_nodes[i]->universes[j],j,450);
        }
    }

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
