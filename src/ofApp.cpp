#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
/* not needed here
    _PresetScenes[0] = new SceneControl(&_MC);
    _PresetScenes[1] = new SceneControl(&_MC);
    _PresetScenes[2] = new SceneControl(&_MC);
    _PresetScenes[3] = new SceneControl(&_MC);
    _PresetScenes[4] = new SceneControl(&_MC);
    _PresetScenes[5] = new SceneControl(&_MC);
    _PresetScenes[6] = new SceneControl(&_MC);
    _PresetScenes[7] = new SceneControl(&_MC);
    */
    _AC = new ArtnetControl(&_MC);
}

//--------------------------------------------------------------
void ofApp::update(){
    _MC.update();
    _AC->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    _AC->drawGui();
    //test visualizer
    // for every segment
    for (int s = 0; s< 8; s++) // acht sequenzen erstmal
    {
        for (int x = 0; x < 150; x++)
        {
            int r = int(_AC->getPreSegment(8)->getArray()[(x*3) + 0]);
            int g = int(_AC->getPreSegment(8)->getArray()[(x*3) + 1]);
            int b = int(_AC->getPreSegment(8)->getArray()[(x*3) + 2]);
            ofSetColor(r,g,b);
            ofDrawRectangle(x * ofGetWidth() / 150, s * 10, ofGetWidth() / 150, 10);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == ' ')
    {
        int select = ofRandom(LedAnimator::COUNT);
        _AC->setAnimation(select);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit()
{
    
    delete _AC;
}