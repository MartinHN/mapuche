//
//  UIComponent.cpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#include "UIComponent.hpp"


UIComponent::UIComponent(const string &_name):name(_name),interceptMouseClick(true),localBounds(0,0,0,0){

}
UIComponent::~UIComponent(){

}
void UIComponent::setParent(UIComponent*  p){
  parent = p;
}

shared_ptr<UIComponent> UIComponent::addNewChild(UIComponent *c){
  shared_ptr<UIComponent> sp = shared_ptr<UIComponent>(c);
  childs.push_back(sp);
  sp->setParent(this);
  return sp;
}





void UIComponent::setBounds(const ofRectangle & b){
  bool isResized = (boundsInParent.width != b.width )|| (boundsInParent.height!=b.height);
  bool isMoved = (boundsInParent.x != b.x )|| (boundsInParent.y!=b.y);
  localBounds.width = b.width;
  localBounds.height = b.height;
  boundsInParent = b;
  if(isResized || isMoved){
    resized();
    for(auto & l:listeners){l->componentResized(this);}
  }
  if(isMoved){
    moved();
    for(auto & l:listeners){l->componentMoved(this);}
  }

}

ofRectangle UIComponent::getBoundsInRoot(){

  ofRectangle res = boundsInParent;
  UIComponent * chk = parent;
  while (chk->parent){
    res.translate(chk->boundsInParent.getPosition());
    chk = chk->parent;
  }
  return res;

}
ofRectangle UIComponent::getBoundsInParent(UIComponent * parentToCheck){

  ofRectangle res = boundsInParent;
  UIComponent * chk = parent;
  while (chk->parent!=parentToCheck){
    if(chk->parent == nullptr){return ofRectangle(-1,-1,-1,-1);}
    res.translate(chk->boundsInParent.getPosition());
    chk = chk->parent;
  }
  return res;
  
}


void UIComponent::sendChildToBack(shared_ptr<UIComponent> c){
  int idx = getChildIdx(c);
  
  iter_swap(childs.begin() + idx,childs.begin() + childs.size()-1);
}


int UIComponent::getChildIdx(shared_ptr<UIComponent> c){
  auto it = std::find(childs.begin(),childs.end(),c);
  if(it!=childs.end()){
    return it - childs.begin();
  }
  return -1;
}

shared_ptr<UIComponent> UIComponent::getChildComponentAt(const ofPoint & p,bool recurse){
  bool found =false;
  for(auto & c:childs){
    if(c->isHit(p.x,p.y)){
      found  = true;
      if(!recurse)return c;
      else {
        ofPoint cP = toChildCoordinates(c.get(),cP);
        shared_ptr<UIComponent> f = c->getChildComponentAt(cP, true);
        if(f.get()==nullptr){
          return c;
        }
      }
    }
  }

  return  shared_ptr<UIComponent> (nullptr);
}


ofPoint UIComponent::toChildCoordinates(UIComponent * c,const ofPoint & p){
  return p+c->getPosition();

}

void UIComponent::mousePressedInternal(int x,int y){
  if( !interceptMouseClick)
    if(auto l = lastHovered.lock()){
    l->mousePressedInternal(x, y);
      lastClicked = lastHovered;
  }
}
void UIComponent::mouseReleasedInternal(int x,int y){
  if(auto l = lastClicked.lock()){
    l->mouseReleasedInternal(x,y);
  }
}
 void UIComponent::mouseMovedInternal(int x,int y ){
   shared_ptr<UIComponent> cH = nullptr;
   bool hasHovered = lastHovered.lock().get() != nullptr;
  for(auto & c:childs){

    if(c->isHit(x,y)){
      if(lastHovered.lock()!=c){
        c->mouseEntered(x,y);
        cH = c;

      }
      c->mouseMoved(x - c->boundsInParent.x,y-c->boundsInParent.y);
      c->mouseMovedInternal(x - c->boundsInParent.x,y-c->boundsInParent.y);

    }
    else if (hasHovered && lastHovered.lock()==c){
      c->mouseExited(x,y);
    }
  }

   lastHovered = cH;

}


void UIComponent::draw(){
  paint();
  for(auto & c:childs){
    ofTranslate(c->boundsInParent.getPosition());
    c->draw();
    ofTranslate(-c->boundsInParent.getPosition());
  }
};
