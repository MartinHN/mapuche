//
//  InputSyphon.hpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#ifndef InputSyphon_hpp
#define InputSyphon_hpp

#include "InputMedia.hpp"
#include "ofxSyphonClient.h"


class InputSyphon :public InputMedia{

public:
  InputSyphon(const ofxSyphonServerDescription & desc):InputMedia(desc.appName+"_"+desc.serverName){

    client.set(desc);
    client.setup();
    client.bind();
    client.unbind();
  };

  ofTexture & getTexture()override{
    client.bind();
    client.unbind();
    return client.getTexture();
  };
  void drawSubsection(const ofRectangle & normalizedSubSection,const ofRectangle & dest) override{

  }



  bool hasValidTexture() override{return client.isSetup();}
  void allocate()override{};
  void release()override{};
  
  ofxSyphonClient client;
};

#endif /* InputSyphon_hpp */
