#include "ofApp.h"

#define SCALE_FACTOR 4

//--------------------------------------------------------------
void ofApp::setup()
{    
    ofBackground(0,0,0);
    
    camWidth 		= 160;	// try to grab at this size.
    camHeight 		= 120;
    
    vidGrabber.setVerbose(true);
    vidGrabber.setup(camWidth,camHeight);
    
    font.load("Courier New Bold.ttf", 20);
    
    // this set of characters comes from processing:
    //http://processing.org/learning/library/asciivideo.html
    
    // changed order slightly to work better for mapping
    asciiCharacters =  string("  ..,,,'''``--_:;^^**""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q");
    
    ofEnableAlphaBlending();
    
    sampleRate = 44100;
    bufferSize = 512;
    
    std:cout << "Sample 1 loaded = " << gam_1.load(ofToDataPath("261938__digitopia-cdm__saron-sdpl1.wav")) << std::endl;
    gam_3.load(ofToDataPath("261730__digitopia-cdm__saron-sdpl3.wav"));
    gam_4.load(ofToDataPath("261927__digitopia-cdm__saron-sdpl4.wav"));
    gam_5.load(ofToDataPath("261924__digitopia-cdm__saron-sdpl5.wav"));
    gam_6.load(ofToDataPath("261968__digitopia-cdm__saron-sdpl6.wav"));
    gam_7.load(ofToDataPath("261883__digitopia-cdm__saron-sdpl7.wav"));
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
}

//--------------------------------------------------------------
void ofApp::exit()
{
    ofSoundStreamStop();
    ofSoundStreamClose();
}

//--------------------------------------------------------------
void ofApp::update()
{
    vidGrabber.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // change background video alpha value based on the mouse position
    float videoAlphaValue = ofMap(mouseX, 0,ofGetWidth(),0,255);
    
    // set a white fill color with the alpha generated above
    ofSetColor(255,255,255,videoAlphaValue);
    
    // draw the raw video frame with the alpha value generated above
    vidGrabber.draw(0,0,640,480);
    
    ofPixelsRef pixelsRef = vidGrabber.getPixels();
    
    ofSetHexColor(0xffffff);
    
    //-------RAPID-MIX---------------//
    std::vector<double> trainingInput;
    std::vector<double> trainingOutput;
    
    switch (recordingState) 
    {
        case 1:
            ofSetColor(255, 0, 0);
            break;
        case 2:
            ofSetColor(0, 255, 0);
            break;
        case 3:
            ofSetColor(0, 0, 255);
            break;
        default:
            ofSetColor(255, 255, 255);
    }
    
    
    for (int i = 0; i < camWidth; i += 5)
    {
        for (int j = 0; j < camHeight; j += 5)
        {
            // get the pixel and its lightness (lightness is the average of its RGB values)
            float lightness = pixelsRef.getColor(i,j).getLightness();
            //RAPIDMIX
            trainingInput.push_back(double(lightness));
            
            if (!runToggle) 
            {
                // calculate the index of the character from our asciiCharacters array
                int character = powf( ofMap(lightness, 0, 255, 0, 1), 2.5) * asciiCharacters.size();
                // draw the character at the correct location
                font.drawString(ofToString(asciiCharacters[character]), i * SCALE_FACTOR, j * SCALE_FACTOR);
            }
        }
    }
    
    if (runToggle) 
    {
        result = myKnn.run(trainingInput)[0];
        for (int i = 0; i < camWidth; i += 5)
        {
            for (int j = 0; j < camHeight; j += 5)
            {
                float lightness = pixelsRef.getColor(i,j).getLightness();
                switch (result) 
                {
                    case 1:
                        ofSetColor(lightness, 0, 0, lightness);
                        break;
                    case 2:
                        ofSetColor(0, lightness, 0, lightness);
                        break;
                    case 3:
                        ofSetColor(0, 0, lightness, lightness);
                        break;
                    default:
                        ofSetColor(lightness, lightness, lightness, lightness);
                }
                font.drawString(ofToString(result), i * SCALE_FACTOR, j * SCALE_FACTOR);
            }
        }
    }
    
    if (recordingState) 
    {
        trainingOutput = { double(recordingState) };
        rapidLib::trainingExample tempExample;
        tempExample.input = trainingInput;
        tempExample.output = trainingOutput;
        myData.push_back(tempExample);
    }
}


//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& output)
{
    for (int i = 0; i < output.getNumFrames(); ++i)
    {
        currentCount=(int)timer.phasor(8);//this sets up a metronome that ticks 8 times a second
        
        if (lastCount!=currentCount)
        {//if we have a new timer int this sample, play the sound
            //This is a 16-step step sequencer
            switch (playHead%16)
            {
                case 1:
                    gam_4.trigger();
                    break;
                case 5:
                    if (result == 3 || result == 2) gam_6.trigger();
                    break;
                case 7:
                    if (result == 1) gam_5.trigger();
                    break;
                case 9:
                    if (result == 1) {
                        gam_6.trigger();
                    }
                    break;
                case 11:
                    if (result == 2) {
                        gam_7.trigger();
                    }
                    break;
                case 13:
                    if (result == 3) gam_1.trigger();
                    break;
                case 15:
                    if (result == 2) gam_3.trigger();
            }
            
            ++playHead;//iterate the playhead
            lastCount = 0;//reset the metrotest
        }
        
        double gamOutput = gam_1.playOnce() + gam_3.playOnce() + gam_4.playOnce() + gam_5.playOnce() + gam_6.playOnce() + gam_7.playOnce();
    
        mymix.stereo(gamOutput, outputs, 0.5);
        output[i * output.getNumChannels()    ] = outputs[0];
        output[i * output.getNumChannels() + 1] = outputs[1];
    
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key)
{
    // in fullscreen mode, on a pc at least, the
    // first time video settings the come up
    // they come up *under* the fullscreen window
    // use alt-tab to navigate to the settings
    // window. we are working on a fix for this...
    
    if (key == 's' || key == 'S') vidGrabber.videoSettings();
    
    switch(key)
    {
        case 49:
            recordingState = 1;
            break;
        case 50:
            recordingState = 2;
            break;
        case 51:
            recordingState = 3;
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
void ofApp::keyReleased(int key)
{
    recordingState = 0;
    if (myData.size() > 0) myKnn.train(myData);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
