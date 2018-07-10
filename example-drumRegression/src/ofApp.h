#pragma once

#include <vector>

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMyo.h"
#include "ofxMaxim.h"
#include "ofxRapidLib.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
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
    
    void probsClearPressed();
    void resetModelPressed();
    
    ofxIntSlider thung0, thung1, thung2, thung3;
    ofxIntSlider thung4, thung5, thung6, thung7;
    ofxIntSlider thung8, thung9, thung10, thung11;
    ofxPanel guiThung;

    ofxIntSlider tong0, tong1, tong2, tong3;
    ofxIntSlider tong4, tong5, tong6, tong7;
    ofxIntSlider tong8, tong9, tong10, tong11;
    ofxPanel guiTong;
    
    ofxFloatSlider gain;
    ofxToggle inputDevice;
    ofxToggle modelControl;
    ofxButton resetModel;
    ofxButton probsClear;
    ofxPanel guiGeneral;
    
    std::vector<ofxIntSlider *> allSliders;
    
    //Maxi
    void audioOut(float * output, int bufferSize, int nChannels);
    //void audioIn(float * input, int bufferSize, int nChannels);
    
    int	bufferSize;
    int	initialBufferSize;
    int sampleRate;
    
private:
    
    ofxMyo::Myo myo;
    
    //---------Maxi---------------//
    double outputs[2];
    
    maxiSample saron_sbpl1;
    maxiSample saron_sbpl2;
    maxiSample saron_sbpl3;
    maxiSample saron_sbpl4;
    maxiSample saron_sbpl5;
    
    maxiSample ciblon_tong;
    maxiSample ciblon_thung;
    
    maxiClock myClock;
   
    int currentBeat;
    
    //---------RapidLib---------------//
    rapidlib::regression myNN;
    std::vector<rapidlib::trainingExample> trainingSet;
    int recordingState;
    bool trained;
    
    rapidlib::rapidStream<double> streamBuf{25};
    
};
