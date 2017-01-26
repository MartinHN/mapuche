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

   static ofShader * blendShader;
  static ofImage * blendImage;
  bool useBlendShader;

  ofVec4f overlapEdge;
  float blendPower;
   float blendLuminance;
  float blendGamma;
  

  void setInputRect(ofRectangle i);
  void setOutputPoints(vector<ofPoint> p);
  void setOutputRect(ofRectangle r);
  void updateMesh();
  void updateEdgeMesh();
  ofMesh edgeMeshL,edgeMeshR;

ofMesh drawMesh;
  ofRectangle inputRect;
private:

  void applyNormalizedSubSection(ofPoint  size);
void updateShaderUniforms();


  vector<ofPoint> outputPoints;
  ofRectangle subSection;

  
};
#endif /* VideoLayer_hpp */
