//
//  RotarySlider.cpp
//  Modulator
//
//  Created by Enrico Becker on 27/08/16.
//
//

#include "RotarySlider.hpp"

RotarySlider::RotarySlider()
{
    ofAddListener(ofEvents().mouseDragged, this, &RotarySlider::mouseDragged);
    ofAddListener(ofEvents().mousePressed, this, &RotarySlider::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &RotarySlider::mouseReleased);
    xStart = -1;
    font[0].load("verdana.ttf", 8);
    font[1].load("verdana.ttf", 12);
    resolution = 128;
    comma = 0;
    active = false;
    value = 1;
    valueMapped = value;
    name = "";
    _isInt = false;
}

RotarySlider::RotarySlider(ofRectangle area,ofVec2f minMax,float startValue,bool type,int isInt,int res, string n):name(n),_isInt(isInt),resolution(res)
{
    ofAddListener(ofEvents().mouseDragged, this, &RotarySlider::mouseDragged);
    ofAddListener(ofEvents().mousePressed, this, &RotarySlider::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &RotarySlider::mouseReleased);
    xStart = startValue;
    font[0].load("verdana.ttf", 8);
    font[1].load("verdana.ttf", 12);
    comma = 0;
    active = false;
    value = 1;
    valueMapped = value;
    setup(area, minMax, startValue, type);
    //cout << "name is " << name << endl;
}


RotarySlider::~RotarySlider()
{
    ofRemoveListener(ofEvents().mouseDragged, this, &RotarySlider::mouseDragged);
    ofRemoveListener(ofEvents().mousePressed, this, &RotarySlider::mousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &RotarySlider::mouseReleased);
}

void RotarySlider::setup(ofRectangle area,ofVec2f minMax,float startValue,bool t)
{
    resolution = 128;
    range = minMax;
    resetValue = startValue;
    value = startValue;
    // reverse the mapping to set the mouse value
    mouseValue = startValue * resolution;
    type = t;
    drawArea = area;
    radius = drawArea.width / 3;
    // 0-270
    fbo.allocate(area.width, area.height);
    fbo.begin();
    ofClear(0,0,0);
    fbo.end();
    circleCenter = ofVec2f(fbo.getWidth() / 2,fbo.getHeight() * 2 / 5);
    redraw = true;
    valueMapped = ofMap(value,0,1,range.x,range.y);

}

void RotarySlider::setup(ofRectangle area,ofVec2f minMax,float startValue,bool t,int commaSize,int res)
{
    resolution = res;
    range = minMax;
    resetValue = startValue;
    value = startValue;
    // reverse the mapping to set the mouse value
    mouseValue = startValue * resolution;
    type = t;
    drawArea = area;
    radius = drawArea.width / 3;
    // 0-270
    fbo.allocate(area.width, area.height);
    fbo.begin();
    ofClear(0,0,0);
    fbo.end();
    circleCenter = ofVec2f(fbo.getWidth() / 2,fbo.getHeight() * 2 / 5);
    redraw = true;
    valueMapped = ofMap(value,0,1,range.x,range.y);
}


void RotarySlider::setup(ofRectangle area,ofVec2f minMax,float startValue,bool t,int c)
{
    comma = c;
    setup(area, minMax, startValue, t);
}


void RotarySlider::updateSlider()
{
    // create a radial line
    

    fbo.begin();
    ofClear(0,0,0);
    ofColor bg = ofColor(0, 0, 0);
    ofSetColor(bg);
    ofDrawRectangle(0,0,fbo.getWidth(),fbo.getHeight());
    // draw markers
    bool markers = true;
    int count = range.y-range.x;// numbers of helplines
    if(markers)
    {
        ofColor mc = ofColor(255);
        ofSetColor(mc);
        ofSetLineWidth(1);
        float shift = 270 / float(count);
        for (int i = 0; i < count + 1; i++)
        {
            float angle = (0.375 + i * shift / 360.) * TWO_PI;
            float x1 = circleCenter.x + (radius * cos(angle));
            float y1 = circleCenter.y + (radius * sin(angle));
            float x2 = circleCenter.x + (1.3 * radius * cos(angle));
            float y2 = circleCenter.y + (1.3 * radius * sin(angle));
            ofDrawLine(x1, y1, x2, y2);
            if(i < _bezeichner.size()) font[0].drawString(_bezeichner[i],x2,y2 );
        }
        ofNoFill();
        ofDrawCircle(circleCenter, radius * 0.8);
        //noch nen anderer angeschnittener circle
        
    }
    
    ofSetLineWidth(4);
    ofColor fg = ofColor(255,255,255);
    ofSetColor(fg);
    float offset = (TWO_PI * 0.375);
    
    if (type) // is line
    {
        float shift = 270 / float(count);

        ofSetLineWidth(4);
        float angle = offset + (TWO_PI * 0.75 * value);// from
        if (_isInt)
        {
            int i = ofMap(value,0,1, range.x, range.y);
            angle = (0.375 + i * shift / 360.) * TWO_PI;
        }
        float x1 = circleCenter.x + (radius * 0.8 * cos(angle));
        float y1 = circleCenter.y + (radius * 0.8 * sin(angle));
        ofDrawLine(circleCenter.x, circleCenter.y,x1,y1);
    }
    else // is mesh
    {
        /*
        ofMesh m;
        m.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        int red = 0;
        int blue = 255;
        for (int i = 0; i < (resolution + 1) * (0.75 * mapValue / (range.y - range.x)); i++)
        {
            blue -=(255 / (resolution + 1));
            red += (255 / (resolution + 1));
            float angle = (TWO_PI * 0.375) + (TWO_PI * i / float(resolution));// from
            float x1 = centerFbo.x + (radius * cos(angle));
            float y1 = centerFbo.y + (radius * sin(angle));
            float x2 = centerFbo.x + (radius * 0.5 * cos(angle));
            float y2 = centerFbo.y + (radius * 0.5 * sin(angle));
            m.addVertex(ofVec2f(x1, y1));
            ofColor outerColor = ofColor(red,0,blue);
            m.addColor(outerColor);
            m.addVertex(ofVec2f(x2, y2));
            ofColor innerColor = ofColor(red - i,255 - i,blue - i);
            m.addColor(innerColor);
        }
        m.draw();
         */
    }
    float shift = font[1].getStringBoundingBox(name, 0, 0).width / 2.;
    font[1].drawString(name, circleCenter.x - shift, fbo.getHeight() - 2);
    //ofDrawBitmapString(name, 10, fbo.getHeight());
    fbo.end();

    
    redraw = false;
}

void RotarySlider::draw()
{
    if(redraw) updateSlider();
    fbo.draw(drawArea);
}

void RotarySlider::mouseDragged(ofMouseEventArgs &evt)
{
    if (!active) return;

    if (xStart > -1)
    {
        //mappe mal die entfernung von ursprung im radius von
        // anstieg, muss in die maus richtung drehen wo der zeiger hindreh vom ursprung
        
        int dir = evt.x - xStart;
        if (dir > 0 && mouseValue < resolution)mouseValue += 4;
        else if (dir < 0 && mouseValue > 0 )mouseValue -= 4;
        // notify or direct update the value
        value = ofMap(mouseValue, 0, resolution, 0, 1);
        //cout << value << endl;
        value = ofClamp(value, 0, 1);
        valueMapped = ofMap(value,0,1,range.x,range.y);
        redraw = true;
        //xStart = evt.x;

        ofNotifyEvent(newValue, active);
    }
}
void RotarySlider::mousePressed(ofMouseEventArgs &evt)
{
    if (drawArea.inside(evt.x, evt.y))
    {
        //ofVec2f rCenter = ofVec2f(drawArea.x + drawArea.width * 0.5,drawArea.y + drawArea.height * 0.5);
        //ofVec2f cM = ofVec2f(evt.x, evt.y) - rCenter;//centerMouse
        //ofVec2f zero = ofVec2f(1,0);
        //float angle = zero.angle(cM) + 180;
        //cout << angle << endl;
        xStart = evt.x;
        active = true;
        valueMapped = ofMap(value,0,1,range.x,range.y);
        ofNotifyEvent(newValue, active);
    }
}
void RotarySlider::mouseReleased(ofMouseEventArgs &evt)
{
    if (!active) return;
    xStart = -1;
    active  = false;
}

void RotarySlider::addBezeichner(string * bez,int len)
{
    for (int i = 0; i < len; i++)
    {
        _bezeichner.push_back(bez[i]);
    }
}


void RotarySlider::reset()
{
    value = resetValue;
    valueMapped = ofMap(value,0,1,range.x,range.y);
    mouseValue = resetValue * resolution;
    redraw = true;
    ofNotifyEvent(newValue, active);

}

void RotarySlider::setValueMapped(float val)
{
    // valueMapped is the "raw" value which stays between range.x and range.y
    valueMapped = val;
    // value is the internal value for the slider which is always between 0-1
    value = ofMap(val, range.x, range.y, 0., 1.);
    updateSlider();
}
