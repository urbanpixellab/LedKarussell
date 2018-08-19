#pragma once

#include "MidiControl.hpp"
#include "ArtnetControl.hpp"
#include "RotarySlider.hpp"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    RotarySlider rl;
    

    void exit();
private:
    MidiControl     _MC;// the midi control including clock
    
    ArtnetControl   *_AC;// the artnetcontrol
};


/*
 only the prepare scenes can be loaded by preset
 
 */
