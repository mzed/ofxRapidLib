/*
*
* Created by Michael Zbyszynski, 7/05/2018
* Copyright (c) 2018 Michael Zbyszynski
* All rights reserved.
*  Permission is hereby granted, free of charge, to any person
*  obtaining a copy of this software and associated documentation
*  files (the "Software"), to deal in the Software without
*  restriction, including without limitation the rights to use,
*  copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the
*  Software is furnished to do so, subject to the following conditions:
*  The above copyright notice and this permission notice shall be
*  included in all copies or substantial portions of the Software.
*  Neither the name of Goldsmiths nor the names of its
*  contributors may be used to endorse or promote products derived from
*  this software without specific prior written permission.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
*    OTHER DEALINGS IN THE SOFTWARE.
*
*/

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
