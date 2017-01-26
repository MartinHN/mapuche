//
//  InputMedia.hpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#ifndef InputMedia_hpp
#define InputMedia_hpp

#include "ofMain.h"

class InputMedia{
  public :
  InputMedia(const string & name);
  virtual ~InputMedia();

  bool hasTexture;
  string  name;



  virtual ofPoint getSize();
  virtual void allocate() {};
  virtual void release() {};



  virtual void update(){};
  virtual ofTexture &  getTexture() =0;


  // custom , non textured draw on top of texture
  virtual void drawSubsection(const ofRectangle & subSection,const ofRectangle & dest) {};



  void notifyResized();



  virtual bool hasValidTexture(){return true;};

  ofPoint fallBackSize;

  typedef shared_ptr<InputMedia> Sp;


};


class DummyMedia : public InputMedia{
public:
  DummyMedia(const string & name):InputMedia(name){
    dirtyFBO = true;


  }


  void drawFBO() {
    ofRectangle  dest(0,0,200,200);
    fbo.allocate(dest.width,dest.height);
    fbo.begin();
    ofFill();
    int w = dest.width/2;
    int h = dest.height/2;
    int x = dest.x;
    int y = dest.y;
    ofSetColor(ofColor(255,0,0));
    ofDrawRectangle(x,y,w , h);
    ofSetColor(ofColor(0,255,0));
    ofDrawRectangle(x+w,y,w , h);
    ofSetColor(ofColor(0,0,255));
    ofDrawRectangle(x+w,y+h,w , h);
    ofSetColor(ofColor(255,255,0));
    ofDrawRectangle(x,y+h,w , h);

    ofSetColor(ofColor(0));
    ofDrawBitmapString(name, dest.getCenter());
    fbo.end();
  };

  ofTexture & getTexture() override{
    if(dirtyFBO){drawFBO();dirtyFBO = false;}
    return fbo.getTexture();
  }

  ofFbo fbo;
  bool dirtyFBO;
  
  
  
  
  
};




#endif /* InputMedia_hpp */
