//
//  InputMedia.cpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#include "InputMedia.hpp"


InputMedia::InputMedia(const string & _name):
name(_name),
fallBackSize(100,100)
{
  hasTexture = true;

}
InputMedia::~InputMedia(){

}

ofPoint InputMedia::getSize(){
  if(hasTexture){return ofPoint(getTexture().getWidth(),getTexture().getHeight());}
  else{
//    ofLogError("inputMedia") << name << " : has no texture, cant find size";
    return fallBackSize;
  }
}
