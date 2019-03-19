#pragma once

#include "ofMain.h"
#include "ofxRapidLib.h"

class ofApp : public ofBaseApp
{   
public:
    void setup() override;
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
    
private:
    //Drawing
    ofPolyline line;
    std::string instructions[3];
    char instructionCount;
    
    //RapidLib
    rapidlib::seriesClassification classifier;
    std::vector<rapidlib::trainingSeries> trainingSet;
    rapidlib::trainingSeries tempSeries;
    bool trained;
    std::string classLabel;
    
};
