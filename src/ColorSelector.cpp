//
//  ColorSelector.cpp
//  LedKarussell
//
//  Created by Machiel Veltkamp on 02/09/18.
//
//


/*
 Read the colors from the XML file an ddraw them on the screen..
 When pressed sent color to the right clas/instance
 */

#include "ColorSelector.hpp"


ColorSelector::ColorSelector()
{
    numCollumns = 6;
    padding = 6;
}


ColorSelector::~ColorSelector(){}


void ColorSelector::setup(ofRectangle area)
{
    // Where is the color palette been drawn
    drawArea = area;
    // --> load colors form XML
    loadColors();
    
    // setup colorswap button
    int x = drawArea.getX()+(numCollumns-2)*tilesize;
    int y = drawArea.getY();
    colorSwapButton.setup(ofRectangle(x,y,tilesize*2,tilesize),"color_swap",false);
    ofAddListener(colorSwapButton.buttonPressed, this, &ColorSelector::colorSwap);
}

void ColorSelector::loadColors(){
    
    ofxXmlSettings XMLcolors("colors.xml");
    int count = XMLcolors.getNumTags("color");
    
    tilesize = drawArea.getWidth() / (numCollumns*2);
    
    Button *buttonA;
    Button *buttonB;

    
    for (int i = 0; i < count; i++)
    {
        XMLcolors.pushTag("color",i);
        
        int red     = ofToInt( XMLcolors.getValue("red","255"));
        int green   = ofToInt( XMLcolors.getValue("green","255"));
        int blue    = ofToInt( XMLcolors.getValue("blue","255"));
        
        cout << "i " << i << "red " << red << " green " << green << " blue " << blue << endl;
        
        // add color to color vector
        ofColor c(red,green,blue);
        colors.push_back(c);
       
        // FIXME: clean code
        // FIXME: active color of button "stciks' doe snot disapear after timeout
        
        // Create Buttons for color pallete
        // Calculate position for button
        int xa = drawArea.getX()+ (i%numCollumns) *tilesize;
        int ya = drawArea.getY() + tilesize + ((i/numCollumns) * tilesize);
    
        buttonA = new Button();
        buttonA->setup(ofRectangle(xa,ya,tilesize,tilesize),ofToString(i),false);
        buttonA->setColors(c,c,ofColor(0));
        ofAddListener(buttonA->buttonPressed, this, &ColorSelector::setColorA);
        buttonsA.push_back(*buttonA);
        
        // Calculate position for button
        int xb = 10+drawArea.getWidth()/2 + drawArea.getX()+ (i%numCollumns) *tilesize;
        int yb = drawArea.getY() + tilesize + ((i/numCollumns) * tilesize);

        buttonB = new Button();
        buttonB->setup(ofRectangle(xb,yb,tilesize,tilesize),ofToString(i),false);
        buttonB->setColors(c,c,ofColor(0));
        ofAddListener(buttonB->buttonPressed, this, &ColorSelector::setColorB);
        buttonsA.push_back(*buttonB);
        
        XMLcolors.popTag();
    }
}


void ColorSelector::setColorIDs(int ids[]){
    for(int i=0;i<2;i++){
        _selectedColorIDs[i] = ids[i];
        _selectedColors[i]      = colors[ids[i]];
    }
}


void ColorSelector::setColorA(string &s)
{    
//    _selectedColorIDs[0]   = ofToInt(s);
//    _selectedColors[0]      = colors[_selectedColorIDs[0]];

//    string t = ofToString(_selectedColorIDs[0])+","+ofToString(_selectedColorIDs[1]);
//    ofNotifyEvent(colorChosen, t);
    _pressed = true;
    ofNotifyEvent(colorPressed,_pressed);

}

void ColorSelector::setColorB(string &s)
{
//    _selectedColorIDs[1]   = ofToInt(s);
//    _selectedColors[1]      = colors[_selectedColorIDs[1]];
    
//    string t = ofToString(_selectedColorIDs[0])+","+ofToString(_selectedColorIDs[1]);
//    ofNotifyEvent(colorChosen, t);
    _pressed = true;
    ofNotifyEvent(colorPressed,_pressed);
    
}

void ColorSelector::colorSwap(string &s){
    ofColor a = _selectedColors[0];
    ofColor b = _selectedColors[1];
    
    _selectedColors[0] = b;
    _selectedColors[1] = a;
    _pressed = true;
    ofNotifyEvent(colorPressed,_pressed);
}

void ColorSelector::draw(){
    
    ofPushStyle();
    ofSetColor(_selectedColors[0]);
    ofFill();
    ofDrawRectRounded(drawArea.getX(), drawArea.getY(), drawArea.getWidth()/4, tilesize, 4);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(_selectedColors[1]);
    ofFill();
    ofDrawRectRounded(10+drawArea.getX()+drawArea.getWidth()/2, drawArea.getY(), drawArea.getWidth()/4, tilesize, 4);
    ofPopStyle();

    colorSwapButton.draw();
    
    for ( auto a : buttonsA ) {
        a.draw();
    }
    
    for ( auto b : buttonsB ) {
        b.draw();
    }
    
}


