/*
 * rapidStream.h
 * Created by Michael Zbyszynski on 6 Feb 2017
 * Copyright © 2017 Goldsmiths. All rights reserved.
 */

#ifndef rapidStream_h
#define rapidStream_h

#include <stdint.h>
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
    void pushToWindow(double input);
    
    /** Calculate the first-order difference (aka velocity) between the last two inputs.
     * @return double: difference between last two inputs.
     */
    double velocity();
    
    /** Calculate the second-order difference (aka acceleration) over the last three inputs.
     * @return double: acceleration over the last three inputs.
     */
    double acceleration();
    
    /** Find the minimum value in the buffer.
     * @return double: minimum.
     */
    double minimum();
    
    /** Find the maximum value in the buffer.
     * @return double: maximum.
     */
    double maximum();
    
    /** Calculate the sum of all values in the buffer.
     * @return double: sum.
     */
    double sum();
    
    /** Calculate the mean of all values in the buffer.
     * @return double: mean.
     */
    double mean();
    
    /** Calculate the standard deviation of all values in the buffer.
     * @return double: standard deviation.
     */
    double standardDeviation();
    
    /** Calculate the root mean square of the values in the buffer
     * @return double: rms
     */
     double rms();
    
    /** Calculate the minimum first-order difference over consecutive inputs in the buffer.
     * @return double: minimum velocity.
     */
    double minVelocity();
    
    /** Calculate the maximum first-order difference over consecutive inputs in the buffer.
     * @return double: maximum velocity.
     */
    double maxVelocity();
    
    /** Calculate the minimum second-order difference over consecutive inputs in the buffer.
     * @return double: minimum acceleration.
     */
    double minAcceleration();
    
    /** Calculate the maximum second-order difference over consecutive inputs in the buffer.
     * @return double: maximum acceleration.
     */
    double maxAcceleration();

private:
    uint32_t windowSize;
    uint32_t windowIndex;
    double *circularWindow;
    
    double calcCurrentVel(int i);
};


#endif