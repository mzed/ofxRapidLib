#pragma once

#include "ofMain.h"
#include "rapidmix.h"
#include "ofxMaxim.h"

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void exit();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofVideoGrabber 		vidGrabber;
    int 				camWidth;
    int 				camHeight;
    
    string asciiCharacters;
    ofTrueTypeFont  font;
    
    //-----RAPID-MIX-----//
    rapidmix::trainingData myData;
    int recordingState;
    rapidmix::staticClassification myKnn;
    bool runToggle;
    int result;
    
    
    xmmToolConfig xcfg;
    rapidmix::xmmStaticClassification myGmm;
    
    //-----MAXI-----//

    int bufferSize;
    int	initialBufferSize; //MZ: What is this for?
    int sampleRate;
    void audioOut(float * output, int bufferSize, int nChannels);
    double outputs[2];
    maxiMix mymix;
    
    maxiOsc timer;
    int currentCount;
    int lastCount;
    int playHead;
    
    maxiSample gam_1, gam_3, gam_4, gam_5, gam_6, gam_7;
   
    
};
