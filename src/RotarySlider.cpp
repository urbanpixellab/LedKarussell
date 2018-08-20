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
//    font.load("verdana.ttf", 10);
    resolution = 128;
    comma = 0;
    active = true;
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
    if(markers)
    {
        int count = 6;// numbers of helplines
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
            //font.drawString(ofToString(i * (range.y - range.x) / float(count),0),x2,y2 );
        }
        ofNoFill();
        ofDrawCircle(circleCenter, radius * 0.8);
    }
    
    ofSetLineWidth(4);
    ofColor fg = ofColor(255,255,255);
    ofSetColor(fg);
    float offset = (TWO_PI * 0.375);
    
    if (type) // is line
    {
        ofSetLineWidth(4);
        float angle = offset + (TWO_PI * 0.75 * value);// from
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
    string number = ofToString(range.x + ((range.y - range.x) * value),comma);
//    float shift = font.getStringBoundingBox(number, 0, 0).width / 2.;
//    font.drawString(number, circleCenter.x - shift, fbo.getHeight() - 2);
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
        int dir = evt.x - xStart;
        if (dir > 0 && mouseValue < resolution)mouseValue++;
        else if (dir < 0 && mouseValue > 0 )mouseValue--;
        // notify or direct update the value
        value = ofMap(mouseValue, 0, resolution, 0, 1);
        value = ofClamp(value, 0, 1);
        //ofNotifyEvent(newValue, value);
        cout << "dragged" << value << endl;
        redraw = true;
        //xStart = evt.x;
    }
}
void RotarySlider::mousePressed(ofMouseEventArgs &evt)
{
    if (!active) return;
    if (drawArea.inside(evt.x, evt.y))
    {
        // anstieg
        //ofVec2f rCenter = ofVec2f(drawArea.x + drawArea.width * 0.5,drawArea.y + drawArea.height * 0.5);
        //ofVec2f cM = ofVec2f(evt.x, evt.y) - rCenter;//centerMouse
        //ofVec2f zero = ofVec2f(1,0);
        //float angle = zero.angle(cM) + 180;
        //cout << angle << endl;
        xStart = evt.x;
    }
}
void RotarySlider::mouseReleased(ofMouseEventArgs &evt)
{
    if (!active) return;
    xStart = -1;
}

void RotarySlider::reset()
{
    value = resetValue;
    mouseValue = resetValue * resolution;
    redraw = true;
}
