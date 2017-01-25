#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
  
  mainComponent.setBounds(ofRectangle (0,0,ofGetWidth(),ofGetHeight()));
  


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
  mainComponent.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
  mainComponent.mouseMovedInternal(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  mainComponent.mousePressedInternal(x,y);

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
  mainComponent.mouseReleasedInternal(x,y);

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
  

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
  mainComponent.setBounds(ofRectangle(0,0,w,h));

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
