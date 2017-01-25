#include "ofMain.h"
#include "ofApp.h"


shared_ptr<ofApp> mainApp;


//========================================================================
int main( ){
  ofGLWindowSettings settings;
  settings.width = 600;
  settings.height = 600;
  settings.setPosition(ofVec2f(300,0));
  auto mainWindow = ofCreateWindow(settings);

  mainApp = make_shared<ofApp>();

  ofRunApp(mainWindow, mainApp);
  ofRunMainLoop();
}



