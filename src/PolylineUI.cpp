//
//  PolylineUI.cpp
//  mapuche
//
//  Created by Martin Hermant on 23/01/2017.
//
//

#include "PolylineUI.hpp"


void PolylineUI::paint(){
  ofRectangle area =  getLocalBounds();
  ofFill();
  ofSetColor(ofColor::aqua);
  ofDrawRectangle(area);
}
