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
    colorA = ofColor(255);
    colorB = ofColor(255);
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

void ColorSelector::setColorA(string &s){
    cout << "COLORSELECTOR A:: have an event " << s << endl;
    colorA = colors[ofToInt(s)];
}

void ColorSelector::setColorB(string &s){
    cout << "COLORSELECTOR B:: have an event " << s << endl;
    colorB = colors[ofToInt(s)];
}

void ColorSelector::colorSwap(string &s){
    ofColor a = colorA;
    ofColor b = colorB;
    
    colorA = b;
    colorB = a;
}


void ColorSelector::draw(){
    
    ofPushStyle();
    ofSetColor(colorA);
    ofFill();
    ofDrawRectRounded(drawArea.getX(), drawArea.getY(), drawArea.getWidth()/4, tilesize, 4);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(colorB);
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


