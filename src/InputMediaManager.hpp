//
//  InputMediaManager.hpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#ifndef InputMediaManager_hpp
#define InputMediaManager_hpp

#include "ofMain.h"
#include "ofxSyphonServerDirectory.h"

#include"InputMedia.hpp"
#include "ofxUI.h"
#include "UIComponent.hpp"

class InputMediaManager : public UIComponent{
public:
  InputMediaManager();
  static InputMediaManager *i(){if(!instance){instance = new InputMediaManager();}return instance;}
  static InputMediaManager * instance;


  void update();
  void paint()override;
  void resized() override;

  shared_ptr<InputMedia> addMedia(unique_ptr<InputMedia> m);
  void removeMedia(InputMedia * m);
  void selectMedia(InputMedia::Sp m);

  //////
  // syphon
  ofxSyphonServerDirectory syphonDir;
  void newSyphonServer(ofxSyphonServerDirectoryEventArgs & a);
  void syphonServerRemoved(ofxSyphonServerDirectoryEventArgs & a);
  void syphonServerUpdated(ofxSyphonServerDirectoryEventArgs & a);




  ofEvent<shared_ptr<InputMedia> > mediaAdded;
  ofEvent<shared_ptr<InputMedia> > mediaRemoved;
  ofEvent<shared_ptr<InputMedia> > mediaResized;


  vector<shared_ptr<InputMedia> > allMedias;
  vector<shared_ptr<InputMedia> > allActiveMedias;



  shared_ptr<InputMedia> getInputMediaForName(const string & );
  shared_ptr<InputMedia> getSelectedMedia( );
private:
  shared_ptr<InputMedia> selected;

  ofxUISuperCanvas cnv;
  ofxUIDropDownList * ddlInputMedias;


  void newGUIEvent( ofxUIEventArgs & a);

};

#endif /* InputMediaManager_hpp */
