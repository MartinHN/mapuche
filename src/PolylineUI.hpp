//
//  PolylineUI.hpp
//  mapuche
//
//  Created by Martin Hermant on 23/01/2017.
//
//

#ifndef PolylineUI_hpp
#define PolylineUI_hpp

#include "UIComponent.hpp"




class PolylineUI:public UIComponent,UIComponent::Listener{

  class AnchorUI : public UIComponent{
    public :
    AnchorUI(PolylineUI * o,ofPoint & p):UIComponent("AnchorUI"),owner(o),pos(p){};

    void paint()override{
      ofRectangle area = getLocalBounds();
      ofNoFill();

      ofDrawRectangle(area);
    }

    ofPoint pos;
    PolylineUI  * owner;
  };

  typedef  int AnchorID;

  
public:
  PolylineUI(const string & name):UIComponent(name){};
  ~PolylineUI(){};

  AnchorID addAnchor(ofPoint & position){
    auto sp = make_shared<AnchorUI>(this,position);
    sp->listeners.push_back(this);
    anchors.push_back(sp);
    return anchors.size()-1;

  }

  void componentMoved(UIComponent * c)override{
    auto it = find_if(anchors.begin(),anchors.end(),[c](shared_ptr<AnchorUI> & a){return c==a.get();});
    if(it!=anchors.end()){
      for(auto & l:listeners){l->anchorMoved(it-anchors.begin());}
    }
  }

  vector < shared_ptr<AnchorUI> > anchors;


  class Listener{
  public:
    virtual ~Listener(){};
    virtual void anchorAdded(AnchorID id);
    virtual void anchorMoved(AnchorID id);
  };

  vector<Listener *> listeners;

private:
  void paint()override;
  

};

#endif /* PolylineUI_hpp */
