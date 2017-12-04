/**
 * @file    rapidStream.h
 * @author  Michael Zbyszynski
 * @date    6 Feb 2017
 * @copyright Copyright © 2017 Goldsmiths. All rights reserved.
 */

#ifndef rapidStream_h
#define rapidStream_h

#include <stdint.h>
#include <atomic>
#include "../dependencies/bayesfilter/BayesianFilter.h"

template<typename T>
class rapidStream {
public:
    
    /**
     * Create a circular buffer with 3 elements.
     */
    rapidStream();
    /**
     * Create a circular buffer with an arbitrary number of elements.
     * @param int: number of elements to hold in the buffer
     */
    rapidStream(int windowSize);
    
    ~rapidStream();
    
    /**
     * Resets all the values in the buffer to zero.
     */
    void clear();
    
    /** Add a value to a circular buffer whose size is defined at creation.
     * @param double: value to be pushed into circular buffer.
     */
    void pushToWindow(T input);
    
    /** Calculate the first-order difference (aka velocity) between the last two inputs.
     * @return double: difference between last two inputs.
     */
    T velocity();
    
    /** Calculate the second-order difference (aka acceleration) over the last three inputs.
     * @return double: acceleration over the last three inputs.
     */
    T acceleration();
    
    /** Find the minimum value in the buffer.
     * @return double: minimum.
     */
    T minimum();
    
    /** Find the maximum value in the buffer.
     * @return double: maximum.
     */
    T maximum();
    
    /** Calculate the sum of all values in the buffer.
     * @return double: sum.
     */
    T sum();
    
    /** Calculate the mean of all values in the buffer.
     * @return double: mean.
     */
    T mean();
    
    /** Calculate the standard deviation of all values in the buffer.
     * @return double: standard deviation.
     */
    T standardDeviation();
    
    /** Calculate the root mean square of the values in the buffer
     * @return double: rms
     */
     T rms();
    
    /** Non-linear Baysian filtering for EMG envelope extraction.
     * @return current envelope value
     */
    T bayesFilter(T inputValue);
    void bayesSetDiffusion(float logDiffusion);
    void bayesSetJumpRate(float jump_rate);
    void bayesSetMVC(float mvc);
    
    /** Calculate the minimum first-order difference over consecutive inputs in the buffer.
     * @return double: minimum velocity.
     */
    T minVelocity();
    
    /** Calculate the maximum first-order difference over consecutive inputs in the buffer.
     * @return double: maximum velocity.
     */
    T maxVelocity();
    
    /** Calculate the minimum second-order difference over consecutive inputs in the buffer.
     * @return double: minimum acceleration.
     */
    T minAcceleration();
    
    /** Calculate the maximum second-order difference over consecutive inputs in the buffer.
     * @return double: maximum acceleration.
     */
    T maxAcceleration();

private:
    uint32_t windowSize;
    uint32_t windowIndex;
    T *circularWindow;
    
    T calcCurrentVel(int i);
    
    BayesianFilter bayesFilt;
};


#endif
