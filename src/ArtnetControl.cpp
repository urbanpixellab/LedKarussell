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
    _GUI = new AnimatorGUI(ofRectangle(200,200,500,500));
    _GUI->createAnimationGUI(LedAnimator::ANIMATION_COUNT,LedAnimator::ENABLE_COUNT);
    ofAddListener(_GUI->animationPressed, this, &ArtnetControl::guiAnimationPressed);
    ofAddListener(_GUI->enablePressed, this, &ArtnetControl::guiEnablePressed);
    _preAnimator = new LedAnimator(_MC);
    _liveAnimator = new LedAnimator(_MC);
    loadNodes();
    //init the gui
    _GUI->init();
}

ArtnetControl::~ArtnetControl()
{
    // blackout all
    ofRemoveListener(_GUI->animationPressed, this, &ArtnetControl::guiAnimationPressed);
    ofRemoveListener(_GUI->enablePressed, this, &ArtnetControl::guiEnablePressed);
    clearNodes();
    delete _preAnimator;
    delete _liveAnimator;
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
    //important for the mapping a segment cant go from one univcerse to another,
    //otherwise its is getting complicated!!!!
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
        int begin = 0;
        int length = ofRandom(150); // should be max 150
        Segment *newSegP = new Segment(node,universe,begin,length,i);
        Segment *newSegL = new Segment(node,universe,begin,length,i);
        _preSegments.push_back(newSegP);
        _liveSegments.push_back(newSegL);
    }
    // update the ledanimator size
    //whiteout
    //also create the preview images
    _preIMG.allocate(170, _preSegments.size(),OF_IMAGE_COLOR); // max of 510 channels
    _liveIMG.allocate(170, _preSegments.size(),OF_IMAGE_COLOR);
    _preIMG.setColor(ofColor(0,0,0));
    _liveIMG.setColor(ofColor(0,0,0));
    // fill the images black!!!
    _preAnimator->setAnimation(2);
    _preAnimator->setSegmentSize(_preSegments.size());
    _liveAnimator->setAnimation(2);
    _liveAnimator->setSegmentSize(_liveSegments.size());
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
        _liveAnimator->addStep();
    }
    //fill all nodes by segment
    for (int i = 0; i < _liveSegments.size(); i++)
    {
        _preAnimator->animationToArray(i, _preSegments[i]->getArray(), _preSegments[i]->getLength(), 0);
        _liveAnimator->animationToArray(i, _liveSegments[i]->getArray(), _liveSegments[i]->getLength(), 0);
    }
    // now updaste the arrays to the visualizer
    writeSegmentsToImage(_preSegments, _preIMG);
    writeSegmentsToImage(_liveSegments, _liveIMG);
    
    sendToNodes();
}

void ArtnetControl::writeSegmentsToImage(vector<Segment*> seg, ofImage& img)
{
    /// every segments get his own line
    for (int i = 0; i < seg.size(); i++)
    {
        int start = seg[i]->getBegin(); // is not in led is in array
        for (int l = 0; l < seg[i]->getLength()/3; l++)
        {
            int x = (start / 3) + l;
            int r = seg[i]->getArray()[start + (l*3) + 0];
            int g = seg[i]->getArray()[start + (l*3) + 1];
            int b = seg[i]->getArray()[start + (l*3) + 2];
            ofColor c = ofColor(r,g,b);//seg[i]->getArray()
            img.setColor(x, i, c);
        }
    }
    img.update();
}


void ArtnetControl::drawGui()
{
    // draw som buttons from a controler gui class
    _GUI->draw(_preIMG,_liveIMG);
}


void ArtnetControl::sendToNodes()
{
    //copy segment arrays to nodes
    int nodeID,universe,first,last = 0;
    for (int i = 0; i < _liveSegments.size(); i++)
    {
        nodeID = _liveSegments[i]->getNodeID();
        universe = _liveSegments[i]->getUniverse();
        first = _liveSegments[i]->getBegin();
        last = _liveSegments[i]->getEnd();
        for (int cell = first; cell < last; cell++)
        {
            _nodes[nodeID]->universes[universe][cell] = _liveSegments[i]->getArray()[first+cell];
        }
    }
    //then send the nodes
    //basic sending
    //int universe = 0;
    //int chnCount = 450;//33 leds * 3
    //artnet.send(universe1,universe,chnCount);
    for (int n = 0; n < _nodes.size(); n++)
    {
        for (int u = 0; u < 8; u++)
        {
            // we set the length to 3x170 = 510 has to been tested
            _nodes[n]->artnet.send(_nodes[n]->universes[u],u,510);
        }
    }
}

void ArtnetControl::guiAnimationPressed(int &buttonid)
{
    setAnimationPreview(buttonid);
    //only by now is later automated and hidden
    setAnimationLive(buttonid);
}

void ArtnetControl::guiEnablePressed(int &buttonid)
{
    setEnablePreview(buttonid);
    //only by now is later automated and hidden
    setEnableLive(buttonid);
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
