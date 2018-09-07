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
    _GUI = new AnimatorGUI(ofRectangle(200,50,500,500));
    _GUI->createAnimationGUI(LedAnimator::CURVE_COUNT);

    _preAnimator = new LedAnimator(_MC);
    _liveAnimator = new LedAnimator(_MC);
    
    //init the gui
    _test = false;
    
    // Load all the nodes form XML
    loadNodes();
    
    // Load aal the patterns from XML
    loadPatroon();
    
    // set colors from editPatroon to color selector
    vector<int> colIDs = _editPatroon->getColorIDs();
    cout <<"HIER "<<  colIDs[0] << " " << colIDs[1] << " "<< colIDs[2] << " "<< colIDs[3] << " "<< endl;
    int a[] = {colIDs[0],colIDs[1]};
    int b[] = {colIDs[2],colIDs[3]};
    
    // FIXME: probably not the best way to do it.
    // SET SEGMENT SELECTOR FORM EDIT PATROON
    for(int i=0;i<8;i++)
    {
        _GUI->segmenselectorA.setSequence(i,_editPatroon->getSeqStepA(i));
        _GUI->segmenselectorB.setSequence(i,_editPatroon->getSeqStepB(i));
    }
    
    
    //Listeners
    
    ofAddListener(ofEvents().keyPressed, this, &ArtnetControl::keyPressed);
    //ad listeners to the patroon select buttons for different functions
    ofAddListener(_GUI->curveAPressed, this, &ArtnetControl::curveAPressed);
    ofAddListener(_GUI->curveBPressed, this, &ArtnetControl::curveBPressed);

    ofAddListener(_GUI->patronPLAY, this, &ArtnetControl::PlayPatronPressed);
    ofAddListener(_GUI->patronEDIT, this, &ArtnetControl::EditPatronPressed);
    ofAddListener(_GUI->patronPLAYSTEPPED, this, &ArtnetControl::PlaySteppedPatronPressed);
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
    ofAddListener(_GUI->getSegmenselectorA().segmentPressed, this, &ArtnetControl::segmentSelectPressed);
    ofAddListener(_GUI->getSegmenselectorA().segmentPressed, this, &ArtnetControl::segmentSelectPressed);

}

ArtnetControl::~ArtnetControl()
{
    //save everything
    
    // blackout all
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
        cout << n->ip << " " << n->name << endl;
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
        _GUI->getMesh().addTexCoord(ofVec2f(0,i + offset)); // offset of 9 rows
        y = 1-y;
        _GUI->getMesh().addVertex(ofVec2f(x,y));
        _GUI->getMesh().addTexCoord(ofVec2f(150,i + offset));
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
        
        for (int l = 0; l < sel.getNumTags("layer"); l++)
        {
            sel.pushTag("layer",l);
            curve[l] = sel.getValue("curve", 0);
            freq[l] = sel.getValue("cFreq", 1);
            dir[l] = sel.getValue("cFreq", 0);
            time[l] = sel.getValue("cTime", 2);
            color[(l * 2) + 0] = sel.getValue("colorA", 0);
            color[(l * 2) + 1] = sel.getValue("colorB", 0);
            sel.popTag();
        }
        Patroon p(id,curve[0],curve[1],freq[0],freq[1],dir[0],dir[1],time[0],time[1],color[0],color[1],color[2],color[3]);
        // add the sequence matrix based on the indices
        //vector<int>
        string steps[8] = {"step0","step1","step2","step3","step4","step5","step6","step7"};
        for (int l = 0; l < sel.getNumTags("layer"); l++)
        {
            sel.pushTag("layer",l);
            for (int step = 0; step < 8; step++)
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
    ofxXmlSettings settings;
    int maxSel = 13;
    int maxStep = 8;
    for (int p = 0; p < _patronen.size(); p++)//patron
    {
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
                for (int s = 0; s < maxStep; s++)
                {
                    string step = "";
                    for (int sel = 0; sel < maxSel; sel++)
                    {
                        if(_patronen[p].getSeqStepB(s)[sel] == true) step += ofToString(sel) + ",";
                    }
                    if(step.length() > 1)step.erase(step.length()-1);
                    settings.addValue("step"+ofToString(s), step);
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
    
    float *freqA = _editPatroon->getFreq(0);
    float *freqB = _editPatroon->getFreq(1);
    
    bool solo = true; // solo means that every segment is treated seperate otherwise we melt it to one big array
    int selectionA = _test; // take selection a from patroon
    int selectionB = 10; // take selection b from patroon
    
    //first fill all with background color
    ofColor black(0,0,0);
    
    _GUI->getSegmenselectorA().getSequence(0);
    
    // Get colors from editPatroon
    vector<int> getColorIDs = _editPatroon->getColorIDs();
    ofColor c1 = _GUI->getColorselectorA().getColorFromID(getColorIDs[0]);
    ofColor c2 = _GUI->getColorselectorA().getColorFromID(getColorIDs[1]);
    ofColor c3 = _GUI->getColorselectorA().getColorFromID(getColorIDs[2]);
    ofColor c4 = _GUI->getColorselectorA().getColorFromID(getColorIDs[3]);
    // to do add index shift function to phaseshift the curve from index by a curve and freq
    fillAllBackgroundColor(black);
    if(solo)
    {
        int s = _selections[selectionA].items.size();
        for (int i = 0; i < s; i++)
        {
            int seg = _selections[selectionA].items[i];
            _preAnimator->drawToArray(*_editPatroon->getCurve(0),*_editPatroon->getDir(0),*_editPatroon->getTime(0),*freqA, _preSegments[seg]->getArray(), _preSegments[seg]->getLength(),c1,c2);
        }
        
        //add now the second color
        s = _selections[selectionB].items.size();
        for (int i = 0; i < s; i++)
        {
            int seg = _selections[selectionB].items[i];
            _preAnimator->addToArray(*_editPatroon->getCurve(1),*_editPatroon->getDir(1),*_editPatroon->getTime(1),*freqB, _preSegments[seg]->getArray(), _preSegments[seg]->getLength(),c3,c4);
        }
    }
    
    // do the live led animator
    //read everything from live
    freqA = _livePatroon->getFreq(0);
    freqB = _livePatroon->getFreq(1);
    
    solo = true; // solo means that every segment is treated seperate otherwise we melt it to one big array
    selectionA = _test; // take selection a from patroon
    selectionB = 10; // take selection b from patroon
    
    //first fill all with background color
    //ofColor black(0,0,0);
    
    // Get colors from editPatroon
    getColorIDs.clear();
    getColorIDs = _livePatroon->getColorIDs();
    c1 = _GUI->getColorselectorA().getColorFromID(getColorIDs[0]);
    c2 = _GUI->getColorselectorA().getColorFromID(getColorIDs[1]);
    c3 = _GUI->getColorselectorB().getColorFromID(getColorIDs[2]);
    c4 = _GUI->getColorselectorB().getColorFromID(getColorIDs[3]);

    // to do add index shift function to phaseshift the curve from index by a curve and freq
    if(solo)
    {
        int s = _selections[selectionA].items.size();
        
        for (int i = 0; i < s; i++)
        {
            int seg = _selections[selectionA].items[i];
            _liveAnimator->drawToArray(*_livePatroon->getCurve(0),*_livePatroon->getDir(0),*_livePatroon->getTime(0),*freqA, _liveSegments[seg]->getArray(), _liveSegments[seg]->getLength(),c1,c2);
        }
        
        //add now the second color
        s = _selections[selectionB].items.size();
        for (int i = 0; i < s; i++)
        {
            int seg = _selections[selectionB].items[i];
            _liveAnimator->addToArray(*_livePatroon->getCurve(1),*_livePatroon->getDir(1),*_livePatroon->getTime(1),*freqB, _liveSegments[seg]->getArray(), _liveSegments[seg]->getLength(),c3,c4);
        }
    }
    
    

    //dont forget the fade out function to the beat and the blink function

    // now updaste the arrays to the visualizer
    writeSegmentsToImage(_preSegments, _preIMG);
    writeSegmentsToImage(_liveSegments, _liveIMG);
    
    sendToNodes();
}

void ArtnetControl::fillAllBackgroundColor(ofColor & color)
{
    for (int i = 0; i < _preSegments.size(); i++)
    {
        _preSegments[i]->setAllPixel(color);
        _liveSegments[i]->setAllPixel(color);
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
    //copy segment arrays to nodes
    int nodeID,universe,first,last = 0;
    for (int i = 0; i < _liveSegments.size(); i++)
    {
        //each segment is drawn by the Segment abstraction
        
        nodeID = _liveSegments[i]->getNodeID();
        universe = _liveSegments[i]->getUniverse();
        first = _liveSegments[i]->getBegin();
        last = _liveSegments[i]->getEnd();
        for (int cell = first; cell < last; cell++)
        {
            //get the array where to write which function
            //and write it to the nodes
//            u_int64_t data = _preSegments[i]->getArray()[first+cell];
            u_int64_t data = _liveSegments[i]->getArray()[first+cell];
            // mach was damit
            _nodes[nodeID]->universes[universe][cell] = data;//_liveSegments[i]->getArray()[first+cell];
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

void ArtnetControl::segmentSelectPressed(bool &pressed)
{
    //we have selected a segment in a seguence sent to edit patroon
    cout << " we need to set the editpatroon " << endl;
    
    // FIXME: Why do we need the getSegmenselectorA for the call?
    // FIXME: do we need a function in patroon to set the whole sequence in one go?
    // Loop through segment selection
    for(int s=0; s < 14; s++)
    {
        // loop through sequence step
        for(int i=0;i< 8;i++){
            // get the sequence step i for segemnt s
            bool valueA = _GUI->getSegmenselectorA().getSequence(s)[i];
            bool valueB = _GUI->getSegmenselectorB().getSequence(s)[i];
            _editPatroon->setSeqA(i, s, valueA);
            _editPatroon->setSeqB(i, s, valueB);
        }
    }
    
    // First _editpatroon to the patronen
    _patronen[_editPatroon->getID()] = *_editPatroon;
    
    
}



void ArtnetControl::keyPressed(ofKeyEventArgs &key)
{
    _test++;
    _test = _test%14;
    cout << key.keycode << endl;
    
}

void ArtnetControl::sliderChanged(bool & value)
{
    *_editPatroon->getCurve(0) = int(_GUI->getSlidersAMapped(0));
    *_editPatroon->getCurve(1) = int(_GUI->getSlidersBMapped(0));
    
    *_editPatroon->getFreq(0) = _GUI->getSlidersAMapped(2);
    *_editPatroon->getFreq(1) = _GUI->getSlidersBMapped(2);

    *_editPatroon->getTime(0) = _GUI->getSlidersAMapped(1);
    *_editPatroon->getTime(1) = _GUI->getSlidersBMapped(1);

    *_editPatroon->getDir(0) = _GUI->getSlidersAMapped(3);
    *_editPatroon->getDir(1) = _GUI->getSlidersBMapped(3);
}

void ArtnetControl::PlayPatronPressed(int & id)
{
    _livePatroon = &_patronen[id];
    cout << "play patron pressed " << id << endl;
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

    //set the colors from edit
    _GUI->colorselectorA.setColorFromIDs(_editPatroon->getColorID(0),_editPatroon->getColorID(1));
    _GUI->colorselectorB.setColorFromIDs(_editPatroon->getColorID(2),_editPatroon->getColorID(3));
    // Looop through the sequence steps and set sequence form pattern
    _GUI->getSegmenselectorA().clearStepsOfSegments();
    _GUI->getSegmenselectorB().clearStepsOfSegments();
    for(int i=0;i< 8;i++)
    {
        _GUI->getSegmenselectorA().setSequence(i, _editPatroon->getSeqStepA(i));
        _GUI->getSegmenselectorB().setSequence(i, _editPatroon->getSeqStepB(i));
    }
    
    cout << "edit patron pressed " << iD << endl;
}

void ArtnetControl::PlaySteppedPatronPressed(int & id)
{
    //todo make it stepped
    _livePatroon = &_patronen[id];
    cout << "play stepped patron pressed " << id << endl;
}
