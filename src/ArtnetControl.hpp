//
//  ArtnetControl.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//
//

#ifndef ArtnetControl_hpp
#define ArtnetControl_hpp

#include <stdio.h>
#include "ofxArtnetProtocol.h"
#include "SceneControl.hpp"
#include "MidiControl.hpp"

class ArtnetControl
{
public:
    struct NODE
    {
        //artnet node
    };
    
    ArtnetControl(MidiControl *mc,SceneControl *live);
    ~ArtnetControl();
    
    void update();
    void sendToNodes();
    
private:
    MidiControl             *_MC;
    SceneControl            *_Scene; // the actual output scene
};

#endif /* ArtnetControl_hpp */
