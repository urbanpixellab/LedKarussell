#include "ofApp.h"

// TODO: if we clear the segment selecot and then load the app all the sequence steps are set to 0
// FIXME: Saves different range for rotarysliders ?
//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetBackgroundColor(21);
    ofSetLineWidth(2);
    ofTrueTypeFont::setGlobalDpi(72);
    _AC = new ArtnetControl(&_MC);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    _MC.update();
    _AC->update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    
    ofSetBackgroundColor(21);
    _MC.drawGUI();
    _AC->drawGui();
    
    // TEMP
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
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
