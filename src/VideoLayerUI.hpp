//
//  VideoLayerUI.hpp
//  mapuche
//
//  Created by Martin Hermant on 22/01/2017.
//
//

#ifndef VideoLayerUI_hpp
#define VideoLayerUI_hpp

#include "VideoLayer.hpp"

#include "PolylineUI.hpp"


class VideoLayerUI:public PolylineUI{
public:
  VideoLayerUI(shared_ptr<VideoLayer> o);
  ~VideoLayerUI();


  
  void resized()override;
  weak_ptr<VideoLayer> owner;

};


#endif /* VideoLayerUI_hpp */
