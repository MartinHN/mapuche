//
//  VideoLayerManager.cpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#include "VideoLayerManager.hpp"

#include "InputMediaManager.hpp"
VideoLayerManager::VideoLayerManager():UIComponent("videoLayerManager"){
  buildFromOutView(shared_ptr<OutputView>(nullptr));
};



void VideoLayerManager::buildFromOutView(shared_ptr<OutputView> _outView){
  outView = _outView;
  if(outView){
//  cnv = make_unique<ofxUISuperCanvas>(outView->name+" layers",200,200,200,200);
//  addLayerB = new ofxUIButton("add",false,10,10);
//  removeLayerB= new ofxUIButton("remove",false,10,10);
//  std::vector<string> dummy;
//  layerDDL = new ofxUIDropDownList("layers",dummy,cnv->getRect()->width -20,100);
//
//  layerDDL->open();
//  //  views->setAllowMultiple(true);
//
//
//  cnv->addWidgetDown(addLayerB);
//  cnv->addWidgetRight(removeLayerB);
//  cnv->addWidgetDown(layerDDL);
//
//  cnv->newGUIEvent.add(this, &VideoLayerManager::newGUIEvent, 0);
//    cnv->getRect()->setParent(nullptr);
  }
  else{
    cnv = nullptr;
  }


  
  
}

void VideoLayerManager::resized(){
  ofRectangle sB = getBoundsInRoot();
  const int pad = 3;
  if(cnv){
  cnv->getRect()->x = sB.x ;
  cnv->getRect()->y = sB.y + pad;
  cnv->getRect()->height = sB.height - 2*pad;
  
    cout << " : "<< cnv->getRect()->x << ","<<cnv->getRect()->y << "," << cnv->getRect()->width << ", "<< cnv->getRect()->height << endl;
  }
};

vector <string> VideoLayerManager::getSelectedLayers(){
  return layerDDL->getSelectedNames();

}


void VideoLayerManager::addLayer(const string &name){
  outView->addMediaLayer(InputMediaManager::i()->getSelectedMedia());
  
}
void VideoLayerManager::removeLayer(const string & name){

}
void VideoLayerManager::selectLayer(const string & name){

}

int layerNum = 0;
void VideoLayerManager::newGUIEvent(  ofxUIEventArgs & a){
  if(a.widget==addLayerB && !addLayerB->getValue()){
    string name = ofToString(layerNum);
    addLayer(name);
    layerDDL->addToggle(name);
    layerNum++;
  }
  else if(a.widget==removeLayerB && !removeLayerB->getValue()){
    for(auto & n:getSelectedLayers()){
      removeLayer(n);
      layerDDL->removeToggle(n);
      layerNum--;
    }

  }
  else if(a.widget->getParent() == layerDDL){
    if( ofxUIToggle * t = a.getToggle()){
      if(t->getValue())
        selectLayer(a.widget->getName());
    }
  }
}


