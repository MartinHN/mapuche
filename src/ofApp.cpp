#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

  mainComponent.setBounds(ofRectangle (0,0,ofGetWidth(),ofGetHeight()));
  ofFile f("Mapping.xml");
  if(f.exists()){
    toLoad = f.getAbsolutePath();
  }
  else{
  toLoad = "";
  }
//  ofSetFrameRate(30);
//  ofSetVerticalSync(true);

}

//--------------------------------------------------------------
void ofApp::update(){
  if(toLoad!=""){

    openArenaLayout(toLoad);
    toLoad = "";

  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  mainComponent.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  switch (key){
    case 'o':{
      ofFileDialogResult res  = ofSystemLoadDialog();
      if(res.bSuccess){
        toLoad = res.filePath;
      }
    }
      break;
  }

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

inline ofRectangle  rectFromVec(vector<ofVec2f>  & inP){
  ofRectangle res;
  res.setPosition(inP[0].x,inP[0].y);
  res.setSize(inP[2].x-inP[0].x, inP[2].y-inP[0].y);
  return res;


}



void ofApp::openArenaLayout(const string & path){
  ofXml xml;
  if(!xml.load(path)){
    ofLogError() << "Couldn't load file";
  }
  else{
    auto screens =  xml.find("//XmlState/ScreenSetup/screens/Screen");
    int dbg = 0;
    ofPoint mediaSize (7200,1080);

    mainComponent.vMgr->addView("big",ofGetScreenWidth(),0,3840*2,1080);
    ofVec2f outScale (1,1);

//    mainComponent.vMgr->addView("big",0,0,3840*2,1080);
//    mainComponent.vMgr->addView("big",0,0,3840/2,1080);
//    ofVec2f outScale (.25,1);
    int screenOffset = 0;
    for(auto &s:screens){
      auto slices = s.find("layers/Slice");
      string sName = s.getAttribute("name").getValue();

      mainComponent.vMgr->selectView("big");
      for(auto & sl:slices){


        VideoLayer * l = mainComponent.vMgr->selected->addMediaLayer(mainComponent.iMgr->getSelectedMedia());

        vector<ofVec2f> inP;
        auto inRect = sl.find("InputRect/v");
        for(auto &p:inRect){
          inP.push_back(ofPoint(p.getAttribute("x").getDoubleValue() ,p.getAttribute("y").getDoubleValue() ));

        }

        vector<ofPoint> outP;
        auto outRect = sl.find("Warper/Homography/dst/v");
        for(auto &p:outRect){
          outP.push_back(ofPoint(p.getAttribute("x").getDoubleValue()+screenOffset,p.getAttribute("y").getDoubleValue() )* outScale );
        }
        l->setInputRect(rectFromVec(inP));
        l->setOutputPoints(outP);
        






      }
      screenOffset+=3840;
      
    }
    mainComponent.vMgr->selected->updateOverlaps();
    cout << dbg << endl;
  }
  
}
