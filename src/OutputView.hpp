//
//  OutputView.hpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#ifndef OutputView_hpp
#define OutputView_hpp

#include "ofMain.h"

#include "VideoLayer.hpp"


class OutputView : public ofBaseApp{
public:
  OutputView(const string & _name);
 virtual  ~OutputView();

  static shared_ptr<OutputView>generate(const string & name,int x, int y,int w,int h);


  void setup()override{};
		void update()override{};
		void draw()override;

		void keyPressed(int key)override{};
		void keyReleased(int key)override{};
		void mouseMoved(int x, int y )override{};
		void mouseDragged(int x, int y, int button)override{};
		void mousePressed(int x, int y, int button)override{};
		void mouseReleased(int x, int y, int button)override{};
		void mouseEntered(int x, int y)override{};
		void mouseExited(int x, int y)override{};
		void windowResized(int w, int h)override;
		void dragEvent(ofDragInfo dragInfo)override{};
		void gotMessage(ofMessage msg)override{};


  void shouldClose() ;

  VideoLayer * addMediaLayer(shared_ptr<InputMedia> media  );
  

  vector<VideoLayer> layers;

  weak_ptr<ofAppBaseWindow> window;
//  ofEvent<ofPoint> viewResized;

  void updateOverlaps();
  ofVec2f getWindowSize();
  string name;

};

#endif /* OutputView_hpp */
