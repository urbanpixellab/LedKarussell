//
//  AnimatorGUI.cpp
//  LedKarussell
//
//  Created by Enrico Becker on 07/08/18.
//
//

#include "AnimatorGUI.hpp"

AnimatorGUI::AnimatorGUI(ofRectangle area,MidiControl *mc):_drawArea(area),_MC(mc)
{
    ofAddListener(ofEvents().mousePressed, this, &AnimatorGUI::mousePressed);
    _verdana.load("verdana.ttf", 18, true, true);
    _autoCounter = 0;
    _maxSteps = 4;
    _midiSelect = 0;
    ofAddListener(_MC->noteON, this, &AnimatorGUI::NoteOn);
    ofAddListener(_MC->noteOFF, this, &AnimatorGUI::NoteOff);
    ofAddListener(_MC->controlChange, this, &AnimatorGUI::ControlChange);
}
AnimatorGUI::~AnimatorGUI()
{
    ofRemoveListener(ofEvents().mousePressed, this, &AnimatorGUI::mousePressed);

    for (int i = 0; i < SLIDERS; i++)
    {
        delete _slidersA[i];
    }
    for (int i = 0; i < SLIDERS; i++)
    {
        delete _slidersB[i];
    }
    for (int i = 0; i < _patEditButtons.size(); i++)
    {
        delete _patEditButtons[i];
        delete _patLiveButtons[i];
    }
    for (int i = 0; i < _postEffectsButtons.size(); i++)
    {
        delete _postEffectsButtons[i];
    }
}

void AnimatorGUI::createAnimationGUI(int animationCount)
{
    
    _masterBrightness = RotarySlider(ofRectangle(20,200,60,60),ofVec2f(0,1),0,true,false,20,"MASTER");

    
    //create curve buttons
    int w = 50;
    int h = 30;
    // sliders
    CollumnEdit1 = _drawArea.getX();
    CollumnEdit2 = _drawArea.getX() +290;
    Collumnwidth = 280;
    Padding = 10;
    int y = _drawArea.getY();
    
    // COLLUMNEDIT1
    RotarySlider *curveA = new RotarySlider(ofRectangle(CollumnEdit1,RowsEdit[0],60,60),ofVec2f(0,animationCount-1),0,true,true,20,"curve");
    _slidersA[0] = curveA;
    RotarySlider *timeA = new RotarySlider(ofRectangle(CollumnEdit1+80,RowsEdit[0],60,60),ofVec2f(0,6),0,true,true,20,"multi");
    string b[7] = {"1/4","1/2","1","2","4","8","16"};
    timeA->addBezeichner(b, 7);
    _slidersA[1] = timeA;
    RotarySlider *freqA = new RotarySlider(ofRectangle(CollumnEdit1,RowsEdit[0]+80,60,60),ofVec2f(0,8),0,true,true,20,"freq");
    _slidersA[2] = freqA;
    RotarySlider *dirA = new RotarySlider(ofRectangle(CollumnEdit1+80,RowsEdit[0]+80,60,60),ofVec2f(0,3),0,true,true,20,"dir");
    _slidersA[3] = dirA;
    RotarySlider *phaseA = new RotarySlider(ofRectangle(CollumnEdit1+160,RowsEdit[0],60,60),ofVec2f(0,animationCount-1),0,true,true,20,"phaseC");
    _slidersA[4] = phaseA;
    RotarySlider *freqPhaseA = new RotarySlider(ofRectangle(CollumnEdit1+160,RowsEdit[0]+80,60,60),ofVec2f(0,5),0,true,false,20,"freqP");
    _slidersA[5] = freqPhaseA;

    // COLLUMNEDIT2
    RotarySlider *curveB = new RotarySlider(ofRectangle(CollumnEdit2,RowsEdit[0],60,60),ofVec2f(0,animationCount-1),0,true,true,20,"curve");
    _slidersB[0] = curveB;
    RotarySlider *timeB = new RotarySlider(ofRectangle(CollumnEdit2+80,RowsEdit[0],60,60),ofVec2f(0,6),0,true,true,20,"multi");
    timeB->addBezeichner(b, 7);
    _slidersB[1] = timeB;
    RotarySlider *freqB = new RotarySlider(ofRectangle(CollumnEdit2,RowsEdit[0]+80,60,60),ofVec2f(0,8),0,true,true,20,"freq");
    _slidersB[2] = freqB;
    RotarySlider *dirB = new RotarySlider(ofRectangle(CollumnEdit2+80,RowsEdit[0]+80,60,60),ofVec2f(0,3),0,true,true,20,"dir");
    _slidersB[3] = dirB;
    RotarySlider *phaseB = new RotarySlider(ofRectangle(CollumnEdit2+160,RowsEdit[0],60,60),ofVec2f(0,animationCount-1),0,true,true,20,"phaseC");
    _slidersB[4] = phaseB;
    RotarySlider *freqPhaseB = new RotarySlider(ofRectangle(CollumnEdit2+160,RowsEdit[0]+80,60,60),ofVec2f(0,5),0,true,false,20,"freqP");
    _slidersB[5] = freqPhaseB;
    
    
    //create patern select buttons for edit and live with double edit size
    w = 65;
    h = 20;
   
    for (int i = 0; i < 16; i++)
    {
        // EDIT BUTTONS
        string name = "Edit_" + ofToString(i);
        int x = CollumnEdit2 + ((i%4)*w*1.05);
        y = RowsEdit[3] + ((i / 4) * h * 1.05);
        Button *e = new Button(i,ofRectangle(x,y,w,h),name,true);
        ofAddListener(e->buttonIDPressed, this, &AnimatorGUI::editButtonPressed);
        _patEditButtons.push_back(e);
        // LIVE BUTTONS
        name = "Pattern_" + ofToString(i);
        x = COLLUMNLIVE + ((i%4)*w*2.4);
        y = RowsEdit[0] + ((i / 4) * h * 1.1);
        Button *l = new Button(i,ofRectangle(x,y,w*2.3,h),name,true);
        ofAddListener(l->buttonIDPressed, this, &AnimatorGUI::liveButtonPressed);
        _patLiveButtons.push_back(l);
    }
    
    // Create colorselectorS
    colorselectorA.setup(ofRectangle(CollumnEdit1,RowsEdit[1],240,200));
    colorselectorB.setup(ofRectangle(CollumnEdit2,RowsEdit[1],240,200));
    colorselectorLive.setup(ofRectangle(COLLUMNLIVE+(2*w*2.4),RowsLive[2],(2*w*2.3),340));
    
    // RowsEdit[2] --> Is the preview image
    // RowsEdit[3] --> Is the preview mesh
    
    //Create SegmentSelector
    segmenselectorA.setup(ofRectangle(CollumnEdit1,RowsEdit[4],200,200));
    segmenselectorB.setup(ofRectangle(CollumnEdit2,RowsEdit[4],200,200));
    
    // Create LightControl
    lightcontrol.setup(ofRectangle(COLLUMNLIVE,RowsLive[4],200,200));

    
    //adding post effects
    int x = COLLUMNLIVE + (w*2.2);

    Button *pBlack = new Button();
    pBlack->setup(ofRectangle(780,550,w,h), "Blackout", true);
    _postEffectsButtons.push_back(pBlack);
    
    y = RowsEdit[0] + (h * 1.1);
    Button *pFlash = new Button();
    pFlash->setup(ofRectangle(780 + w*1.1,550,w,h), "Flash", true);
    _postEffectsButtons.push_back(pFlash);

    Button *pInvert = new Button();
    pInvert->setup(ofRectangle(780+w*2.2,550,w,h), "Invert", true);
    _postEffectsButtons.push_back(pInvert);

    Button *pBlink1 = new Button();
    pBlink1->setup(ofRectangle(780+w*3.2,550,w,h), "Blink_FF", true);
    _postEffectsButtons.push_back(pBlink1);
    
    Button *pLoopLichtP1 = new Button();
    pLoopLichtP1->setup(ofRectangle(780+w*4.2,550,w,h), "LoopP1", true);
    _postEffectsButtons.push_back(pLoopLichtP1);
    
    Button *pLoopLichtP2 = new Button();
    pLoopLichtP2->setup(ofRectangle(780+w*5.2,550,w,h), "LoopK1", true);
    _postEffectsButtons.push_back(pLoopLichtP2);
    
    Button *pLoopLichtP3 = new Button();
    pLoopLichtP3->setup(ofRectangle(780+w*6.2,550,w,h), "LoopP2", true);
    _postEffectsButtons.push_back(pLoopLichtP3);
    
    Button *pLoopLichtP4 = new Button();
    pLoopLichtP4->setup(ofRectangle(780+w*7.2,550,w,h), "LoopK2", true);
    _postEffectsButtons.push_back(pLoopLichtP4);

    Button *pStrobe = new Button();
    pStrobe->setup(ofRectangle(780,550 + h*1.1,w,h), "Rand Strobe", true);
    _postEffectsButtons.push_back(pStrobe);
    

    //the auto button
    _autoPattern.setup(ofRectangle(ofGetWidth()-w*3,ofGetHeight()-h*3,w*3,h*3), "GetDrinks", true);
    
    // the mute artnet button
    _muteArtnet.setup(ofRectangle(20,300,50,50), "MUTE", true);
    ofAddListener(_muteArtnet.buttonIDPressed, this, &AnimatorGUI::muteArtnetButtonPressed);

}

void AnimatorGUI::update()
{
    /*
    //do the auto function here
    int old =
    int sel = fmod(ofGetElapsedTimef(),16);
    _patLiveButtons[sel]->pressedControler();
    //also press the button or better pre
     */
    if(_MC->getBeat() == true)
    {
        if (_autoPattern.getState() == true)
        {
            cout << "beat" << _autoCounter << endl;
            if(_autoCounter >= _maxSteps)
            {
                _maxSteps = ofRandom(2,10);
                //choose new live pattern
                
            }
            _autoCounter++;
            if (_autoCounter >= _maxSteps)
            {
                _autoCounter = 0;
                //trigger event to choose a new patroon randomlye
                int newPattern = floor(ofRandom(0,16));
                _maxSteps = ofRandom(8)*8;
                //deselect all others
                for (int i = 0; i < _patLiveButtons.size(); i++)
                {
                    if(_patLiveButtons[i]->getState() == true)
                    {
                        _patLiveButtons[i]->setState(false);
                    }
                    
                }
                _patLiveButtons[newPattern]->pressedControler();
                ofNotifyEvent(patronPLAY,newPattern);
            }
        }
        else if(_autoPattern.getState() == false && _oldAutoState == true)
        {
            _autoCounter = 0;
        }
        
    }
    _oldAutoState = _autoPattern.getState() ;
}

void AnimatorGUI::draw(ofImage &pre, ofImage &live)
{
    //draw background frame
    ofSetColor(70);
    ofFill();
    // Left
    ofDrawRectRounded(CollumnEdit1-Padding,RowsEdit[0]-2*Padding,Collumnwidth*2+Padding*2,800,4);
    // Right
    ofDrawRectRounded(COLLUMNLIVE-Padding,RowsLive[0]-2*Padding,Collumnwidth*2.8+Padding*2,560,4);
    // botom right
    ofDrawRectRounded(COLLUMNLIVE-Padding,RowsLive[3]-2*Padding,Collumnwidth*2.8+Padding*2,230,4);
    
    
    // Draw text
    ofSetColor(255);
    // Left
    _verdana.drawString("Edit pattern", CollumnEdit1, RowsEdit[0]-Padding);
    // Right
    _verdana.drawString("Live", COLLUMNLIVE, RowsLive[0]-Padding);
    // Right
    _verdana.drawString("Light Commands", COLLUMNLIVE, RowsLive[3]-Padding);
    
    
    ofSetColor(255);
    pre.draw(CollumnEdit1, RowsEdit[3],Collumnwidth,100);
    live.draw(COLLUMNLIVE, RowsLive[2],Collumnwidth+20,140);
    
    //draw it under the preview windows
    ofPushMatrix();
    ofTranslate(CollumnEdit1,RowsEdit[2]);
    ofScale(ofGetWidth()/3, 100);
    pre.bind();
    _realStructure.draw();
    pre.unbind();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(COLLUMNLIVE,RowsLive[1]);
    ofScale(800, 200);
    live.bind();
    _realStructure.draw();
    live.unbind();
    ofPopMatrix();
    //sliders
    for (int i = 0; i < SLIDERS; i++)
    {
        _slidersA[i]->draw();
        _slidersB[i]->draw();
    }
    
    // Color Selector
    colorselectorA.draw();
    colorselectorB.draw();
    colorselectorLive.draw();
    
    // SegmentSelector
    segmenselectorA.draw();
    segmenselectorB.draw();
    
    //LightController
    lightcontrol.draw();
    
    // the patron selector
    for (int i = 0; i < _patEditButtons.size(); i++)
    {
        //set the color based on the mode
        _patEditButtons[i]->draw();
        _patLiveButtons[i]->draw();
    }
    // post effect sliders
    for (int i = 0; i < _postEffectsButtons.size(); i++)
    {
        _postEffectsButtons[i]->draw();
    }
    //master broightness
    ofSetColor(255);
    _masterBrightness.draw();
    _autoPattern.draw();
    _muteArtnet.draw();
}

void AnimatorGUI::mousePressed(ofMouseEventArgs &args)
{
}

// sets state to false of all buttons except the one being pressed
void AnimatorGUI::editButtonPressed(int &id)
{
    for (int i=0; i < _patEditButtons.size(); i++)
    {
        if(i == id) continue;
        _patEditButtons[i]->getState() = false;
    }
    ofNotifyEvent(patronEDIT,id);
}

// sets state to false of all buttons except the one being pressed
void AnimatorGUI::liveButtonPressed(int &id)
{
    for (int i=0; i < _patLiveButtons.size(); i++)
    {
        if(i == id) continue;
        _patLiveButtons[i]->getState() = false;
    }
    ofNotifyEvent(patronPLAY,id);
}


void AnimatorGUI::muteArtnetButtonPressed(int &id)
{
    bool state =_muteArtnet.getState();
    ofNotifyEvent(muteButtonPressed,state);
}

void AnimatorGUI::setEditButtonState(int &id, bool state)
{
    _patEditButtons[id]->setState(state);
}

void AnimatorGUI::setLiveButtonState(int &id, bool state)
{
    _patLiveButtons[id]->setState(state);
}

void AnimatorGUI::setMuteArtnetButtonState(bool state)
{
    _muteArtnet.setState(state);
}



void AnimatorGUI::NoteOn(int &id)
{
    //first check select buttons
    for (int i = 0; i < 6; i++)
    {
        if(id == _MidiSelector[i])
        {
            // we have a midi select knop pressed
            if (id == _MidiSelector[0]) _midiSelect = 1; //color a
            else if (id == _MidiSelector[1]) _midiSelect = 5; //color b
            else if (id == _MidiSelector[2]) _midiSelect = 2; //color b
            else if (id == _MidiSelector[3]) _midiSelect = 3; //color c
            else if (id == _MidiSelector[5]) _midiSelect = 4; //color d
            break;
        }
    }
    switch (_midiSelect) {
        case 0://live pattern
            for (int i = 0; i < 16; i++)
            {
                if (_MidiSequenzerButtons[i] == id)
                {
                    _patLiveButtons[i]->pressedControler();
                    liveButtonPressed(i);
                }
            }
            break;
            
        case 1://color a
            for (int i = 0; i < 16; i++)
            {
                if (_MidiSequenzerButtons[i] == id)
                {
                    colorselectorA.setColorAId(i);
                }
            }
            break;
            
        case 2://color b
            for (int i = 0; i < 16; i++)
            {
                if (_MidiSequenzerButtons[i] == id)
                {
                    colorselectorA.setColorBId(i);
                }
            }
            break;
            
        case 3://color a
            for (int i = 0; i < 16; i++)
            {
                if (_MidiSequenzerButtons[i] == id)
                {
                    colorselectorB.setColorAId(i);
                }
            }
            break;
            
        case 4://color b
            for (int i = 0; i < 16; i++)
            {
                if (_MidiSequenzerButtons[i] == id)
                {
                    colorselectorB.setColorBId(i);
                }
            }
            break;
            
        case 5://edit pattern
            for (int i = 0; i < 16; i++)
            {
                if (_MidiSequenzerButtons[i] == id)
                {
                }
            }
            break;
            
        default:
            break;
    }
    if(id == 49) //flash
    {
        _postEffectsButtons[1]->pressedControler();
    }
    if(id == 51) //bpm
    {
        _MC->newTap();
    }
}

void AnimatorGUI::NoteOff(int &id)
{
    for (int i = 0; i < 6; i++)
    {
        if (id == _MidiSelector[0]) _midiSelect = 0; //color a
        else if (id == _MidiSelector[1]) _midiSelect = 0; //color b
        else if (id == _MidiSelector[2]) _midiSelect = 0; //color b
        else if (id == _MidiSelector[3]) _midiSelect = 0; //color c
        else if (id == _MidiSelector[5]) _midiSelect = 0; //color d

    }
    if(id == 49)
    {
        _postEffectsButtons[1]->pressedControler();
    }

}

void AnimatorGUI::ControlChange(int &id)
{
    /*
    if(id == 2)
    {
        cout << ofMap(_MC->getControlValue(id),0,127,0,1) << endl;
        float value = ofMap(_MC->getControlValue(id),0,127,0,1);
        ofClamp(value, 0, 1);
        _masterBrightness.setValueMapped(value);
    }*/
}
