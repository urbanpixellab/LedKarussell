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
    _GUI->createAnimationGUI(LedAnimator::CURVE_COUNT);
    ofAddListener(_GUI->curvePressed, this, &ArtnetControl::guiCurvePressed);
    _preAnimator = new LedAnimator(_MC);
    _liveAnimator = new LedAnimator(_MC);
    loadNodes();
    //init the gui
    _GUI->init();
    _test = false;
    
    ofAddListener(ofEvents().keyPressed, this, &ArtnetControl::keyPressed);
}

ArtnetControl::~ArtnetControl()
{
    // blackout all
    ofRemoveListener(_GUI->curvePressed, this, &ArtnetControl::guiCurvePressed);
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

    loadPatroon();
    
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
                    cout << l << "selected indices " <<ofToInt(result[selection]) << endl;
                    if(l == 0) p.setSeqA(step, ofToInt(result[selection]), true);
                    else if(l == 1) p.setSeqB(step, ofToInt(result[selection]), true);
                }
            }
            sel.popTag();
        }
        _patronen.push_back(p);
        sel.popTag();
    }
    cout << _patronen.size() << endl;
}

void ArtnetControl::savePatroon(){}


void ArtnetControl::update()
{
    // update artnet based on the selected sce or use the hottbutton function
    // which is overriding the actual state
    //fill all nodes by selection
    float freq = 6;
    int direction = LedAnimator::FORWARD;
    bool solo = true; // solo means that every segment is treated seperate otherwise we melt it to one big array
    int selectionA = _test;
    int selectionB = 10;
    
    //first fill all with background color
    ofColor black(0,0,0);
    ofColor c1(255,0,0);
    ofColor c2(0,0,255);
    ofColor c3(0,255,0);
    ofColor c4(255,0,255);
    fillAllBackgroundColor(black);
    if(solo)
    {
        int s = _selections[selectionA].items.size();
        
        
        for (int i = 0; i < s; i++)
        {
            int seg = _selections[selectionA].items[i];
            
            _preAnimator->drawToArray(_curvePreview,direction,1, _preSegments[seg]->getArray(), _preSegments[seg]->getLength(),c1,c2);
            _liveAnimator->drawToArray(_curveLive,direction,1, _liveSegments[seg]->getArray(), _liveSegments[seg]->getLength(),c1,c2);
        }
        
        //add now the second color
        s = _selections[selectionB].items.size();
        for (int i = 0; i < s; i++)
        {
            int seg = _selections[selectionB].items[i];
            
            _preAnimator->addToArray(_curvePreview,direction,1, _preSegments[seg]->getArray(), _preSegments[seg]->getLength(),c3,c4);
            _liveAnimator->addToArray(_curveLive,direction,1, _liveSegments[seg]->getArray(), _liveSegments[seg]->getLength(),c3,c4);
        }
    }
    else
    {
        //all segments in one array as one long array, create them first after inloading of the selector
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
            u_int64_t data = _preSegments[i]->getArray()[first+cell];
//            u_int64_t data = _liveSegments[i]->getArray()[first+cell];
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

void ArtnetControl::guiCurvePressed(int &buttonid)
{
    _curvePreview = buttonid;
    _curveLive = buttonid; // must been changed later
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

void ArtnetControl::keyPressed(ofKeyEventArgs &key)
{
    _test++;
    _test = _test%12;
    cout << key.keycode << endl;
    
}
