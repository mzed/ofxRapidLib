#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxRapidLib.h"
#include "ofxMaxim.h"

class ofApp : public ofBaseApp
{    
public:
    
    void setup() override;
    void exit() override;
    void update() override;
    void draw() override;
    
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y ) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    
    ofVideoGrabber 		vidGrabber;
    int 				camWidth;
    int 				camHeight;
    
    string asciiCharacters;
    ofTrueTypeFont  font;
    
    //-----RAPID-MIX-----//
    std::vector<rapidLib::trainingExample> myData;
    int recordingState;
    rapidLib::classification myKnn;
    bool runToggle;
    int result;
    
    //-----MAXI-----//
    std::size_t bufferSize;
    std::size_t sampleRate;
    void audioOut(ofSoundBuffer& output) override;
    ofSoundStream soundStream;
    double outputs[2];
    maxiMix mymix;
    
    maxiOsc timer;
    int currentCount;
    int lastCount;
    int playHead;
    
    maxiSample gam_1, gam_3, gam_4, gam_5, gam_6, gam_7;
};
