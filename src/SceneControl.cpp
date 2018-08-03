//
//  SceneControl.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//
//

#include "SceneControl.hpp"

SceneControl::SceneControl(MidiControl *mc):_MC(mc)
{
    _sceneSelector = 0;//init with function
}

SceneControl::~SceneControl()
{
    //send a blackout to all artnet nodes...?
}

void SceneControl::update()
{
    updateSequencer();
    //do things base on the live scene and the step
//    _visualizer.updateTexture(<#ofTexture &tex#>)
//      also send to artnet
//      but only the live one
}

void SceneControl::updateSequencer()
{
    if (!_MC->getBeat()) return;
    _step++;
    if(_step >= _maxStep) _step = 0;
}


void SceneControl::drawGui()
{
    _visualizer.draw();
}

void SceneControl::addListener()
{
    // when thi ios an live ore preset element then add the listener
    // else if it is a stored presed dont add a listner
    ofAddListener(ofEvents().mousePressed, this, &SceneControl::mousePressed);
}

void SceneControl::removeListener()
{
    ofRemoveListener(ofEvents().mousePressed, this, &SceneControl::mousePressed);
}

void SceneControl::mousePressed(ofMouseEventArgs & args)
{
    
}
