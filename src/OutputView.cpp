//
//  OutputView.cpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#include "OutputView.hpp"




OutputView::OutputView(const string & _name) :
name(_name){
//  static shared_ptr<ofMainLoop> * mainLoop(new shared_ptr<ofMainLoop>(new ofMainLoop));
  ofGLFWWindowSettings settings;
  settings.width = 300;
  settings.height = 300;
  settings.decorated = false;
  settings.setPosition(ofVec2f(0,0));
//  settings.windowMode  = ofWindowMode::OF_GAME_MODE;
  window = ofCreateWindow(settings);


  ofRunApp(window, shared_ptr<ofBaseApp> (this));

//  addMediaLayer(make_shared<DummyMedia>("dum"+name));
}

OutputView::~OutputView(){

}


void OutputView::draw(){

  for(auto & l:layers){
    l.draw();
  }

}


void OutputView::addMediaLayer(shared_ptr<InputMedia> media  ){
  layers.push_back(VideoLayer(media));
  VideoLayer * cL  =&layers[layers.size() - 1];

  cL->destRect = ofRectangle(0,0,window->getWidth(),window->getHeight());

};

void OutputView::windowResized(int w, int h){
  
  for(auto & l:layers){
    l.destRect.setWidth(w);
    l.destRect.setWidth(h);
  }
};


///////////////////
//

