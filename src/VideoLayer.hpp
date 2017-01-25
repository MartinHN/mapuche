//
//  VideoLayer.hpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#ifndef VideoLayer_hpp
#define VideoLayer_hpp

#include "InputMedia.hpp"
//class InputMedia;


class VideoLayer{
public:
  VideoLayer(shared_ptr<InputMedia> m );

  InputMedia::Sp media;

  ofRectangle normalizedSubSection;
  ofRectangle destRect;


  void draw();

  class InputAnchor:public ofPoint{
    public :
    InputAnchor(VideoLayer * o):owner(o){}
    VideoLayer * owner;

  };

  vector<InputAnchor> inputAnchors;


  class OutputAnchor:public ofPoint{
    public :
    OutputAnchor(VideoLayer * o):owner(o){}
    VideoLayer * owner;
  };

  vector<OutputAnchor> outputAnchors;

private:

  void applyNormalizedSubSection(ofPoint  size);



  ofRectangle subSection;

  
};
#endif /* VideoLayer_hpp */
