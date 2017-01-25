//
//  OutputViewManager.hpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#ifndef OutputViewManager_hpp
#define OutputViewManager_hpp

#include "OutputView.hpp"
#include "ofxUI.h"
#include "UIComponent.hpp"

class VideoLayerManager;


class OutputViewManager : public UIComponent{
private:
  OutputViewManager();
  public:

  typedef shared_ptr<OutputView> Sp;
  static OutputViewManager * i(){if(!instance){instance = new OutputViewManager();}return instance;}
  static OutputViewManager * instance;

  void setup();
  void update();
  void paint()override;
  void resized()override;

  void addView(const string & name);
  void removeView(const string & name);



  vector<shared_ptr<OutputView> > views;



  void monitorAdded(int num);
  void monitorRemoved(int num);

  void selectView(string name);

  ofEvent<Sp> viewSelectedEvent;

  shared_ptr<OutputView> getViewForName(const string & name);



//  void viewResized(const OutputView * v,ofPoint & p);
  shared_ptr<OutputView> selected;

  shared_ptr<VideoLayerManager> videoLayerManagerUI;



  //ui

  void newGUIEvent( ofxUIEventArgs & a);
  vector<string>  getSelectedViews();

  ofxUISuperCanvas cnv;

  ofxUIButton *addScreen,*removeScreen;
  ofxUIDropDownList * viewsDDL;


  



private:
  ofFbo renderViewSelected;
  //  void viewSelected(OutputView * & v );

  void checkNewDisplaySetup();
  int lastMonitorCount;

  void monitorResized(int num, ofPoint size);
  void monitorMoved(int num, ofPoint pos);

  struct MonitorInfo{
    ofRectangle rect;
    string name;
  };
  vector<MonitorInfo> lastMonitors;





};


#endif /* OutputViewManager_hpp */
