![alt text](ofxaddons_thumbnail.png "rapidmix")  

[![GitHub license](https://img.shields.io/badge/License-BSD%203--Clause-brightgreen.svg)](https://github.com/mzed/ofxRepidLib/blob/master/LICENSE)

# ofxRapidLib
ofxRapidLib is an [openFrameworks](http://openframeworks.cc/) wrapper for the RapidLib library. RapidLib is a lightweight, interactive machine learning library intended to be used in interactive music and visual projects. It was directly inspired by Rebecca Fiebrink's [Wekinator](http://www.wekinator.org/), and was written in collaboration with her at Goldsmiths, University of London, as part of the [RAPID-MIX](http://rapidmix.goldsmithsdigital.com/) project.

RapidLib is an open source, cross-platform project and is avaiable under a BSD license.

### The master branch of ofxRapidLib has been tested with:
- ofx_0.10.1
- MacOS 10.14 with XCode 10
- Windows 10 with Visual Studio 2017

# Documentation
### [RapidLib DOxygen documentation](http://doc.gold.ac.uk/eavi/rapidmix/docs_cpp/annotated.html)

### Interactive machine learning

The interactive machine learning API has the following classes:
- classification (k-Nearest Neighbor)
- regression (Neural Network)
- seriesClassification (Dynamic Time Warping)

There are also two classes for holding the data that are used to train machine learning models:
- trainingExample
- trainingSeries

### Basic signal processing

In addition to machine learning, ofxRapidLib provides users with some basic signal processing algorithms for pre-processing incoming sensor data. This is centered around a buffer class, called **rapidStream**. It offers the following functions:
- rapidStream.velocity() (aka first-order difference)
- rapidStream.acceleration() (aka second-order difference)
- rapidStream.minimum() _The smallest value in the buffer_
- rapidStream.maximum() _The largest value in the buffer_
- rapidStream.sum() _sum of all buffered values_
- rapidStream.mean()
- rapidStream.standardDeviation()
- rapidStream.rms() _root mean square of values in the buffer_
- rapidStream.bayesfilter(input) _Bayesian filter for EMG envelope detection_
- rapidStream.minVelocity()
- rapidStream.maxVelocity()
- rapidStream.minAcceleration()
- rapidStream.maxAcceleration()

# Examples  
Description of examples  

# JavaScript
RapidLib has been ported to JavaScript. A node module is maintained [here](https://www.npmjs.com/package/rapidlib) Add it to your node app with:
```
npm install rapidlib
```
The RapidLib JavaScript library also runs client side.  It is extensively documented on CodeCircle. Search for the tag "#RapidLib"
- [RapidLib_001: Basic](https://live.codecircle.com/d/wiCgiE7ogQXFgMEMt)
