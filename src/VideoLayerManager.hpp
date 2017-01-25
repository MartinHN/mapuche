//
//  VideoLayerManager.hpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#ifndef VideoLayerManager_hpp
#define VideoLayerManager_hpp

#include "VideoLayerUI.hpp"
#include "OutputView.hpp"
#include "ofxUI.h"

class VideoLayerManager : public UIComponent{
public:
  VideoLayerManager();


  void buildFromOutView(shared_ptr<OutputView> outView);

  void resized()override;

  unique_ptr<ofxUISuperCanvas> cnv;
  ofxUIButton * addLayerB,*removeLayerB;
  vector <VideoLayer *> selectedLayers;
  ofxUIDropDownList *layerDDL;

  shared_ptr<OutputView> outView;


  vector<shared_ptr<VideoLayerUI> > layerUIs;

  void newGUIEvent(  ofxUIEventArgs & a);
  vector <string > getSelectedLayers();
  void addLayer(const string &name);
  void removeLayer(const string & name);
  void selectLayer(const string & name);
  
};

#endif /* VideoLayerManager_hpp */
