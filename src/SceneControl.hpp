//
//  SceneControl.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 29/07/18.
//  this holds the complete scene from mapping to output
//

#ifndef SceneControl_hpp
#define SceneControl_hpp

#include <stdio.h>
#include "Visualizer.hpp"
#include "MidiControl.hpp"
#include "Colorizer.hpp"

class SceneControl
{
public:
    enum STATES
    {
        PRESET = 0,
        LIVE
    };
    
    enum ANIMATION
    {
        FUNCTION = 0,
        SHADER
    };
    
    SceneControl(MidiControl *mc);
    ~SceneControl();
    
    void update();
    void drawGui();
    
    
    void addListener();
    void removeListener();
    void mousePressed(ofMouseEventArgs & args);
    
private:
    void                updateSequencer();
    MidiControl         *_MC;
    Visualizer          _visualizer;
    Colorizer           _colorizer;
    
    bool                _sceneSelector; // is it a function or a shader
    
    int                 _step;      //the step sequence
    int                 _maxStep;   // the maximum length of steps in the selected pattern
};

#endif /* SceneControl_hpp */
