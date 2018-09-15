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
    // set number of steps in the segment sequence
    _numStepsInSequnce = 8;
    _flashCount = 0;
    _isFlash = false;
    _isMuted = false;
    
    _GUI = new AnimatorGUI(ofRectangle(100,50,500,500),_MC);
    _GUI->createAnimationGUI(LedAnimator::CURVE_COUNT);

    _preAnimator = new LedAnimator(_MC,_GUI->getMasterBrightnessKnob().getValueNormalized());
    _liveAnimator = new LedAnimator(_MC,_GUI->getMasterBrightnessKnob().getValueNormalized());
    
    //init the gui
    _test = false;
    _step = 0;
    
    // Load all the nodes form XML
    loadNodes();
    
    // Load all the patterns from XML
    loadPatroon();
    
    // FIXME: probably not the best way to do it.
    // SET SEGMENT SELECTOR FORM EDIT PATROON
    for(int i=0;i<getNumStepsSequence();i++)
    {
        _GUI->segmenselectorA.setSequence(i,_editPatroon->getSeqStepA(i));
        _GUI->segmenselectorB.setSequence(i,_editPatroon->getSeqStepB(i));
    }
    
    // To initialize the program we set the dit pattern and live patten to 0
    int id = 0;
    // Simulate a button press for edit and live buttons
    EditPatronPressed(id);
    PlayPatronPressed(id);
    // Set states of edit buttons and live buttons
    _GUI->setEditButtonState(id,true);
    _GUI->setLiveButtonState(id,true);
    
    
    //Listeners
    ofAddListener(ofEvents().keyPressed, this, &ArtnetControl::keyPressed);
    //ad listeners to the patroon select buttons for different functions
    ofAddListener(_GUI->curveAPressed, this, &ArtnetControl::curveAPressed);
    ofAddListener(_GUI->curveBPressed, this, &ArtnetControl::curveBPressed);

    ofAddListener(_GUI->patronPLAY, this, &ArtnetControl::PlayPatronPressed);
    ofAddListener(_GUI->patronEDIT, this, &ArtnetControl::EditPatronPressed);
    ofAddListener(_GUI->patronPLAYSTEPPED, this, &ArtnetControl::PlaySteppedPatronPressed);
    // FIXME: NOT needed ?
    ofAddListener(_GUI->colorSelectPressed, this,&ArtnetControl::colorPressed);
    
    ofAddListener(_GUI->getCurveSlidersA()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getCurveSlidersB()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getTimeSliderA()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getTimeSliderB()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getFreqSliderA()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getFreqSliderB()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getDirSliderA()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getDirSliderB()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getColorselectorA().colorPressed, this, &ArtnetControl::colorPressed);
    ofAddListener(_GUI->getColorselectorB().colorPressed, this, &ArtnetControl::colorPressed);
    ofAddListener(_GUI->getColorselectorLive().colorPressed, this, &ArtnetControl::colorLivePressed);
    ofAddListener(_GUI->getSegmenselectorA().segmentPressed, this, &ArtnetControl::segmentSelectPressed);
    ofAddListener(_GUI->getSegmenselectorB().segmentPressed, this, &ArtnetControl::segmentSelectPressed);
    ofAddListener(_GUI->getPhaseCurveSliderA()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getPhaseCurveSliderB()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getPhaseFreqSliderA()->newValue, this, &ArtnetControl::sliderChanged);
    ofAddListener(_GUI->getPhaseFreqSliderB()->newValue, this, &ArtnetControl::sliderChanged);
    
    // Catch listener
    ofAddListener(_GUI->muteButtonPressed, this, &ArtnetControl::muteArtnetPressed);

}

ArtnetControl::~ArtnetControl()
{
    //save everything
    
    // blackout all
    ofColor black(0);
    for (int i = 0; i < _liveSegments.size(); i++)
    {
        _liveSegments[i]->setAllPixel(black);
    }
    sendToNodes();
    
    savePatroon();
    ofRemoveListener(ofEvents().keyPressed, this, &ArtnetControl::keyPressed);
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
    for (int i = 0; i < _preSegments.size(); i++)
    {
        delete _preSegments[i];
        delete _liveSegments[i];
    }
    _preSegments.clear();
    _liveSegments.clear();
    
}

void ArtnetControl::loadNodes()
{
    clearNodes();
    //important for the mapping a segment cant go from one univcerse to another,
    //otherwise its is getting complicated!!!!
    //load nodes from xml
    ofxXmlSettings nodes("nodes.xml");
    int count = nodes.getNumTags("node");
    for (int i = 0; i < count; i++)
    {
        nodes.pushTag("node",i);
        Node *n = new Node();
        n->ip = nodes.getValue("ip", "127.0.0.1");
        n->name = nodes.getValue("name", "A1");
//        cout << n->ip << " " << n->name << endl;
        //string ip = "192.168.12."+ ofToString(200+i);
        n->artnet.begin(n->ip.c_str());
        //fill universes black
        for (int u = 0; u < 8; u++)
        {
            for (int l = 0; l < 512; l++)
            {
                n->universes[u][l] = 0;
            }
        }
        _nodes.push_back(n);
        nodes.popTag();
    }
    cout << "num nodes " << _nodes.size() << endl;
    // now load the segments
    _preSegments.clear();
    _liveSegments.clear();
    ofxXmlSettings segments("segments.xml");
    int numSegments = segments.getNumTags("segment");
    /*	<id>0</id>
     <node>0</node>
     <universe>0</universe>
     <beginPixel>0</beginPixel>
     <length>150</length>
     */
    for (int i = 0; i < numSegments; i++)
    {
        segments.pushTag("segment",i);
        
        //int universe,int begin,int end, Node * node
        //or settings based on an xml from the settings
        int node = segments.getValue("node", 0);
        int universe = segments.getValue("universe", 0);
        //int nodeID,int universe,int beginLed,int endLed,int segmentID
        int begin = segments.getValue("beginPixel", 0);
        int length = segments.getValue("length", 150);//ofRandom(150); // should be max 150
        cout << "segment " << i << " node " << node << " " << universe << endl;
        Segment *newSegP = new Segment(node,universe,begin,length,i);
        Segment *newSegL = new Segment(node,universe,begin,length,i);
        _preSegments.push_back(newSegP);
        _liveSegments.push_back(newSegL);
        segments.popTag();
    }
    
    //load the selections from xml and create buttons for every selection with the name and id
    ofxXmlSettings sel("selector.xml");
    _selections.clear();
    for (int i = 0; i < sel.getNumTags("select"); i++)
    {
        sel.pushTag("select",i);
        Selection s;
        s.name = sel.getValue("name", "untitled");
        string segments = sel.getValue("segment", "");
        vector<string> segmentSelection = ofSplitString(segments, ",");
        for (int seg = 0; seg < segmentSelection.size(); seg++)
        {
            s.items.push_back(ofToInt(segmentSelection[seg]));
        }
        _selections.push_back(s);
        sel.popTag();
    }

    
    
    //whiteout
    //also create the preview images
    _preIMG.allocate(150, _preSegments.size(),OF_IMAGE_COLOR); // max of 510 channels
    _liveIMG.allocate(150, _preSegments.size(),OF_IMAGE_COLOR);
    _preIMG.setColor(ofColor(0,0,0));
    _liveIMG.setColor(ofColor(0,0,0));
    
    // fill the images black!!!
    //setup the mesh
    _GUI->getMesh().clear();
    _GUI->getMesh().setMode(OF_PRIMITIVE_LINES);
    int offset = 9;//row texture offset
    for (int i = 0; i < 16; i++)
    {
        float x = 0.25 + (i * (0.5/16.));
        float y = 0.10;
        _GUI->getMesh().addVertex(ofVec2f(x,y));
        _GUI->getMesh().addTexCoord(ofVec2f(150,i + offset)); // offset of 9 rows
        y = 1-y;
        _GUI->getMesh().addVertex(ofVec2f(x,y));
        _GUI->getMesh().addTexCoord(ofVec2f(0,i + offset));
    }
    // to fix the texture is right for the leds, but it is not sharp on the preview
    
    // now the triangles left and right, as losse lines
    //left and right
    for (int s = 0; s < 2; s++)
    {
        offset = s*25;
        ofVec2f center = ofVec2f(0.125 + (s * 0.75),0.5);
        for (int d = 0; d < 3; d++)//every driehoek
        {
            float rad = 0.66 - (d * (0.66/3));//inverse great to small
            
            _GUI->getMesh().addVertex(ofVec2f(center.x,center.y - center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(0,0 + (d*3) + offset));
            _GUI->getMesh().addVertex(ofVec2f(center.x + 0.125*rad,center.y + center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(150,0 + (d*3) + offset));
            
            _GUI->getMesh().addVertex(ofVec2f(center.x + 0.125*rad,center.y + center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(0,1 + (d*3) + offset));
            _GUI->getMesh().addVertex(ofVec2f(center.x - 0.125*rad,center.y + center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(150,1 + (d*3) + offset));
            
            _GUI->getMesh().addVertex(ofVec2f(center.x - 0.125*rad,center.y + center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(0,2 + (d*3) + offset));
            _GUI->getMesh().addVertex(ofVec2f(center.x,center.y - center.y*rad));
            _GUI->getMesh().addTexCoord(ofVec2f(150,2 + (d*3) + offset));
        }
    }
}

void ArtnetControl::loadPatroon()
{
    cout << "Loading Patroonen" << endl;
    ofxXmlSettings sel("patroonen.xml");
    for (int i = 0; i < sel.getNumTags("patroon"); i++)
    {
        sel.pushTag("patroon",i);
        //int id,int curveA,int curveB,float freqA,float freqB,float dirA, float dirB,int timeA, int timeB,int colorA, int colorB,int colorC, int colorD
        int id = sel.getValue("id", i);
        
        
        //shift layer
        sel.getNumTags("layer");
        int curve[sel.getNumTags("layer")];
        float freq[sel.getNumTags("layer")];
        int dir[sel.getNumTags("layer")];
        int time[sel.getNumTags("layer")];
        int color[sel.getNumTags("layer") * 2];
        int phaseCurve[sel.getNumTags("layer")];
        float phaseFreq[sel.getNumTags("layer")];
        
        for (int l = 0; l < sel.getNumTags("layer"); l++)
        {
            sel.pushTag("layer",l);
            curve[l] = sel.getValue("curve", 0);
            freq[l] = sel.getValue("cFreq", 1);
            dir[l] = sel.getValue("cFreq", 0);
            time[l] = sel.getValue("cTime", 2);
            color[(l * 2) + 0] = sel.getValue("colorA", 0);
            color[(l * 2) + 1] = sel.getValue("colorB", 0);
            phaseCurve[l] = sel.getValue("phaseCurve", 0.0);// getColorIDs()[(l*2) + 0]);
            phaseFreq[l] = sel.getValue("phaseFreq", 0.0);// getColorIDs()[(l*2) + 0]);
            sel.popTag();
        }
        
        Patroon p(id,curve[0],curve[1],freq[0],freq[1],dir[0],dir[1],time[0],time[1],color[0],color[1],color[2],color[3],phaseCurve[0],phaseCurve[1],phaseFreq[0],phaseFreq[1]);
        
        
        //vector<int>
        // add the sequence matrix based on the indices
        string steps[8] = {"step0","step1","step2","step3","step4","step5","step6","step7"};
        for (int l = 0; l < sel.getNumTags("layer"); l++)
        {
            sel.pushTag("layer",l);
            for (int step = 0; step < getNumStepsSequence(); step++)
            {
                vector<string> result = ofSplitString(sel.getValue(steps[step], ""), ",");
                for (int selection = 0; selection < result.size(); selection++)
                {
                    //cout << l << "selected indices " <<ofToInt(result[selection]) << endl;
                    if(l == 0) p.setSeqA(step, ofToInt(result[selection]), true);
                    else if(l == 1) p.setSeqB(step, ofToInt(result[selection]), true);
                }
            }
            sel.popTag();
        }
        _patronen.push_back(p);
        sel.popTag();
    }

    _editPatroon = &_patronen[0];
    _livePatroon = &_patronen[0];
    
}

void ArtnetControl::savePatroon()
{
    cout << "SAVE PATROON " << endl;
    ofxXmlSettings settings;
    int maxSel = getNumSelections();
    int maxStep = getNumStepsSequence();
    for (int p = 0; p < _patronen.size(); p++)//patron
    {
        //cout << "saving number: " << p <<" with id: " << _patronen[p].getID() << endl;
        //_patronen[p].printPatroon();
        
        settings.addTag("patroon");
        settings.pushTag("patroon",p);
        settings.addValue("id", _patronen[p].getID());
        for (int l = 0; l < 2; l++) //layer
        {
            settings.addTag("layer");
            settings.pushTag("layer",l);
            settings.addValue("curve", *_patronen[p].getCurve(l));
            settings.addValue("cFreq", *_patronen[p].getFreq(l));
            settings.addValue("cDir", *_patronen[p].getDir(l));
            settings.addValue("cTime", *_patronen[p].getTime(l));
            settings.addValue("colorA", _patronen[p].getColorIDs()[(l*2) + 0]);
            settings.addValue("colorB", _patronen[p].getColorIDs()[(l*2) + 1]);
            settings.addValue("phaseCurve", *_patronen[p].getPhase(l));// getColorIDs()[(l*2) + 0]);
            settings.addValue("phaseFreq", *_patronen[p].getPhaseFreq(l));
    
            if(l == 0)
            {
                for (int s = 0; s < maxStep; s++)
                {
                    string step = "";
                    for (int sel = 0; sel < maxSel; sel++)
                    {
                        if(_patronen[p].getSeqStepA(s)[sel] == true) step += ofToString(sel) + ",";
                    }
                    if(step.length() > 1)step.erase(step.length()-1);
                    settings.addValue("step"+ofToString(s), step);
                }
            }
            else if(l == 1)
            {
                //cout << "steps layer 1" << endl;
                for (int s = 0; s < maxStep; s++)
                {
                    string step = "";
                    for (int sel = 0; sel < maxSel; sel++)
                    {
                        if(_patronen[p].getSeqStepB(s)[sel] == true) step += ofToString(sel) + ",";
                    }
                    if(step.length() > 1)step.erase(step.length()-1);
                    settings.addValue("step"+ofToString(s), step);
                   // cout << "step"+ofToString(s) << " -> " << step << endl;
                }
            }
            settings.popTag();
        }
        settings.popTag();
    }
    settings.save("patroonen.xml");
    
}


void ArtnetControl::update()
{
    _GUI->update();
    if(_MC->getBeat())
    {
        _step++;
        if(_step >= getNumStepsSequence()) _step = 0;
    }
    fillWithBackgroundColor(_black);
    
    doLedAnimation(_editPatroon, _preAnimator,_preSegments,_step);
    doLedAnimation(_livePatroon, _liveAnimator,_liveSegments,_step);

    _GUI->segmenselectorA.setStep(_step);
    _GUI->segmenselectorB.setStep(_step);

    //dont forget the fade out function to the beat and the blink function
    // use the postarray of the segments for this functions
    
    // now updaste the arrays to the visualizer
    writeSegmentsToImage(_preSegments, _preIMG);
    writeSegmentsToImage(_liveSegments, _liveIMG);
    
    sendToNodes();
}

void ArtnetControl::doLedAnimation(Patroon * pattern,LedAnimator * animator,vector<Segment*> segments,int &step)
{
    int select = *pattern->getFreq(0);
    float *freqA = &_freqences[select];
    
    select = *pattern->getFreq(1);
    float *freqB = &_freqences[select];
    
    bool solo = true; // solo means that every segment is treated seperate otherwise we melt it to one big array
    //first fill all with background color
    
    // Get colors from editPatroon
    vector<int> getColorIDs = pattern->getColorIDs();
    ofColor c1 = _GUI->getColorselectorA().getColorFromID(getColorIDs[0]);
    ofColor c2 = _GUI->getColorselectorA().getColorFromID(getColorIDs[1]);
    ofColor c3 = _GUI->getColorselectorB().getColorFromID(getColorIDs[2]);
    ofColor c4 = _GUI->getColorselectorB().getColorFromID(getColorIDs[3]);
    // to do add index shift function to phaseshift the curve from index by a curve and freq

    if(_GUI->getPostEffectButton(4)->getState() == false)
    {
        for (int stepElement = 0; stepElement < getNumSelections(); stepElement++)
        {
            if(pattern->getSeqStepA(_step)[stepElement] ==  true)
            {
                int s = _selections[stepElement].items.size();
                for (int i = 0; i < s; i++)
                {
                    int seg = _selections[stepElement].items[i];

                    float segShift = frequenzShift(*pattern->getPhase(0), i/float(s), *pattern->getPhaseFreq(0));
                    
                    animator->drawToArray(*pattern->getCurve(0),*pattern->getDir(0),*pattern->getTime(0),*freqA, segments[seg]->getArray(),segments[seg]->getBegin(), segments[seg]->getLength(),c1,c2,segShift);
                    
                }
            }
        }
        //add now the second color
        for (int stepElement = 0; stepElement < getNumSelections(); stepElement++)
        {
            if(pattern->getSeqStepB(_step)[stepElement] ==  true)
            {
                int s = _selections[stepElement].items.size();
                for (int i = 0; i < s; i++)
                {
                    int seg = _selections[stepElement].items[i];
                    float segShift = frequenzShift(*pattern->getPhase(1), i/float(s), *pattern->getPhaseFreq(1));
                    
                    animator->addToArray(*pattern->getCurve(1),*pattern->getDir(1),*pattern->getTime(1),*freqB, segments[seg]->getArray(),segments[seg]->getBegin(), segments[seg]->getLength(),c3,c4,segShift);
                }
            }
        }
    }
    //flash
    
    if(_GUI->getPostEffectButton(1)->getState() == true) _flashCount = 255;
    //paint white over it if false reduce the value and if value == 0 stop it
    if (_flashCount > 5)
    {
        ofColor col = ofColor(_GUI->getColorselectorLive().getColorFromID(_GUI->getColorselectorLive().getSelectedColorIDs()[0]) * (_flashCount/255.));
        //ofColor col = ofColor(_flashCount);
//        cout << _GUI->getColorselectorLive().getColorFromID(0) << endl;
        float f = 0;
        for (int seg = 0; seg < _liveSegments.size(); seg++)
        {
            animator->maxToArray(0,0,0,0, segments[seg]->getArray(),segments[seg]->getBegin(), segments[seg]->getLength(),col,col,f);
        }
        _flashCount -= 25;
    }
    //invert
    if (_GUI->getPostEffectButton(2)->getState() == true)
    {
        for (int seg = 0; seg < _liveSegments.size(); seg++)
        {
            animator->invert(segments[seg]->getArray(),segments[seg]->getBegin(), segments[seg]->getLength());
        }
    }
    // datamosh
    // blink blink fast
    if(_GUI->getPostEffectButton(3)->getState() == true)
    {
        //first call clear the array
        // then add random points in white
        for (int i = 0; i < _liveSegments.size(); i++)
        {
            for(int p = 0;p < _liveSegments[i]->getLength();p++)
            {
                if(_liveSegments[i]->getPostArray()[p] > 5) _liveSegments[i]->getPostArray()[p] -= 5;
                else _liveSegments[i]->getPostArray()[p] = 0;
            }
            //now add random pixels
            int r = ofRandom(0,2);
            for (int sp = 0; sp < r; sp++)
            {
                int rp = ofRandom(_liveSegments[i]->getLength()/3);
                _liveSegments[i]->setPostPixel(rp, _GUI->getColorselectorLive().getColorFromID(_GUI->getColorselectorLive().getSelectedColorIDs()[0]));
                
            }
            _liveSegments[i]->copyPostToArray();
        }
    }
    

    // stage looplicht a
    if(_GUI->getPostEffectButton(4)->getState() == true)
    {
        int segOff = 9;
        for (int seg = 0; seg < _liveSegments.size(); seg++)
        {
            if(seg == segOff+_step || seg == segOff+8+_step)
            {
                continue;
            }
            else
            {
                animator->blackout(segments[seg]->getArray(),segments[seg]->getBegin() ,segments[seg]->getLength());
            }
            
        }
    }
    
    // stage looplicht b
    if(_GUI->getPostEffectButton(5)->getState() == true)
    {
        int segOff = 9;
        for (int seg = 0; seg < _liveSegments.size(); seg++)
        {
            if(seg == segOff+_step || seg == 24-_step)
            {
                continue;
            }
            else
            {
                animator->blackout(segments[seg]->getArray(),segments[seg]->getBegin() ,segments[seg]->getLength());
            }
        }
    }
    
    // stage looplicht c
    if(_GUI->getPostEffectButton(6)->getState() == true)
    {
        int segOff = 9;
        for (int seg = 0; seg < _liveSegments.size(); seg++)
        {
            if(seg == 16-_step || seg == 17+_step)
            {
                continue;
            }
            else
            {
                animator->blackout(segments[seg]->getArray(),segments[seg]->getBegin() ,segments[seg]->getLength());
            }
        }
    }
    

    // stage looplicht d
    if(_GUI->getPostEffectButton(7)->getState() == true)
    {
        int segOff = 9; //offset from triangle
        for (int seg = 0; seg < _liveSegments.size(); seg++)
        {
            if(seg >= 9 && seg < 25)
            {
                continue;
            }
            else
            {
                animator->blackout(segments[seg]->getArray(),segments[seg]->getBegin() ,segments[seg]->getLength());
            }
        }
    }
    //met modulo the two colors

    // random strobe
    if(_GUI->getPostEffectButton(8)->getState() == true)
    {
        for (int seg = 0; seg < _liveSegments.size(); seg++)
        {
            if (ofRandom(10) > 7)
            {
                animator->white(segments[seg]->getArray(),segments[seg]->getBegin() ,segments[seg]->getLength());
            }
            else
            {
                animator->blackout(segments[seg]->getArray(),segments[seg]->getBegin() ,segments[seg]->getLength());
            }
        }
    }
    
}

float ArtnetControl::frequenzShift(int curve,float time, float freq)
{
    //FIXME add curves
    /*
     WHITEOUT,1
     RAMP,2
     SAW,3
     TRI,4
     RECT,5
     SINE,6
     COS,7

     */
    float segShift = 0;//     BLACKOUT = 0,

    switch (curve) {
        case 1:
            segShift = 1;//whiteout
            break;
            
        case 2://ramp
            segShift = fmod(time,1./freq) * (1-(1./freq));
            break;
            
        case 3://saw
            segShift = (1.-(fmod(time,1./freq))) * (1-(1./freq));
            break;
            
        case 4://tri
            segShift = fmod(time,1./freq) * (1-(1./freq));
            if(fmod(time,freq) < 0.5) segShift = 0;
            break;
            
        case 5://rect
            
            segShift = fmod(time,1./freq);
            if((fmod(time,1./freq) * (1-(1./freq))) < 0.5) segShift = 0;
            break;
            
        case 6:
            segShift = sin(time * TWO_PI * freq);
            break;
            
        case 7:
            segShift = cos(time * TWO_PI * freq);
            break;
            
        default:
            break;
    }
    return segShift;
    /*
     case CURVE::RECT://add frequency in repetition
     {
     for (int i = 0; i < len; i++) //reduce to pixel
     {
     float delta = len/freq;
     float value = 0;
     float timeShift = _MC->getDtMulti(time)*delta;
     if(fmod(i+delta+timeShift+shift,delta) > delta/2) value = 1;
     values[i] = value;
     }
     
     break;
     }
     case CURVE::RAMP:
     {
     for (int i = 0; i < len; i++) //reduce to pixel
     {
     values[i] = fmod(((shift + i/len) + _MC->getDtMulti(time))*freq,1);
     }
     
     break;
     }
     case CURVE::SAW:
     {
     for (int i = 0; i < len; i++) //reduce to pixel
     {
     values[i] = 1.0 - fmod(((shift + i/len) + _MC->getDtMulti(time))*freq,1);
     }
     
     break;
     }
     case CURVE::TRI:
     {
     for (int i = 0; i < len; i++) //reduce to pixel
     {
     //have some glitch in the middle
     float value = fmod(((shift + i/len) + _MC->getDtMulti(time))*freq,1) * 2.;
     if(value > 1.0) value = 1.0-value;
     values[i] = value;
     }
     
     break;
     }

     */
    //float segShift = sin(i/float(s) * TWO_PI * *pattern->getPhaseFreq(0)); // based on the curve and the frequency and the index
    //muss nur einmal hier berechnet werden

}


void ArtnetControl::fillWithBackgroundColor(ofColor & color)
{
    for(int i = 0;i < _preSegments.size();i++)
    {
        _preSegments[i]->setAllPixel(_black);
        _liveSegments[i]->setAllPixel(_black);
    }
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
    if(!_isMuted)
    {
        //black
        if (_GUI->getPostEffectButton(0)->getState() == true)
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
                    u_int64_t data = _liveSegments[i]->getArray()[cell];
                    // mach was damit
                    _nodes[nodeID]->universes[universe][cell] = 0;
                }
            }
        }
        else
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
                    //get the array where to write which function
                    //and write it to the nodes
                    //            u_int64_t data = _preSegments[i]->getArray()[first+cell];
                    u_int64_t data = _liveSegments[i]->getArray()[cell];
                    // mach was damit
                    _nodes[nodeID]->universes[universe][cell] = data;//_liveSegments[i]->getArray()[first+cell];
                }
            }
        }
        
        for (int n = 0; n < _nodes.size(); n++)
        {
            for (int u = 0; u < 8; u++)
            {
                // we set the length to 3x170 = 510 has to been tested
                _nodes[n]->artnet.send(_nodes[n]->universes[u],u,510);
            }
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

void ArtnetControl::colorPressed(bool &pressed)
{
    //we have a changed color, set them to the edit patroon
    int colors[4];
    colors[0] = _GUI->getColorselectorA().getSelectedColorIDs()[0];
    colors[1] = _GUI->getColorselectorA().getSelectedColorIDs()[1];
    colors[2] = _GUI->getColorselectorB().getSelectedColorIDs()[0];
    colors[3] = _GUI->getColorselectorB().getSelectedColorIDs()[1];
    _editPatroon->setColors(colors, 4);
}

void ArtnetControl::colorLivePressed(bool &pressed)
{
    // We changed live colours let's do soemthing with it..
 //   cout << "We changed live colours let's do soemthing with it.. " << endl;
}

void ArtnetControl::segmentSelectPressed(bool &pressed)
{
    //we have selected a segment in a seguence sent to edit patroon
    //cout << "ArtnetControl::segmentSelectPressed " << endl;
    
    // FIXME: do we need a function in patroon to set the whole sequence in one go?
    // Loop through segment selection
    for(int s=0; s < getNumSelections(); s++)
    {
        //cout << "numselection: " << s << endl;
        // loop through sequence step
        for(int i=0;i< getNumStepsSequence();i++){
            // get the sequence step i for segemnt s
            bool valueA = _GUI->getSegmenselectorA().getSequence(s)[i];
            bool valueB = _GUI->getSegmenselectorB().getSequence(s)[i];
            //cout <<" "<< i << ":" << valueA << "," << valueB;
            _editPatroon->setSeqA(i, s, valueA);
            _editPatroon->setSeqB(i, s, valueB);
        }
        //cout << endl;
    }
    
    // DEBUG
    //_editPatroon->printMatrix();
    
}



void ArtnetControl::keyPressed(ofKeyEventArgs &key)
{
    if(key.key == 114)
    {
        loadNodes();
        cout << key.key << endl;
    }
}

void ArtnetControl::muteArtnetPressed(bool & state){
    cout << "ArtnetControl:: mute button pressed state -> " << state << endl;
    _isMuted = state;
}

void ArtnetControl::sliderChanged(bool & value)
{
    // Edit patroon wants the value between the min and max range NOT 0-1
    *_editPatroon->getCurve(0) = int(_GUI->getSlidersAMapped(0));
    *_editPatroon->getCurve(1) = int(_GUI->getSlidersBMapped(0));
    
    *_editPatroon->getFreq(0) = _GUI->getSlidersAMapped(2);
    *_editPatroon->getFreq(1) = _GUI->getSlidersBMapped(2);

    *_editPatroon->getTime(0) = _GUI->getSlidersAMapped(1);
    *_editPatroon->getTime(1) = _GUI->getSlidersBMapped(1);

    *_editPatroon->getDir(0) = _GUI->getSlidersAMapped(3);
    *_editPatroon->getDir(1) = _GUI->getSlidersBMapped(3);

    *_editPatroon->getPhase(0) = _GUI->getSlidersAMapped(4);
    *_editPatroon->getPhase(1) = _GUI->getSlidersBMapped(4);

    *_editPatroon->getPhaseFreq(0) = _GUI->getSlidersAMapped(5);
    *_editPatroon->getPhaseFreq(1) = _GUI->getSlidersBMapped(5);
    
    //_editPatroon->printPatroon();
}

void ArtnetControl::PlayPatronPressed(int & id)
{
    _livePatroon = &_patronen[id];
    //cout << "play patron pressed " << id << endl;
}
void ArtnetControl::EditPatronPressed(int & iD)
{
    //turn all of and only the right one on
    _editPatroon = &_patronen[iD];
    //update the GUI
    _GUI->getCurveSlidersA()->setValueMapped(float(*_editPatroon->getCurve(0)));
    _GUI->getCurveSlidersB()->setValueMapped(float(*_editPatroon->getCurve(1)));
    _GUI->getFreqSliderA()->setValueMapped(float(*_editPatroon->getFreq(0)));
    _GUI->getFreqSliderB()->setValueMapped(float(*_editPatroon->getFreq(1)));
    _GUI->getTimeSliderA()->setValueMapped(float(*_editPatroon->getTime(0)));
    _GUI->getTimeSliderB()->setValueMapped(float(*_editPatroon->getTime(1)));
    _GUI->getDirSliderA()->setValueMapped(float(*_editPatroon->getDir(0)));
    _GUI->getDirSliderB()->setValueMapped(float(*_editPatroon->getDir(1)));

    _GUI->getPhaseCurveSliderA()->setValueMapped(float(*_editPatroon->getPhase(0)));
    _GUI->getPhaseCurveSliderB()->setValueMapped(float(*_editPatroon->getPhase(1)));
    _GUI->getPhaseFreqSliderA()->setValueMapped(float(*_editPatroon->getPhaseFreq(0)));
    _GUI->getPhaseFreqSliderB()->setValueMapped(float(*_editPatroon->getPhaseFreq(1)));

    //set the colors from edit
    _GUI->colorselectorA.setColorFromIDs(_editPatroon->getColorID(0),_editPatroon->getColorID(1));
    _GUI->colorselectorB.setColorFromIDs(_editPatroon->getColorID(2),_editPatroon->getColorID(3));
    // Looop through the sequence steps and set sequence form pattern
    _GUI->getSegmenselectorA().clearStepsOfSegments();
    _GUI->getSegmenselectorB().clearStepsOfSegments();
    for(int i=0;i< getNumStepsSequence();i++)
    {
        _GUI->getSegmenselectorA().setSequence(i, _editPatroon->getSeqStepA(i));
        _GUI->getSegmenselectorB().setSequence(i, _editPatroon->getSeqStepB(i));
    }
}

void ArtnetControl::PlaySteppedPatronPressed(int & id)
{
    //todo make it stepped
    _livePatroon = &_patronen[id];
}
