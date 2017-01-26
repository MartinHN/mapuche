#pragma once

#include "ofMain.h"

#include "UIComponent.hpp"
#include "OutputViewManager.hpp"
#include "InputMediaManager.hpp"
#include "ofxSyphon.h"
#include "InputSyphon.hpp"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
  void openArenaLayout(const string & path);
  class MainLayout:public UIComponent{
  public:
    MainLayout():UIComponent("MainLayout"){
      

      iMgr = InputMediaManager::i();
      addNewChild(iMgr);

      vMgr =OutputViewManager::i();
      addNewChild(vMgr);

      // dummy
      ofxSyphonServerDescription desc("ImMedia","ImMedia");
      auto sypMedia = iMgr->addMedia(make_unique<InputSyphon>(desc));
      iMgr->selectMedia (sypMedia);
      
//      vMgr->addView("lala");

    }

    void resized(){
      ofRectangle area = getLocalBounds();
      int h = area.height/2;
      iMgr->setBounds(removeFromTop(area,h));
      vMgr->setBounds(removeFromTop(area,h));


//      ofRectangle header = removeFromTop(area,h);
//      int h = area.height/2;
//      vMgr->setBounds(header.removeFromRight(header,header.width/2));
//      iMgr->setBounds(header);


    };

    OutputViewManager * vMgr;
    InputMediaManager * iMgr;

  };
  MainLayout mainComponent;
  string toLoad;

//		OutputViewManager outViewManager;
//  InputMediaManager inputMediaManager;
};



