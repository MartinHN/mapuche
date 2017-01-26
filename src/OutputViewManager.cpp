//
//  OutputViewManager.cpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#include "OutputViewManager.hpp"
#include "VideoLayerManager.hpp"

OutputViewManager * OutputViewManager::instance(nullptr);

OutputViewManager::OutputViewManager():
UIComponent("outView"),

cnv ("OutView",200,200,200,200)
{


  addScreen = new ofxUIButton("add",false,10,10);
  removeScreen= new ofxUIButton("remove",false,10,10);
  std::vector<string> dummy;
  viewsDDL = new ofxUIDropDownList("views",dummy,cnv.getRect()->width -20,100);

  viewsDDL->open();
  //  views->setAllowMultiple(true);


  cnv.addWidgetDown(addScreen);
  cnv.addWidgetRight(removeScreen);
  cnv.addWidgetDown(viewsDDL);
  cnv.newGUIEvent.add(this, &OutputViewManager::newGUIEvent, 0);
  videoLayerManagerUI = addNewChild<VideoLayerManager>();
}

void OutputViewManager::update(){
  checkNewDisplaySetup();
}
void OutputViewManager::paint(){
  ofRectangle b = getLocalBounds();
  if(selected){
    ofPushStyle();
    ofPushMatrix();
    renderViewSelected.allocate(selected->window.lock()->getWidth(),selected->window.lock()->getHeight());
    renderViewSelected.begin();
    glClearColor(0.,0.,0.,  255.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    selected->draw();//0,0,100,100);

    renderViewSelected.end();
    renderViewSelected.draw(b);

    ofSetLineWidth(.1);
    ofPoint scale = ofPoint(b.width,b.height) / selected->window.lock()->getWindowSize();
    int lidx = 0;

    for(auto & l:selected->layers){
      int numV = l.drawMesh.getNumVertices();
      ofSetColor(lidx%3==0?255:0,lidx%3==1?255:0,lidx%3==2?255:0,255);
      for(int i = 0; i < numV ; i++){

        ofDrawLine(l.drawMesh.getVertex((i-2 +numV)%numV )*scale,l.drawMesh.getVertex(i)*scale);
      }
      lidx++;
    }
    ofPopMatrix();
    ofPopStyle();
  }

}

void OutputViewManager::resized(){
  ofRectangle sB = getBoundsInRoot();
  int pad = 3;
  cnv.getRect()->x = sB.x ;
  cnv.getRect()->y = sB.y + pad;
  cnv.getRect()->height = sB.height - 2*pad;



  if(videoLayerManagerUI){
    ofRectangle local = getLocalBounds();
    removeFromLeft(local,cnv.getRect()->width);
    videoLayerManagerUI->setBounds(removeFromLeft(local,200));
  }
}


void OutputViewManager::addView(const string & name,int x,int y, int w,int h){
  shared_ptr<OutputView> v = OutputView::generate(name, x,  y,w,h);
  views.push_back(v);
//  v->window->setWindowShape(w,h);
//  v->viewResized.add(this,&OutputViewManager::viewResized,0);

}


//void OutputViewManager::viewResized(const OutputView * v,ofPoint & p){
//
//}

shared_ptr<OutputView>  OutputViewManager::getViewForName(const string & name){
  for(auto & v:views){
    if(v->name == name){return v;}
  }
  return nullptr;
}
void OutputViewManager::selectView(string name){
  selected = getViewForName(name);
  auto sel = selected;
  videoLayerManagerUI->buildFromOutView(  selected);
  viewSelectedEvent.notify(sel);
}
void OutputViewManager::monitorResized(int num, ofPoint size){

}

void OutputViewManager::monitorMoved(int num, ofPoint pos){

}

void OutputViewManager::monitorAdded(int num){

}
void OutputViewManager::monitorRemoved(int num){

}



void OutputViewManager::removeView(const string & name){
  auto vIt = std::find_if(views.begin(),views.end(),[&name](shared_ptr<OutputView> v){return v->name == name;});
  if(vIt!=views.end()){
    (*vIt)->shouldClose();
    views.erase(vIt);
  }
}



void OutputViewManager::checkNewDisplaySetup(){
  int count = 0;
  GLFWmonitor** monitors = glfwGetMonitors(&count);
  if(count > lastMonitorCount ){
    for(int i = lastMonitorCount ; i < count ; i++){
      monitorAdded(i-1);
    }
  }
  else if(count < lastMonitorCount){
    vector<string> allNames(count);
    for(int i = 0 ; i < count ; i++){allNames[i] = glfwGetMonitorName(monitors[i]);};
    for(auto n:allNames){
      auto mIt = std::find_if(lastMonitors.begin(),lastMonitors.end(),[&n](MonitorInfo & m){return m.name==n;});
      if(mIt!=lastMonitors.end()){
        monitorRemoved(&n - &allNames[0]);
      }
    }
  }




  for(int i = 0 ; i < count ; i++){
    int xM; int yM;
    glfwGetMonitorPos(monitors[i], &xM, &yM); // We take the second monitor
    const GLFWvidmode * desktopMode = glfwGetVideoMode(monitors[i]);
    const string name = glfwGetMonitorName(monitors[i]);
    ofRectangle curRect = ofRectangle(xM,yM,desktopMode->width,desktopMode->height);
    ofPoint curSize = ofPoint(curRect.width,curRect.height);
    auto mIt = std::find_if(lastMonitors.begin(),lastMonitors.end(),[&name](MonitorInfo & m){return m.name==name;});
    if(mIt!=lastMonitors.end()){
      if(mIt->rect.getPosition()!=curRect.getPosition()){
        monitorMoved(i, curRect.getPosition());
      }
      if(ofPoint(mIt->rect.width,mIt->rect.height)!=curSize){
        monitorResized(i, curSize);
      }
    }
    else{
      monitorMoved(i, curRect.getPosition());
      monitorResized(i, curSize);
    }
  }
  
  
  lastMonitorCount = count;
}


vector <string> OutputViewManager::getSelectedViews(){
  return viewsDDL->getSelectedNames();

}
int screenNum = 0;
void OutputViewManager::newGUIEvent(  ofxUIEventArgs & a){
  if(a.widget==addScreen && !addScreen->getValue()){
    string name = ofToString(screenNum);
    addView(name,200,200,300,300);
    viewsDDL->addToggle(name);
    screenNum++;
  }
  else if(a.widget==removeScreen && !removeScreen->getValue()){
    for(auto & n:getSelectedViews()){
      removeView(n);
      viewsDDL->removeToggle(n);
      screenNum--;
    }

  }
  else if(a.widget->getParent() == viewsDDL){
    if( ofxUIToggle * t = a.getToggle()){
      if(t->getValue())
        selectView(a.widget->getName());
    }
  }
}
