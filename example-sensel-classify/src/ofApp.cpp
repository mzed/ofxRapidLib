#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0,0,0);
    ofEnableAlphaBlending();
    
    //Sensel
    senselGetDeviceList(&senselList);
    if (senselList.num_devices == 0)
    {
        std::cerr << "No device found" << std::endl;
        senselFound = false;
    } else {
        
        senselFound = true;
        senselOpenDeviceByID(&senselHandle, senselList.devices[0].idx);
        senselGetSensorInfo(senselHandle, &senselInfo);
        
        std::cout << "Sensel Device: " <<senselList.devices[0].serial_num << std::endl;
        std::cout << "Width: " << senselInfo.width << "mm" << std::endl;
        std::cout << "Height: " << senselInfo.height << "mm" << std::endl;
        std::cout << "Cols: " << senselInfo.num_cols << std::endl;
        std::cout << "Rows: " << senselInfo.num_rows <<std::endl;
        
        senselSetFrameContent(senselHandle, FRAME_CONTENT_PRESSURE_MASK);
        senselAllocateFrameData(senselHandle, &senselFrame);
        //Start scanning the Sensel device
        senselStartScanning(senselHandle);
    }
    
    //Maxim
    sampleRate 	= 44100; /* Sampling Rate */
    bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    loop_1.load(ofToDataPath("SOLAR.WAV"));
    loop_2.load(ofToDataPath("DE-LA-~1.WAV"));
    loop_3.load(ofToDataPath("ENGLISH.WAV"));
    
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    ofSoundStreamSetup(2,0,this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
}

//--------------------------------------------------------------
void ofApp::exit(){
    if (senselFound) {
        senselClose(senselHandle);
    }
    ofSoundStreamStop();
    ofSoundStreamClose();
}

//--------------------------------------------------------------
void ofApp::update(){
    if (senselFound) {
        //------- SENSEL ---------------//
        std::vector<double> senselInput;
        uint numFrames=0;
        senselReadSensor(senselHandle);
        senselGetNumAvailableFrames(senselHandle, &numFrames);
        for (int i = 0; i < numFrames; ++i) {
            senselGetFrame(senselHandle, senselFrame);
            senselInput.clear();
            int arraySize = senselInfo.num_cols * senselInfo.num_rows;
            for (int j = 0; j < arraySize; ++j) {
                senselInput.push_back(double(senselFrame->force_array[j]));
            }
        }
        
        //-------RapidLib---------------//
        if (runToggle) {
            result = myKnn.run(senselInput)[0];
            std::cout << "result " << result << std::endl;
            switch (result) {
                case 2:
                    senselSetLEDBrightness(senselHandle, 4, 255);
                    senselSetLEDBrightness(senselHandle, 12, 0);
                    senselSetLEDBrightness(senselHandle, 20, 0);
                    break;
                case 3:
                    senselSetLEDBrightness(senselHandle, 12, 255);
                    senselSetLEDBrightness(senselHandle, 4, 0);
                    senselSetLEDBrightness(senselHandle, 20, 0);
                    break;
                case 4:
                    senselSetLEDBrightness(senselHandle, 20, 255);
                    senselSetLEDBrightness(senselHandle, 4, 0);
                    senselSetLEDBrightness(senselHandle, 12, 0);
                    break;
                default:
                    senselSetLEDBrightness(senselHandle, 4, 0);
                    senselSetLEDBrightness(senselHandle, 12, 0);
                    senselSetLEDBrightness(senselHandle, 20, 0);
            }
        }
        
        if (recordingState) {
            trainingExample tempExample;
            tempExample.input = senselInput;
            tempExample.output = { double(recordingState) };
            trainingSet.push_back(tempExample);
            recordingState = 0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (senselFound) {
        ofDrawBitmapString("Press 1 to record an example of an empty class", 20, 20);
        ofDrawBitmapString("Put an object on the morph and pres 2, 3, or 4 to record", 20, 40);
        ofDrawBitmapString("examples of that class", 20, 55);
        ofDrawBitmapString("identified class " + std::to_string(result), 20, 200);
    } else {
        ofDrawBitmapString("Could not find a sensel device", 20, 200);
    }
}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    for (int i = 0; i < bufferSize; ++i) {
        double loopOutput;
        switch (result) {
            case 2:
                loopOutput = loop_1.play();
                break;
            case 3:
                loopOutput = loop_2.play();
                break;
            case 4:
                loopOutput = loop_3.play();
                break;
        }
        
        mymix.stereo(loopOutput, outputs, 0.5);
        output[i*nChannels    ] = outputs[0];
        output[i*nChannels + 1] = outputs[1];
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key) {
        case 49:
            recordingState = 1;
            break;
        case 50:
            recordingState = 2;
            break;
        case 51:
            recordingState = 3;
            break;
        case 52:
            recordingState = 4;
            break;
        case 32:
            runToggle = (runToggle) ? false : true;
            if (!runToggle) {
                result = 0;
            }
    }
    std::cout << "runToggle " << runToggle << std::endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    recordingState = 0;
    if(trainingSet.size() > 0) {
        myKnn.train(trainingSet);
        std::cout << "Trained with " << trainingSet.size() << " examples." << std::endl;
        runToggle = true;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
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
