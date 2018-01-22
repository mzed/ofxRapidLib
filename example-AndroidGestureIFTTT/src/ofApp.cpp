#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// initialize the accelerometer
	ofxAccelerometer.setup(  );
	ifttt.setup(  );

	font.load( "type/verdana.ttf", 24 );
	classifierFont.load( "type/verdana.ttf", 80 );
	ofSetLineWidth( 10 );
	ofBackground( 0, 0, 0 );
}

//--------------------------------------------------------------
void ofApp::update(){
	accel = ofxAccelerometer.getForce();
	normAccel = ofVec3f( accel ).normalize(  );
	dAccel = accel - oldAccel;
	xyzn = " gn(x) = " + ofToString( normAccel.x, 2 );
    xyzn += " gn(y) = " + ofToString( normAccel.y, 2 );
    xyzn += " gn(z) = " + ofToString( normAccel.z, 2 );
    oldAccel = accel;

    float currentForce = accel.length(  ); // The magnitude of all combined forces

    // Segmentation
    if ( recognitionRunning )
    {
        if ( collectTrainingData )
        {
            // Collect training data
            tempSeries.input.push_back( {double( normAccel.x ), double( normAccel.y ), double( normAccel.z ) } );
        } else {
            // Process data and check for gesture
            segment.push_back( {double( normAccel.x ), double( normAccel.y ), double( normAccel.z ) } );
            if ( trained )
                classLabel = "Class Found: " + classifier.run( segment );
        }
        // Check if recognition should stop
        if ( currentForce < stopRecognitionForce )
        {
            // Wait until threshold of time has also been surpassed
            if ( segmentationTimer.getTimeElapsed(  ) >= stopRecognitionTime )
            {
                // TrainingData state
                if ( collectTrainingData )
                {
                    collectTrainingData = false;
                    // Add the trainingSet and train the model
                    trainingSet.push_back( tempSeries );
                    trained = classifier.train( trainingSet );
                } else {
                    // Do something with recognized gesture
                    // Uncomment for IFTTT
                    // ifttt.triggerEvent( "your-event-name", "your-key-for-webhooks", classLabel, ofToString( currentForce ), "something else" );

                }

                recognitionRunning = false;
                segmentationTimer.reset(  );
            }
        } else {
            segmentationTimer.reset(  );
        }
    } else {
       // Check if recognition should start
        if ( currentForce > startRecognitionForce )
        {
            // Wait until threshold of time has also been surpassed
            if ( segmentationTimer.getTimeElapsed(  ) >= startRecognitionTime )
            {
                recognitionRunning = true;
                segment.clear(  );
                segmentationTimer.reset(  );
            }
        } else {
            segmentationTimer.reset(  );
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( collectTrainingData )
    {
        if ( recognitionRunning )
            ofBackground( 150, 0, 0 );
        else
            ofBackground( 75, 20, 20 );
    } else {
        if ( recognitionRunning )
            ofBackground( 0, 0, 150 );
        else
            ofBackground( 0, 150, 0 );
    }

    ofSetColor( 255, 255, 255, 255 );

    // Draw the accelerometer values
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

	ofSetColor(255,255,0);
	ofDrawLine(0,0,normAccel.x*ofGetWidth(  )/2,0);
	ofSetColor(255,0,255);
	ofDrawLine(0,0,0,-normAccel.y*ofGetHeight(  )/2);
	ofSetColor(100,255,100);
    ofDrawLine(0,0,normAccel.z*ofGetWidth(  )/2,-normAccel.z*ofGetHeight(  )/2);
	ofPopMatrix();
    
	ofSetColor( 255, 255, 0, 255 );
    font.drawString( xyzn, 10, font.stringHeight( xyzn ) + 24 );
    ofSetColor( 255, 255, 255, 175 ); // Replace with class color
    classifierFont.drawString( classLabel, ( ofGetWidth(  ) / 2 ) - ( classifierFont.stringWidth( classLabel ) / 2 ),
                               ( ofGetHeight(  ) / 2 ) - ( classifierFont.stringHeight( classLabel ) ) / 2 );
    ofSetColor( 255, 255, 255, 255 );
}

//--------------------------------------------------------------
void ofApp::keyPressed  ( int key ) {
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key ) {

}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h ){

}

//--------------------------------------------------------------
void ofApp::touchDown( int x, int y, int id ) {
    if ( id == 9 ) {
        trainingSet.clear(  );
        classLabel = "Cleared";
    } else {
        collectTrainingData = true;
        id += 1;
        classLabel = "Selected: " + ofToString( id );
        tempSeries.input.clear(  );
        tempSeries.label = ofToString( id );
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved( int x, int y, int id ) {
}

//--------------------------------------------------------------
void ofApp::touchUp( int x, int y, int id ) {
}


//--------------------------------------------------------------
void ofApp::touchDoubleTap( int x, int y, int id ) {
    collectTrainingData = false;
}

//--------------------------------------------------------------
void ofApp::touchCancelled( int x, int y, int id ) {

}

//--------------------------------------------------------------
void ofApp::swipe( ofxAndroidSwipeDir swipeDir, int id ) {
}

//--------------------------------------------------------------
void ofApp::pause ( void ) {

}

//--------------------------------------------------------------
void ofApp::stop ( void ) {

}

//--------------------------------------------------------------
void ofApp::resume ( void ) {

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
