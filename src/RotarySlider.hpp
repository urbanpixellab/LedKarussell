//
//  RotarySlider.hpp
//  Modulator
//
//  Created by Enrico Becker on 27/08/16.
//
//

#ifndef RotarySlider_hpp
#define RotarySlider_hpp

#include <stdio.h>
#include "ofMain.h"

class RotarySlider
{
public:
    RotarySlider();
    RotarySlider(ofRectangle area,ofVec2f minMax,float startValue,bool type,int isInt,int resolution, string n);
    ~RotarySlider();
    
    void setup(ofRectangle area,ofVec2f minMax,float startValue,bool type);
    void setup(ofRectangle area,ofVec2f minMax,float startValue,bool type,int commaSize,int resolution);
    void setup(ofRectangle area,ofVec2f minMax,float startValue,bool type,int commaSize);
    void updateSlider();// the mapped range
    void setResolution(int res){resolution = res;};
    void draw();
    void reset();
    void mouseDragged(ofMouseEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    ofVec2f &getRange(){return range;};
    float &getValue(){return value;};
    ofRectangle &getDrawArea(){return drawArea;};
    bool &getActive(){return active;};
    void activate(){active = true;};
    void deactivate(){active = false;};
    ofEvent<bool>  newValue;
    void setName(string n){name = n;};
    float getValueMapped(){return valueMapped;}
    int getValueInt(){return valueMapped;};
    void setValueMapped(float value);
    void addBezeichner(string * bez);
  
private:
    bool            active;
    string          name;
    ofRectangle     drawArea;
    ofFbo           fbo;
    float           radius;
    float           mouseValue;
    float           value;// 0...1
    float           valueMapped;// 0...1
    float           resetValue;
    ofVec2f         circleCenter;
    ofVec2f         range;
    float           xStart;
    bool            redraw;
    bool            type;//line drawing or mesh
    bool            _isInt;//int slider with steps or float
    ofTrueTypeFont  font;
    int             resolution;// resolution for mouse movement
    int             comma;
    ofPolyline      _lineGraphics;//if we want to add some line graphics_
    vector<string>  _bezeichner;
};

#endif /* RotarySlider_hpp */
