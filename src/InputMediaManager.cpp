//
//  InputMediaManager.cpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#include "InputMediaManager.hpp"
#include "OutputViewManager.hpp"

#include "InputSyphon.hpp"


InputMediaManager * InputMediaManager::instance(nullptr);

InputMediaManager::InputMediaManager():UIComponent("inputManager"),cnv("inputs"),selected(nullptr){

  vector<string> empty;
  ddlInputMedias = new ofxUIDropDownList("inputMedias",empty,100,100);
  cnv.addWidgetDown(ddlInputMedias);

  ddlInputMedias->setAutoClose(false);
  ddlInputMedias->open();

  cnv.setPosition(300, 300);
  cnv.newGUIEvent.add(this,&InputMediaManager::newGUIEvent,0);

  // syphon

  syphonDir.events.serverAnnounced.add(this,&InputMediaManager::newSyphonServer,0);
  syphonDir.events.serverRetired.add(this,&InputMediaManager::syphonServerRemoved,0);
  syphonDir.events.serverUpdated.add(this,&InputMediaManager::syphonServerUpdated,0);
  syphonDir.setup();




}

void InputMediaManager::update(){

};

void InputMediaManager::resized(){
  ofRectangle sB = getBoundsInRoot();
  int pad = 3;
  cnv.getRect()->x = sB.x;
  cnv.getRect()->y = sB.y + pad;
  cnv.getRect()->height = sB.height - 2*pad;
}

void InputMediaManager::paint(){
  ofRectangle b = getLocalBounds();
  ofPushStyle();
  if(selected){
    if(selected->hasTexture){
      selected->getTexture().draw(b);
    }
    selected->drawSubsection(b,b);
    if(OutputView * selO = OutputViewManager::i()->selected.get()){
      ofVec2f scale  = ofVec2f(b.getWidth(),b.getHeight())/ selected->getSize();
      int lidx = 0;
      ofNoFill();
      for(auto & l:selO->layers){
        if(l.media==selected){
          ofRectangle sR = l.inputRect;
          sR.scale(scale);
          sR.x*=scale.x;
          sR.y*=scale.y;
          ofSetColor(lidx%3==0?255:0,lidx%3==1?255:0,lidx%3==2?255:0,255);

          ofDrawRectangle(sR);
        }
      lidx++;
      }


    }
  }

  ofPopStyle();

};

void InputMediaManager::selectMedia(InputMedia::Sp m){
  selected = m;
}


InputMedia::Sp InputMediaManager::getSelectedMedia(){
  return selected;
}
shared_ptr<InputMedia> InputMediaManager::addMedia(unique_ptr<InputMedia> m){
  auto sp = shared_ptr<InputMedia>(std::move(m));
  allMedias.push_back(sp);
  mediaAdded.notify(sp);
  ddlInputMedias->addToggle(sp->name);
  return sp;

}

void InputMediaManager::removeMedia(InputMedia *m){
  if(m!=nullptr){
    {
      auto mediaIt = find_if(allMedias.begin(), allMedias.end(),
                             [m] (const shared_ptr<InputMedia>& s){return s.get()==m;});
      if(mediaIt!=allMedias.end()){
        mediaRemoved.notify(*mediaIt);
        ddlInputMedias->removeToggle((*mediaIt)->name);
        allMedias.erase(mediaIt);
      }
    }

    {
      auto mediaIt = find_if(allActiveMedias.begin(), allActiveMedias.end(),
                             [m] (const shared_ptr<InputMedia>& s){return s.get()==m;});
      if(mediaIt!=allActiveMedias.end()){
        allActiveMedias.erase(mediaIt);
      }
    }
  }



}
shared_ptr<InputMedia> InputMediaManager::getInputMediaForName(const string & n){
  auto mediaIt = find_if(allMedias.begin(), allMedias.end(),
                         [n] (const shared_ptr<InputMedia>& s){return s.get()->name==n;});
  if(mediaIt!=allMedias.end()){
    return *mediaIt;

  }

  return nullptr;

}



//////////////
// syphon

void InputMediaManager::newSyphonServer(ofxSyphonServerDirectoryEventArgs & a){
  //  for(auto & s:a.servers){
  //    addMedia(make_unique<InputSyphon>(s));
  //  }

}
void InputMediaManager::syphonServerUpdated(ofxSyphonServerDirectoryEventArgs & a){

}

void InputMediaManager::syphonServerRemoved(ofxSyphonServerDirectoryEventArgs & a){

}

//////////////
//ui

void InputMediaManager::newGUIEvent( ofxUIEventArgs & a){
  if(a.widget->getParent()==ddlInputMedias){
    if(ofxUIToggle * t = a.getToggle()){
      if(t->getValue()){
        string n = t->getName();
        auto mediaIt = find_if(allMedias.begin(), allMedias.end(),
                               [n] (const shared_ptr<InputMedia>& s){return s.get()->name==n;});
        if(mediaIt!=allMedias.end()){
          selectMedia( *mediaIt);
        }
      }
    }
  }
  
}
