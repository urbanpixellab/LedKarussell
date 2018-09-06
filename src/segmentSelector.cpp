//
//  segmentSelector.cpp
//  LedKarussell
//
//  Created by Machiel Veltkamp on 02/09/18.
//
//

#include "segmentSelector.hpp"


segmentSelector::segmentSelector()
{
    // iniate font
    fontSize = 10;
    verdana14.load("verdana.ttf", fontSize, true, true);
    spacingVert = 20;
}

segmentSelector::~segmentSelector(){}


void segmentSelector::setup(ofRectangle area){
    drawArea = area;
    
    cout << "SETUP Segmentselector..." << endl;
    
    ofxXmlSettings sel("selector.xml");
    numSelections = sel.getNumTags("select");
    
    int button_size = 20;

    for (int i = 0; i < sel.getNumTags("select"); i++)
    {
        sel.pushTag("select",i);
        Selection s;
        s.name = sel.getValue("name", "untitled");
        
        int x = drawArea.x + 150;
        int y = drawArea.y + i * button_size;
        
        for(int j=0;j<SEQUENCESTEPS;j++)
        {
            Button *b = new Button;
            b->setup(ofRectangle(x+(j*button_size),y,button_size,button_size),ofToString(i)+"-"+ofToString(j),true);
            s.stepbuttons[j] = b;
        }
        selections.push_back(s);
        cout << "select " << s.name << endl;
        sel.popTag();
    }

}



void segmentSelector::draw(){
    
    ofPushStyle();
    for (int i = 0; i < selections.size(); i++)
    {
        Selection s = selections[i];
        int x = drawArea.getX();
        int y = drawArea.getY() + i * spacingVert;
        ofSetColor(255);
        verdana14.drawString(s.name, x, y);
        
        ofSetColor(128);
        int l = sizeof(s.beats) / sizeof(s.beats[0]);
        for (int j = 0; j < l; j++){
            s.stepbuttons[j]->draw();
        }

    }
    ofPopStyle();

}
