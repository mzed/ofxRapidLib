#include <array>

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    myo.setup();
    
    //gui
    probsClear.addListener(this, &ofApp::probsClearPressed);
    resetModel.addListener(this, &ofApp::resetModelPressed);
    
    guiTong.setup("tong");
    guiTong.add(tong0.setup("1", 50, 0, 100));
    guiTong.add(tong1.setup("2", 0, 0, 100));
    guiTong.add(tong2.setup("3", 90, 0, 100));
    guiTong.add(tong3.setup("4", 0, 0, 100));
    guiTong.add(tong4.setup("5", 90, 0, 100));
    guiTong.add(tong5.setup("6", 0, 0, 100));
    guiTong.add(tong6.setup("7", 0, 0, 100));
    guiTong.add(tong7.setup("8", 60, 0, 100));
    guiTong.add(tong8.setup("9", 0, 0, 100));
    guiTong.add(tong9.setup("10", 90, 0, 100));
    guiTong.add(tong10.setup("11", 0, 0, 100));
    guiTong.add(tong11.setup("12", 0, 0, 100));
    ofColor green(0, 255, 0);
    guiTong.setHeaderBackgroundColor(green);
    
    guiThung.setup("thung", "thung", 250, 10);
    guiThung.add(thung0.setup("1", 100, 0, 100));
    guiThung.add(thung1.setup("2", 0, 0, 100));
    guiThung.add(thung2.setup("3", 10, 0, 100));
    guiThung.add(thung3.setup("4", 0, 0, 100));
    guiThung.add(thung4.setup("5", 60, 0, 100));
    guiThung.add(thung5.setup("6", 0, 0, 100));
    guiThung.add(thung6.setup("7", 10, 0, 100));
    guiThung.add(thung7.setup("8", 0, 0, 100));
    guiThung.add(thung8.setup("9", 60, 0, 100));
    guiThung.add(thung9.setup("10", 0, 0, 100));
    guiThung.add(thung10.setup("11", 30, 0, 100));
    guiThung.add(thung11.setup("12", 0, 0, 100));
    ofColor yellow(255, 255, 0);
    guiThung.setHeaderBackgroundColor(yellow);
    
    guiGeneral.setup("general", "general", 500, 10);
    guiGeneral.add(gain.setup("gain", 1., 0., 1.));
    guiGeneral.add(modelControl.setup("run model", false));
    guiGeneral.add(resetModel.setup("reset model"));
    guiGeneral.add(inputDevice.setup("myo", false));
    guiGeneral.add(probsClear.setup("clear"));
    
    //This will make life easier later
    allSliders.push_back(tong0);
    allSliders.push_back(tong1);
    allSliders.push_back(tong2);
    allSliders.push_back(tong3);
    allSliders.push_back(tong4);
    allSliders.push_back(tong5);
    allSliders.push_back(tong6);
    allSliders.push_back(tong7);
    allSliders.push_back(tong8);
    allSliders.push_back(tong9);
    allSliders.push_back(tong10);
    allSliders.push_back(tong11);
    allSliders.push_back(thung0);
    allSliders.push_back(thung1);
    allSliders.push_back(thung2);
    allSliders.push_back(thung3);
    allSliders.push_back(thung4);
    allSliders.push_back(thung5);
    allSliders.push_back(thung6);
    allSliders.push_back(thung7);
    allSliders.push_back(thung8);
    allSliders.push_back(thung9);
    allSliders.push_back(thung10);
    allSliders.push_back(thung11);
    
    //RapidLib
    trained = false;
    
    //loading samples    
    ciblon_tong.load(ofToDataPath("drums-ciblon-medium-tong.wav"));
    ciblon_thung.load(ofToDataPath("drums-ciblon-medium-thung.wav"));
    
    //maxi Clock
    myClock.setTicksPerBeat(4);//This sets the number of ticks per beat
    myClock.setTempo(120);// This sets the tempo in Beats Per Minute
    
    //audio setup
    sampleRate 	= 44100;
    bufferSize	= 512;
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){

    if (inputDevice) { //Only when Myo is toggled on
        
        //Simple gain control
        float emg = myo.getDevices()[0]->getEmgSamples()[4];
        streamBuf.pushToWindow(double(emg));
        gain = streamBuf.rms() * 0.01;
        
        //Machine learning with quarternions
        double myoX = myo.getDevices()[0]->getQuaternion().x();
        double myoY = myo.getDevices()[0]->getQuaternion().y();
        double myoZ = myo.getDevices()[0]->getQuaternion().z();
        double myoW = myo.getDevices()[0]->getQuaternion().w();
        
        if (inputDevice) {
            if (recordingState > 0) {
                trainingExample<double> tempExample;
                tempExample.input = { myoX, myoY, myoZ, myoW };
                for (int i = 0; i < allSliders.size() ; ++i) {
                    tempExample.output.push_back(double(allSliders[i]));
                }
                trainingSet.push_back(tempExample);
            } else if (trained && modelControl == 1) {
                std::vector<double> inputVec;
                inputVec.push_back(myoX);
                inputVec.push_back(myoY);
                inputVec.push_back(myoZ);
                inputVec.push_back(myoW);
                std::vector<double> output = myNN.run(inputVec);
                
                for (int i = 0; i < output.size(); ++i) {
                    allSliders[i] = int(output[i]);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    myo.stop();
}


//--------------------------------------------------------------
void ofApp::draw(){
    //ofClear(0);
    ofDrawBitmapString(currentBeat, 100, 300);
    ofDrawBitmapString("Hold space to record", 200, 300);
    
    guiTong.draw();
    guiThung.draw();
    guiGeneral.draw();
    
    if (inputDevice) {
        ofSetColor(255);
        for ( int i=0; i<myo.getDevices().size(); i++ ) {
            stringstream s;
            s << "id: " << myo.getDevices()[i]->getId() << endl;
            s << "which: " << myo.getDevices()[i]->getWhichArm() << endl;
            s << "pose: " << myo.getDevices()[i]->getPose() << endl;
            s << "accel:          ";
            s << myo.getDevices()[i]->getAccel().x << ",";
            s << myo.getDevices()[i]->getAccel().y << ",";
            s << myo.getDevices()[i]->getAccel().z << endl;
            s << "gyro:           ";
            s << myo.getDevices()[i]->getGyro().x << ",";
            s << myo.getDevices()[i]->getGyro().y << ",";
            s << myo.getDevices()[i]->getGyro().z << endl;
            s << "quaternion:     ";
            s << myo.getDevices()[i]->getQuaternion().x() << ",";
            s << myo.getDevices()[i]->getQuaternion().y() << ",";
            s << myo.getDevices()[i]->getQuaternion().z() << ",";
            s << myo.getDevices()[i]->getQuaternion().w() << endl;
            s << "roll/pitch/yaw: ";
            s << myo.getDevices()[i]->getRoll() << ",";
            s << myo.getDevices()[i]->getPitch() << ",";
            s << myo.getDevices()[i]->getYaw() << endl;
            s << "raw data:       ";
            for ( int j=0; j<8; j++ ) {
                s << myo.getDevices()[i]->getEmgSamples()[j];
                s << ",";
            }
            s << endl;
            ofSetColor(0);
            ofDrawBitmapString(s.str(), 10, 400 + i*100);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::probsClearPressed() {
    std::cout << "clearing probs" << std::endl;
    for (int i = 0; i < allSliders.size(); ++i) {
        allSliders[i] = 0;
    }
}

//--------------------------------------------------------------
void ofApp::resetModelPressed() {
    std::cout << "resetting models" << std::endl;
    myNN.reset();
    trainingSet.clear();
    modelControl = false;
}

//--------------------------------------------------------------
bool eventTest(int prob) {
    int testRand = rand() % 100;
    if (testRand < prob) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    //probs = { tong0, tong1, tong2, tong3, tong4, tong5, tong6, tong7, tong8, tong9, tong10, tong11 };
    //probs2 = { thung0, thung1, thung2, thung3, thung4, thung5, thung6, thung7, thung8, thung9, thung10, thung11 };
    bool beatsTong[12];
    bool beats2[12];
    for (int i = 0; i < 12; ++i) {
        beatsTong[i] = eventTest(allSliders[i]);
        beats2[i] = eventTest(allSliders[i + 12]);
    }
    int lastCount = 0;
    int testMe = 0;
    for (int i = 0; i < bufferSize; i++){
        
        myClock.ticker();
        if (myClock.tick) {
            if (beatsTong[currentBeat]) {
                ciblon_tong.trigger();
            }
            if (beats2[currentBeat]) {
                ciblon_thung.trigger();
            }
            currentBeat = (currentBeat + 1) % 12;
        }
        
        outputs[0] = ciblon_tong.playOnce() * gain;
        outputs[1] = ciblon_thung.playOnce() * gain;
        output[i*nChannels    ] = outputs[0];
        output[i*nChannels + 1] = outputs[1];
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //std::cout << "key: " << key << std::endl;
    switch(key) {
        case 32:
            recordingState = 1;
            break;
        case 13:
            modelControl = (modelControl) ? false : true;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    recordingState = 0;
    if (trainingSet.size() > 0) {
        trained = myNN.train(trainingSet);
        std::cout << "trained: " << trained << std::endl;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (inputDevice == false) { //don't do this with Myo is on
        if (recordingState > 0) {
            trainingExample<double> tempExample;
            tempExample.input = { double(x), double(y) };
            for (int i = 0; i < allSliders.size() ; ++i) {
                tempExample.output.push_back(double(allSliders[i]));
            }
            trainingSet.push_back(tempExample);
        } else if (trained && modelControl == 1) {
            std::vector<double> inputVec;
            inputVec.push_back(double(x));
            inputVec.push_back(double(y));
            std::vector<double> output = myNN.run(inputVec);
            
            for (int i = 0; i < output.size(); ++i) {
                allSliders[i] = int(output[i]);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
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
