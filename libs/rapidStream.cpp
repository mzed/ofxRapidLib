/*
 * rapidStream.cpp
 * Created by Michael Zbyszynski on 6 Feb 2017
 * Copyright © 2017 Goldsmiths. All rights reserved.
 */

#include "rapidStream.h"
#include <iostream>
#include <cmath>
#include <limits>

#ifdef EMSCRIPTEN
#include "emscripten/rapidStreamEmbindings.h"
#endif


rapidStream::rapidStream(int window_size) {
    windowSize = window_size;
    windowIndex = 0;
    circularWindow = new double[window_size];
    for (int i = 0; i < windowSize; ++i) {
        circularWindow[i] = 0;
    }
}

rapidStream::rapidStream() {
    windowSize = 3;
    windowIndex = 0;
    circularWindow = new double[windowSize];
    for (int i = 0; i < windowSize; ++i) {
        circularWindow[i] = 0;
    }
}

rapidStream::~rapidStream() {
    delete []circularWindow;
}

void rapidStream::clear() {
    windowIndex = 0;
    circularWindow = new double[windowSize];
    for (int i = 0; i < windowSize; ++i) {
        circularWindow[i] = 0;
    }
}

void rapidStream::pushToWindow(double input) {
    circularWindow[windowIndex] = input;
    windowIndex = (windowIndex + 1) % windowSize;
}

inline double rapidStream::calcCurrentVel(int i) {
    return circularWindow[(i + windowIndex) % windowSize] - circularWindow[(i + windowIndex - 1) % windowSize];
}

double rapidStream::velocity() {
    return calcCurrentVel(-1);
};

double rapidStream::acceleration() {
    return calcCurrentVel(-2) - calcCurrentVel(-3);
};

double rapidStream::minimum() {
    double minimum = std::numeric_limits<double>::infinity();
    for (int i = 0; i < windowSize; ++i) {
        if (circularWindow[i] < minimum) {
            minimum = circularWindow[i];
        }
    }
    return minimum;
}

double rapidStream::maximum() {
    double maximum = std::numeric_limits<double>::min();
    for (int i = 0; i < windowSize; ++i) {
        if (circularWindow[i] > maximum) {
            maximum = circularWindow[i];
        }
    }
    return maximum;
}

double rapidStream::sum() {
    double newSum = 0;
    for(int i = 0; i < windowSize; ++i)
    {
        newSum += circularWindow[i];
    }
    return newSum;
}

double rapidStream::mean() {
    return sum()/windowSize;
}

double rapidStream::standardDeviation() {
    double newMean = mean();
    double standardDeviation = 0.;
    for(int i = 0; i < windowSize; ++i) {
        standardDeviation += pow(circularWindow[i] - newMean, 2);
    }
    return sqrt(standardDeviation / windowSize);
}

double rapidStream::rms() {
    double rms;
    for (int i = 0; i < windowSize; ++i) {
        rms += (circularWindow[i] * circularWindow[i]);
    }
    rms = rms/windowSize;
    return sqrt(rms);
}

double rapidStream::minVelocity() {
    double minVel = std::numeric_limits<double>::infinity();
    for (int i = 0; i < windowSize; ++i) {
        double currentVel = calcCurrentVel(i);
        if ( currentVel < minVel) {
            minVel = currentVel;
        }
    }
    return minVel;
}

double rapidStream::maxVelocity() {
    double maxVel = std::numeric_limits<double>::lowest();
    for (int i = 0; i < windowSize; ++i) {
        double currentVel = calcCurrentVel(i);
        if (currentVel > maxVel) {
            maxVel = currentVel;
        }
    }
    return maxVel;
}

double rapidStream::minAcceleration() {
    double minAccel = std::numeric_limits<double>::infinity();
    double lastVel = calcCurrentVel(1);
    for (int i = 2; i < windowSize; ++i) {
        double currentVel = calcCurrentVel(i);
        double currentAccel =  currentVel - lastVel;
        lastVel = currentVel;
            if (currentAccel < minAccel) {
            minAccel = currentAccel;
        }
    }
    return minAccel;
}

double rapidStream::maxAcceleration() {
    double maxAccel = std::numeric_limits<double>::lowest();
    double lastVel = calcCurrentVel(1);
    for (int i = 2; i < windowSize; ++i) {
        double currentVel = calcCurrentVel(i);
        double currentAccel = currentVel - lastVel;
        lastVel = currentVel;
        if (currentAccel > maxAccel) {
            maxAccel = currentAccel;
        }
    }
    return maxAccel;
}
