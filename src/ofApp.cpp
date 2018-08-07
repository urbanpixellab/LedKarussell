#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    _LiveScene = new SceneControl(&_MC);
    _PrepareScene = new SceneControl(&_MC);
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
    _AC = new ArtnetControl(&_MC,_LiveScene);
}

//--------------------------------------------------------------
void ofApp::update(){
    _MC.update();
    _LiveScene->update();
    _PrepareScene->update();
    _AC->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    _LiveScene->drawGui();
    _PrepareScene->drawGui();
    
    //test visualizer
    // for every segment
    for (int uni = 0; uni< 8; uni++)
    {
        for (int x = 0; x < 150; x++)
        {
            ofSetColor(_AC->getNode(0)->universes[uni%2][x]);//only for preview must been done with setpixels, is faster
            ofDrawRectangle(x * ofGetWidth() / 150, uni * 10, ofGetWidth() / 150, 10);
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
    
    for (int i = 0; i < 8; i++)
    {
        delete _PresetScenes[i];
    }

    delete _PrepareScene;
    delete _LiveScene;
    delete _AC;
}