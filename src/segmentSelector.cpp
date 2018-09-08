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
    spacingVert = 20;   // offset buttons versus the text
    
    ofAddListener(ofEvents().mousePressed, this, &segmentSelector::mousePressed);
}

segmentSelector::~segmentSelector(){}


void segmentSelector::setup(ofRectangle area){
    drawArea = area;
    
    //cout << "SETUP Segmentselector..." << endl;
    
    ofxXmlSettings sel("selector.xml");
    numSelections = sel.getNumTags("select");
    
    int button_size = 20;
    int textLength = 100;
    int numSelections = sel.getNumTags("select");

    for (int i = 0; i < numSelections; i++)
    {
        sel.pushTag("select",i);
        Selection *s = new Selection();
        s->name = sel.getValue("name", "untitled");
        
        int x = drawArea.x + textLength;
        int y = drawArea.y + i * button_size;
        
        for(int j=0;j<SEQUENCESTEPS;j++)
        {
            Button *b = new Button;
            b->setup(ofRectangle(x+(j*button_size),y-spacingVert,button_size,button_size)," ",true);
            s->stepbuttons[j] = b;
        }
        selections.push_back(s);
        //cout << "select " << s->name << endl;
        sel.popTag();
    }
    
    // button to clear sequence
    clearButton = new Button();
    int x = drawArea.x + textLength + (SEQUENCESTEPS-2)*button_size;
    int y = drawArea.y + button_size/3+(numSelections-1) * button_size;
    clearButton->setup(ofRectangle(x,y,button_size*2.5,button_size*1.5),"clear",false);

}

void segmentSelector::clearSequence(int &id)
{
    clearStepsOfSegments();
}

void segmentSelector::draw(){
    
    ofPushStyle();
    for (int i = 0; i < selections.size(); i++)
    {
        int x = drawArea.getX();
        int y = drawArea.getY() + i * spacingVert;
        ofSetColor(255);
        verdana14.drawString(selections[i]->name, x, y);
        
        ofSetColor(128);
        for (int j = 0; j < SEQUENCESTEPS; j++){
            selections[i]->stepbuttons[j]->draw();
        }

    }
    clearButton->draw();
    ofPopStyle();

}

// i: is the step sequence: is an array with all the selections for that step
void segmentSelector::setSequence(int i,bool sequence[]){
    
    //cout << " i: " << i << " ";
    for (int j = 0; j < selections.size(); j++)
    {
            selections[j]->sequence[i] =  sequence[j];
            if(sequence[j])
            {
                selections[j]->stepbuttons[i]->setState(true);
            }
            //cout << sequence[j] << " ";
    }
    //cout << endl;
}

void segmentSelector::clearStepsOfSegments(){
    for (int i = 0; i < selections.size(); i++)
    {
        for (int j = 0; j < SEQUENCESTEPS; j++)
        {
            selections[i]->sequence[j] = false;
            selections[i]->stepbuttons[j]->setState(false);
        }
    }
}

void segmentSelector::mousePressed(ofMouseEventArgs &args)
{
    
    for (int i = 0; i < selections.size(); i++)
    {
        for (int j = 0; j < SEQUENCESTEPS; j++){
            
            if(selections[i]->stepbuttons[j]->getArea().inside(args.x, args.y))
            {
                selections[i]->sequence[j] = ! selections[i]->sequence[j];
                //cout << "HIERO " << selections[i]->name << " j " << ofToString(j) << " state " << selections[i]->sequence[j] <<  endl;
            }
            
        }
    }
    
    if(clearButton->getArea().inside(args.x, args.y))
    {
        clearStepsOfSegments();
    }
    
    _pressed = true;
    ofNotifyEvent(segmentPressed,_pressed);
    
    
    // Debug
    
   for (int i = 0; i < selections.size(); i++)
   {
        cout << selections[i]->name << " i: " << i << " ";
        for (int j = 0; j < SEQUENCESTEPS; j++){
                cout << ofToString(selections[i]->sequence[j]) << " ";
        }
        cout << endl;
    }
     
}
