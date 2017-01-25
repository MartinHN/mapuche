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
  OutputView(const string & name);
  ~OutputView();


  void setup(){};
		void update(){};
		void draw();

		void keyPressed(int key){};
		void keyReleased(int key){};
		void mouseMoved(int x, int y ){};
		void mouseDragged(int x, int y, int button){};
		void mousePressed(int x, int y, int button){};
		void mouseReleased(int x, int y, int button){};
		void mouseEntered(int x, int y){};
		void mouseExited(int x, int y){};
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo){};
		void gotMessage(ofMessage msg){};


  void shouldClose() {window->setWindowShouldClose();};

  void addMediaLayer(shared_ptr<InputMedia> media  );
  

  vector<VideoLayer> layers;
  shared_ptr<ofAppBaseWindow> window;
//  ofEvent<ofPoint> viewResized;


  string name;

};

#endif /* OutputView_hpp */
