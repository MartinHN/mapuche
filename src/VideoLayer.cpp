//
//  VideoLayer.cpp
//  mapuche
//
//  Created by Martin Hermant on 20/01/2017.
//
//

#include "VideoLayer.hpp"
#include "EdgeBlendShader.h"

#define NUM_BLENDED_LAYERS 6


ofShader * VideoLayer::blendShader(nullptr);
ofImage * VideoLayer::blendImage(nullptr);
// helper

inline void drawSubsection(const ofTexture & t, const ofRectangle & src,const ofRectangle & dest){
  t.drawSubsection(dest.x,dest.y,dest.width,dest.height,
                   src.x,src.y,src.width,src.height);
}



VideoLayer::VideoLayer(shared_ptr<InputMedia> m ):normalizedSubSection(0,0,1,1)
{
  media = m;
  useBlendShader= true;
  if(!useBlendShader && blendImage == nullptr){
    blendImage = new ofImage("edge.png");
  }
  if(useBlendShader  &&!blendShader){
    blendShader = new ofShader();
    blendShader->unload();
    blendShader->setupShaderFromSource(GL_FRAGMENT_SHADER, ofxProjectorBlendFragShader2(NUM_BLENDED_LAYERS));
    blendShader->setupShaderFromSource(GL_VERTEX_SHADER, ofxProjectorBlendVertShader);
    blendShader->linkProgram();

    overlapEdge = ofVec4f(0,0,0,0);
    blendPower  = 2;
    blendLuminance = 0.5;
    blendGamma = 1;

  }


};

void VideoLayer::updateShaderUniforms(){
  if(!useBlendShader  )return;
  blendShader->setUniform1f("BlendPower", blendPower);
  blendShader->setUniform1f("SomeLuminanceControl", blendLuminance);
  blendShader->setUniform1f("GammaCorrection",blendGamma);

  blendShader->setUniform1f("width", inputRect.getWidth());
  blendShader->setUniform1f("height", inputRect.getHeight());

  blendShader->setUniform1f("OverlapLeft", overlapEdge.x);
  blendShader->setUniform1f("OverlapRight", overlapEdge.y);
  blendShader->setUniform1f("OverlapTop", overlapEdge.z);
  blendShader->setUniform1f("OverlapBottom", overlapEdge.w);

//  blendShader->setUniform1f("BlackOutLeft", 0);
//  blendShader->setUniform1f("BlackOutRight", 0);
//  blendShader->setUniform1f("BlackOutTop", 0);
//  blendShader->setUniform1f("BlackOutBottom", 0);
//
//  blendShader->setUniform1f("SolidEdgeEnable", 1);
//  blendShader->setUniform4f("SolidEdgeColor", 255,0,0,255);
  blendShader->setUniform2f("texCoordOffset", inputRect.x, inputRect.y);
  if(media->hasTexture){
    blendShader->setUniformTexture("Tex0", media->getTexture(), 0);
  }



}

void VideoLayer::draw(){
  if(media.get()){
    ofPushStyle();
    ofPushMatrix();
    ofPushView();
    applyNormalizedSubSection(media->getSize());
    if(media->hasTexture){
      if(media->hasValidTexture()){

        ofEnableAlphaBlending();
        if(useBlendShader)
        {blendShader->begin();
                  updateShaderUniforms();}
        ofSetColor(255,255,255,255);
        media->getTexture().bind();
        drawMesh.draw();
        media->getTexture().unbind();
        if(useBlendShader)blendShader->end();
        if(!useBlendShader){

          glEnable(GL_BLEND);
//          glBlendColor(1,1,1,0);
          glBlendEquation(GL_FUNC_ADD);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          blendImage->bind();
          edgeMeshR.draw();
          edgeMeshL.draw();
          blendImage->unbind();
        }
      }
      //        drawSubsection(media->getTexture(),subSection,destRect);
    }
    else{
      ofLogError("layer") << "trying to draw an old texture of media";
    }

    //      media->drawSubsection(subSection,destRect);
    ofPopStyle();
    ofPopMatrix();
    ofPopView();
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

void VideoLayer::setOutputPoints(vector<ofPoint> p){
  outputPoints = p;
  updateMesh();
}
void VideoLayer::setInputRect(ofRectangle i){
  inputRect = i;
  if(outputPoints.size()){
    updateMesh();
  }
}

void VideoLayer::setOutputRect(ofRectangle r){
  vector<ofPoint> tmp=
  { r.getTopLeft(),
    r.getTopRight(),
    r.getBottomRight(),
    r.getBottomLeft()
  };
  setOutputPoints(tmp);

}
void  VideoLayer::updateMesh(){
  drawMesh = ofMesh::plane(1,1,2,2,OF_PRIMITIVE_TRIANGLES);
  int v0 = 0;
  int v1 = 1;
  int v2 = 2;
  int v3 =3;

  drawMesh.setVertex(0, outputPoints[v3]);
  drawMesh.setVertex(1, outputPoints[v2]);
  drawMesh.setVertex(2, outputPoints[v0]);
  drawMesh.setVertex(3, outputPoints[v1]);



  drawMesh.setTexCoord(0,ofVec2f(inputRect.getTopLeft()));
  drawMesh.setTexCoord(1,ofVec2f(inputRect.getTopRight()));
  drawMesh.setTexCoord(2,ofVec2f(inputRect.getBottomLeft()));
  drawMesh.setTexCoord(3,ofVec2f(inputRect.getBottomRight()));

  if(!useBlendShader){
    updateEdgeMesh();
  }


}
void  VideoLayer::updateEdgeMesh(){
  if(useBlendShader) return;
  edgeMeshL = ofMesh::plane(1,1,2,2,OF_PRIMITIVE_TRIANGLES);
  edgeMeshR = ofMesh::plane(1,1,2,2,OF_PRIMITIVE_TRIANGLES);
  int v0 = 3;
  int v1 = 2;
  int v2 = 0;
  int v3 =1;
  ofVec2f vDirTop = (outputPoints[v1] - outputPoints[v0]) ;
  ofVec2f vDirBottom = (outputPoints[v3] - outputPoints[v2]) ;
  ofVec2f imgSize (blendImage->getWidth(),blendImage->getHeight());

  edgeMeshL.setVertex(0, outputPoints[v0]);
  edgeMeshL.setVertex(1, outputPoints[v0] + vDirTop * overlapEdge.x*1.0/inputRect.getWidth());
  edgeMeshL.setVertex(2, outputPoints[v2]);
  edgeMeshL.setVertex(3, outputPoints[v2]+ vDirBottom * overlapEdge.x*1.0/inputRect.getWidth());

  edgeMeshL.setTexCoord(0,ofVec2f(0,0)*imgSize);
  edgeMeshL.setTexCoord(1,ofVec2f(1,0)*imgSize);
  edgeMeshL.setTexCoord(2,ofVec2f(0,1)*imgSize);
  edgeMeshL.setTexCoord(3,ofVec2f(1,1)*imgSize);


  edgeMeshR.setVertex(0, outputPoints[v1] -  vDirTop * overlapEdge.y*1.0/inputRect.getWidth());
  edgeMeshR.setVertex(1, outputPoints[v1] );
  edgeMeshR.setVertex(2, outputPoints[v3]- vDirBottom * overlapEdge.y*1.0/inputRect.getWidth());
  edgeMeshR.setVertex(3, outputPoints[v3]);

  edgeMeshR.setTexCoord(1,ofVec2f(0,0)*imgSize);
  edgeMeshR.setTexCoord(0,ofVec2f(1,0)*imgSize);
  edgeMeshR.setTexCoord(3,ofVec2f(0,1)*imgSize);
  edgeMeshR.setTexCoord(2,ofVec2f(1,1)*imgSize);
  
  
  
}



