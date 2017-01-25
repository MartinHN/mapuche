//
//  UIComponent.hpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#ifndef UIComponent_hpp
#define UIComponent_hpp

#include "ofMain.h"

class UIComponent{
public:
  UIComponent(const string &_name);
  virtual ~UIComponent();

  shared_ptr<UIComponent> addNewChild(UIComponent *c);

  template<typename T,typename... Arguments>
  shared_ptr<T> addNewChild(Arguments ... args);

  void setBounds(const ofRectangle & b);

  bool interceptMouseClick;


  virtual void resized(){};
  virtual void moved(){};
  virtual void paint(){};


  virtual void mouseMoved(int x,int y){};
  virtual void mouseEntered(int x,int y){};
  virtual void mouseExited(int x,int y){};
  virtual void mousePressed(int x,int y){};
  virtual void mouseReleased(int x,int y){};




  void setParent(UIComponent*  p);

  virtual bool isHit(int x, int y){ return boundsInParent.inside(x,y);}


  const ofRectangle & getLocalBounds(){return localBounds;}
  const ofPoint getPosition(){return boundsInParent.getPosition();}
  ofRectangle getBoundsInRoot();
  ofRectangle getBoundsInParent(UIComponent * parentToCheck);
  shared_ptr<UIComponent> getChildComponentAt(const ofPoint & p,bool recurse);
  ofPoint toChildCoordinates(UIComponent * c,const ofPoint & p);


  string name;

  void draw();
  void mouseMovedInternal(int x,int y );
  void mousePressedInternal(int x,int y);
  void mouseReleasedInternal(int x,int y);

  UIComponent* parent;
  vector<shared_ptr < UIComponent > > childs;

  weak_ptr<UIComponent> lastHovered,lastClicked;

  void sendChildToBack(shared_ptr<UIComponent> c);
  int getChildIdx(shared_ptr<UIComponent> c);


  // listeners
  class Listener{
  public:
    virtual ~Listener(){};
    virtual void componentMoved(UIComponent *){};
    virtual void componentResized(UIComponent *){};

  };

  vector< Listener * > listeners;

  void addListener(Listener * l){ listeners.push_back(l);}
  void removeListener(Listener * l){auto it = find_if(listeners.begin(),listeners.end(),[l](Listener * ll){return l==ll;});
                                    if(it!=listeners.end())listeners.erase(it);}

  
  // helpers Rectangle

  static ofRectangle removeFromTop(ofRectangle & r,float amount){r.y+=amount ; r.height-=amount;return ofRectangle(r.x,r.y-amount,r.width,amount);}
  static ofRectangle removeFromLeft(ofRectangle & r,float amount){r.x+=amount ; r.width-=amount;return ofRectangle(r.x-amount,r.y,amount,r.height);}



private:
  ofRectangle boundsInParent,localBounds;

};


template<typename T,typename... Arguments>
shared_ptr<T> UIComponent::addNewChild(Arguments ... args){

  shared_ptr<T> sp = make_shared<T>(args...);
  childs.push_back(sp);
  sp->setParent(this);
  return sp;
};


//class UIDraggableGroup  {
//public:
//  UIDraggableGroup(shared_ptr<UIComponent> ){};
//  virtual ~UIDraggableGroup(){};
//
//  void addToGroup(UIComponent * c){
//    c->addListener(this);
//  };
//
//  void componentMousePress(UIComponent * c){
//    if (dragged!=c){
//      dragged = c;
//    }
//  }
//
//
//
//
//    UIComponent * dragged;
//
//
//
//};

#endif /* UIComponent_hpp */
