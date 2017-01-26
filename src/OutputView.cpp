//
//  OutputView.cpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#include "OutputView.hpp"




OutputView::OutputView(const string & _name):name(_name){};

shared_ptr<OutputView> OutputView::generate(const string & _name,int x, int y,int w,int h)
{
//  static shared_ptr<ofMainLoop> * mainLoop(new shared_ptr<ofMainLoop>(new ofMainLoop));
  ofGLFWWindowSettings settings;
  settings.width = w;
  settings.height = h;
  settings.decorated = false;
  settings.setPosition(ofVec2f(x,y));
  settings.shareContextWith = ofGetCurrentWindow();

//  settings.windowMode  = ofWindowMode::OF_GAME_MODE;
  shared_ptr<ofAppBaseWindow> _window = ofCreateWindow(settings);
  
  auto out = make_shared<OutputView> (_name);
  ofRunApp(_window, out);
  out->window = _window;




  return out;


//  addMediaLayer(make_shared<DummyMedia>("dum"+name));
}

OutputView::~OutputView(){
  
  window.reset();

}
void OutputView::shouldClose() {
  window.reset();
  window.lock()->setWindowShouldClose();
  

};

void OutputView::draw(){


  glClearColor(0.,0.,0.,  255.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(auto & l:layers){
    l.draw();
  }
  
}


VideoLayer * OutputView::addMediaLayer(shared_ptr<InputMedia> media  ){
  layers.push_back(VideoLayer(media));
  VideoLayer * cL  =&layers[layers.size() - 1];
  cL->setInputRect(ofRectangle(0,0,media->getSize().x,media->getSize().y));
  cL->setOutputRect(ofRectangle(0,0,window.lock()->getWidth(),window.lock()->getHeight()));

  return cL;

};


void OutputView::updateOverlaps(){
  for(auto & l:layers){
    shared_ptr<InputMedia> curMed = l.media;
    l.overlapEdge.x = 0;
    l.overlapEdge.y = 0;

    for(auto & ll : layers){
      if(&ll!=&l && curMed==ll.media ){
        ofRectangle intersection = l.inputRect.getIntersection(ll.inputRect);
        if(intersection.getWidth()>0){
          if(ll.inputRect.getRight()>l.inputRect.getRight()){
            l.overlapEdge.y = MAX(l.overlapEdge.y,intersection.getWidth());
          }
          else{
            l.overlapEdge.x = MAX(l.overlapEdge.x,intersection.getWidth());

          }
        }

      }

    }
    cout << l.overlapEdge.x << "," << l.overlapEdge.y << ";;" << l.inputRect <<  endl;
  }


  for(auto & l : layers){
    l.updateEdgeMesh();
  }
  
}

ofVec2f OutputView::getWindowSize(){
  return window.lock()->getWindowSize();
}
void OutputView::windowResized(int w, int h){
  
  for(auto & l:layers){
//    l.destRect.setWidth(w);
//    l.destRect.setWidth(h);
  }
};


///////////////////
//

