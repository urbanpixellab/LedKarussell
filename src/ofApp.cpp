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
void ofApp::draw()
{
    _MC.drawGUI();
    _AC->drawGui();
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
