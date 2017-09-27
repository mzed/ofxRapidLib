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

template<typename T>
rapidStream<T>::rapidStream(int window_size) {
    windowSize = window_size;
    windowIndex = 0;
    circularWindow = new T[window_size];
    for (int i = 0; i < windowSize; ++i) {
        circularWindow[i] = 0;
    }
}

template<typename T>
rapidStream<T>::rapidStream() {
    windowSize = 3;
    windowIndex = 0;
    circularWindow = new T[windowSize];
    for (int i = 0; i < windowSize; ++i) {
        circularWindow[i] = 0;
    }
}

template<typename T>
rapidStream<T>::~rapidStream() {
    delete []circularWindow;
}

template<typename T>
void rapidStream<T>::clear() {
    windowIndex = 0;
    circularWindow = new T[windowSize];
    for (int i = 0; i < windowSize; ++i) {
        circularWindow[i] = 0;
    }
}

template<typename T>
void rapidStream<T>::pushToWindow(T input) {
    circularWindow[windowIndex] = input;
    windowIndex = (windowIndex + 1) % windowSize;
}

template<typename T>
inline T rapidStream<T>::calcCurrentVel(int i) {
    return circularWindow[(i + windowIndex) % windowSize] - circularWindow[(i + windowIndex - 1) % windowSize];
}

template<typename T>
T rapidStream<T>::velocity() {
    return calcCurrentVel(-1);
};

template<typename T>
T rapidStream<T>::acceleration() {
    return calcCurrentVel(-2) - calcCurrentVel(-3);
};

template<typename T>
T rapidStream<T>::minimum() {
    T minimum = std::numeric_limits<T>::infinity();
    for (int i = 0; i < windowSize; ++i) {
        if (circularWindow[i] < minimum) {
            minimum = circularWindow[i];
        }
    }
    return minimum;
}

template<typename T>
T rapidStream<T>::maximum() {
    T maximum = std::numeric_limits<T>::min();
    for (int i = 0; i < windowSize; ++i) {
        if (circularWindow[i] > maximum) {
            maximum = circularWindow[i];
        }
    }
    return maximum;
}

template<typename T>
T rapidStream<T>::sum() {
    T newSum = 0;
    for(int i = 0; i < windowSize; ++i)
    {
        newSum += circularWindow[i];
    }
    return newSum;
}

template<typename T>
T rapidStream<T>::mean() {
    return sum()/windowSize;
}

template<typename T>
T rapidStream<T>::standardDeviation() {
    T newMean = mean();
    T standardDeviation = 0.;
    for(int i = 0; i < windowSize; ++i) {
        standardDeviation += pow(circularWindow[i] - newMean, 2);
    }
    return sqrt(standardDeviation / windowSize);
}

template<typename T>
T rapidStream<T>::rms() {
    T rms;
    for (int i = 0; i < windowSize; ++i) {
        rms += (circularWindow[i] * circularWindow[i]);
    }
    rms = rms/windowSize;
    return sqrt(rms);
}

template<typename T>
T rapidStream<T>::minVelocity() {
    T minVel = std::numeric_limits<T>::infinity();
    for (int i = 0; i < windowSize; ++i) {
        T currentVel = calcCurrentVel(i);
        if ( currentVel < minVel) {
            minVel = currentVel;
        }
    }
    return minVel;
}

template<typename T>
T rapidStream<T>::maxVelocity() {
    T maxVel = std::numeric_limits<T>::lowest();
    for (int i = 0; i < windowSize; ++i) {
        T currentVel = calcCurrentVel(i);
        if (currentVel > maxVel) {
            maxVel = currentVel;
        }
    }
    return maxVel;
}

template<typename T>
T rapidStream<T>::minAcceleration() {
    T minAccel = std::numeric_limits<T>::infinity();
    T lastVel = calcCurrentVel(1);
    for (int i = 2; i < windowSize; ++i) {
        T currentVel = calcCurrentVel(i);
        T currentAccel =  currentVel - lastVel;
        lastVel = currentVel;
            if (currentAccel < minAccel) {
            minAccel = currentAccel;
        }
    }
    return minAccel;
}

template<typename T>
T rapidStream<T>::maxAcceleration() {
    T maxAccel = std::numeric_limits<T>::lowest();
    T lastVel = calcCurrentVel(1);
    for (int i = 2; i < windowSize; ++i) {
        T currentVel = calcCurrentVel(i);
        T currentAccel = currentVel - lastVel;
        lastVel = currentVel;
        if (currentAccel > maxAccel) {
            maxAccel = currentAccel;
        }
    }
    return maxAccel;
}

//explicit instantiation
template class rapidStream<double>;
template class rapidStream<float>;

