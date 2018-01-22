#ifndef _TEST_APP
#define _TEST_APP

/*
 This example works as following:
 - Touch the screen with the number of fingers that you want the class label to represent (1 to 9, 10 is clear)
 - Move the phone to record the gesture for the label you have entered ( number of fingers )
    + Make sure to exceed the detection threshold ( darkred will become lightred ) to record the gesture
 - After recording gestures ( if the screen is green ) replicate gestures to classify
    + Also make sure to exceed the detection threshold ( screen will become blue ) to detect gestures

 - You can change the detection threshold which is called starrtRecognitionForce
 - If you wish to experiment with IFTTT uncomment the line relating to that in the cpp file
    + You need your own webhooks key for that
 */


#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxAccelerometer.h"
#include "ofxRapidLib.h"

#include "Timer.h"
#include "IFTTTThreaded.hpp"

class ofApp : public ofxAndroidApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

        bool recognitionRunning = false;

        float startRecognitionForce = 2.0f; // > 2 g for
        uint64_t startRecognitionTime = 100; // 100 ms
        float stopRecognitionForce =  1.2f; // < 1.2 g for
        uint64_t stopRecognitionTime = 300; // 300 ms

		ofTrueTypeFont font, classifierFont;
		ofVec3f dAccel, accel, normAccel, oldAccel;
		string xyz, xyzn;

		Timer segmentationTimer;

        bool collectTrainingData = false;

		// RapidLib
        seriesClassification classifier;
        std::vector< trainingSeries > trainingSet;
        std::vector< std::vector< double > > segment;
        trainingSeries tempSeries;
        bool trained;
        std::string classLabel = "Start";

        // IFTTT
        IFTTTThreaded ifttt;
};


#endif

