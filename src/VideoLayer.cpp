//
//  VideoLayer.cpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#include "VideoLayer.hpp"


// helper

inline void drawSubsection(const ofTexture & t, const ofRectangle & src,const ofRectangle & dest){
  t.drawSubsection(dest.x,dest.y,dest.width,dest.height,
                   src.x,src.y,src.width,src.height);
}



VideoLayer::VideoLayer(shared_ptr<InputMedia> m ):normalizedSubSection(0,0,1,1)
{
  media = m;

};

void VideoLayer::draw(){
  if(media.get()){
    applyNormalizedSubSection(media->getSize());
    if(media->hasTexture){
      if(media->hasValidTexture()){
        drawSubsection(media->getTexture(),subSection,destRect);
      }
      else{
        ofLogError("layer") << "trying to draw an old texture of media";
      }
    }
      media->drawSubsection(subSection,destRect);
    
  }
  else{
    ofLogError("layer") << "trying to draw an old media";
  }

};




void VideoLayer::applyNormalizedSubSection(ofPoint  size){
  subSection.x  = normalizedSubSection.x * size.x;
  subSection.y  = normalizedSubSection.y * size.y;
  subSection.width  = normalizedSubSection.width * size.x;
  subSection.height  = normalizedSubSection.height * size.y;
}



