//
//  ArtnetControl.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//
//

#include "ArtnetControl.hpp"

ArtnetControl::ArtnetControl(MidiControl *mc):_MC(mc)
{
    _GUI = new AnimatorGUI(ofRectangle(100,100,500,500));
    _GUI->createAnimationGUI(LedAnimator::COUNT);
    ofAddListener(_GUI->buttonPressed, this, &ArtnetControl::guiPressed);
    _preAnimator = new LedAnimator(_MC);
    _LiveAnimator = new LedAnimator(_MC);
    loadNodes();
}

ArtnetControl::~ArtnetControl()
{
    // blackout all
    ofRemoveListener(_GUI->buttonPressed, this, &ArtnetControl::guiPressed);
    clearNodes();
    delete _preAnimator;
    delete _LiveAnimator;
    for (int i = 0; i < _liveSegments.size(); i++)
    {
        delete _preSegments[i];
        delete _liveSegments[i];
    }
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
        Node *n = new Node();
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
    
    _preSegments.clear();
    _liveSegments.clear();
    for (int i = 0; i < 34; i++)
    {
        //int universe,int begin,int end, Node * node
        //or settings based on an xml from the settings
        int node = 0;
        int universe = 0;
        //int nodeID,int universe,int beginLed,int endLed,int segmentID
        Segment *newSegP = new Segment(node,universe,0,150,i);
        Segment *newSegL = new Segment(node,universe,0,150,i);
        _preSegments.push_back(newSegP);
        _liveSegments.push_back(newSegL);
    }
    //whiteout
    _preAnimator->setAnimation(2);
    _LiveAnimator->setAnimation(2);
}

void ArtnetControl::update()
{
    // update artnet based on the selected sce or use the hottbutton function
    // which is overriding the actual state
    // also use the delta time and next beat from the mmidi controler
    // to run through the step sequences from the on off function

    if(_MC->getBeat() == true)
    {
        _preAnimator->addStep();
        _LiveAnimator->addStep();
    }
    //fill all nodes by segment
    for (int i = 0; i < _liveSegments.size(); i++)
    {
        //writeSegment(the id of the segment from left to right,char[150]); these are the max per stripe
        //_segments[i]->setArrayByArray(rgb);
        //or
        _preAnimator->animationToArray(i, _preSegments[i]->getArray(), _preSegments[i]->getLength(), 0);
        _LiveAnimator->animationToArray(i, _liveSegments[i]->getArray(), _liveSegments[i]->getLength(), 0);
    }
    sendToNodes();
}

void ArtnetControl::setAnimation(int id)
{
    _preAnimator->setAnimation(id);
    _LiveAnimator->setAnimation(id);
}

void ArtnetControl::drawGui()
{
    // draw som buttons from a controler gui class
    _GUI->draw();
}


void ArtnetControl::sendToNodes()
{
    //basic sending
    //int universe = 0;
    //int chnCount = 450;//33 leds * 3
    //artnet.send(universe1,universe,chnCount);

    //send all nodes but only from live
    for (int i = 0; i < _liveSegments.size(); i++)
    {
            _nodes[_liveSegments[i]->getNodeID()]->artnet.send(_liveSegments[i]->getArray(),_liveSegments[i]->getUniverse(),512);
    }

}

void ArtnetControl::guiPressed(int &buttonid)
{
    setAnimation(buttonid);
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
