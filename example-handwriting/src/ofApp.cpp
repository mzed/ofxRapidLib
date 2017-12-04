#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    instructionCount = 0;
    instructions[0] = "Draw a figure 8 with the mouse.";
    instructions[1] = "Now draw a number 2.";
    instructions[2] = "Draw shapes for continuous classification.";
    
    tempSeries.label = "8";
    trained = false;
    classLabel = "";
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString(instructions[instructionCount], 20, 20);
    ofDrawBitmapString(classLabel, 20, 750);
    if (instructionCount == 2) {
        ofDrawBitmapString("Classifying on the last " + std::to_string(classifier.getMinLength()) + " mouse positions.", 600, 750);
    }
  
    line.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofPoint pt;
    pt.set(x,y);
    line.addVertex(pt);
    
    //Training data for machine learning
    if (instructionCount < 2) {
        tempSeries.input.push_back({double(x), double(y)});
    } else {
        classLabel = "class found: " + classifier.runContinuous({double(x), double(y)});
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    line.clear();
    if (instructionCount == 1) {
        //Put the 8 example in the training set
        trainingSet.push_back(tempSeries);
        //Get ready for 2 example
        tempSeries.input.clear();
        tempSeries.label = "2";
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (instructionCount == 1) {
        //Push the "Z" example into the set and train
        trainingSet.push_back(tempSeries);
        trained = classifier.train(trainingSet);
    }
    if (instructionCount < 2) {
        ++instructionCount;
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
