//
//  Patroon.hpp
//  LedKarussell
//
//  Created by Enrico Becker on 01/09/18.
//
//

#ifndef Patroon_hpp
#define Patroon_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ColorSelector.hpp"

class Patroon
{
public:
    struct Button
    {
        int id;
        string name;
        ofRectangle area;
        ofColor color;
        bool toggle;
        bool isPressed;
    };

    Patroon(int id,int curveA,int curveB,float freqA,float freqB,float dirA, float dirB,int timeA, int timeB,int colorA, int colorB,int colorC, int colorD);
    ~Patroon();
    
    void clear();
    void createGui();
    void drawGui();
    void nextStep();
    void resetStep(){_step = 0;};
    int &getID(){return _id;};
    int *getCurve(int id){return &_curve[id];};
//    void toggleVisibility(bool mode);
    bool *getSeqStepA(int id){return _sequenceA[id];};
    bool *getSeqStepB(int id){return _sequenceB[id];};
    void setVisible();
    void setInvisible();
    
    float *getFreq(int id){return &_freq[id];};
    
    void setSeqA(int step, int row, bool value){_sequenceA[step][row] = value;};
    void setSeqB(int step, int row, bool value){_sequenceB[step][row] = value;};
    
    void setSeqAColor(string &c);
    void setSeqBColor(string &c);
    
    void setCurve(int id,int value){_curve[id] = value;};
    
    vector<int> getColorIDs();
    
    void setColors(int * array,int length);
    void setSeqSel(int layer,int step,int *selection,int length);
    
    //listener
    void mousePressed(ofMouseEventArgs & args);
    ColorSelector &getColorselectorA(){return colorselectorA;};
    ColorSelector &getColorselectorB(){return colorselectorB;};

private:
    int             _id;
    int             _curve[2] = {0,0};
    float           _freq[2] = {1,1};
    int             _dir[2] = {0,0};
    int             _time[2] = {0,0};
    int             _color[4] = {0,0,0,0};
    bool            _sequenceA[8][13];
    bool            _sequenceB[8][13];
    bool            _visible;
    int             _step;
    
    //gui
    vector<Button>      _curveAButtons;
    vector<Button>      _curveBButtons;
    
    vector<Button>      _patSelButtons;//the buttons to select a patron for playing or edit selectie

    ColorSelector colorselectorA;
    ColorSelector colorselectorB;
    

};
#endif /* Patroon_hpp */
