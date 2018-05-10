#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxRapidLib.h"
#include "ofxMaxim.h"

#include "sensel.h"
#include "sensel_device.h"

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
    
    //ofTrueTypeFont  font;
    
    //-----SENSEL-----//
    SENSEL_HANDLE senselHandle = NULL;
    SenselDeviceList senselList;
    SenselSensorInfo senselInfo;
    SenselFrameData *senselFrame = NULL;
    bool senselFound = false;
    
    //-----RAPID-LIB-----//
    std::vector<trainingExample> trainingSet;
    int recordingState;
    classification myKnn;
    bool runToggle;
    int result;
    
    //-----MAXI-----//
    int bufferSize;
    int	initialBufferSize;
    int sampleRate;
    void audioOut(float * output, int bufferSize, int nChannels);
    double outputs[2];
    maxiMix mymix;
    maxiSample loop_1, loop_2, loop_3;
};
