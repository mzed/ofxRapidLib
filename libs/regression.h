#ifndef regression_h
#define regression_h

#include <vector>
#include "modelSet.h"

/*! Class for implementing a set of regression models.
 *
 * This doesn't do anything modelSet can't do. But, it's simpler and more like wekinator.
 */

class regression : public modelSet {
public:
    /** with no arguments, just make an empty vector */
    regression();
    /** create based on training set inputs and outputs */
    regression(const std::vector<trainingExample> &trainingSet);
    /** create with proper models, but not trained */
    regression(const int &numInputs, const int &numOutputs);
    
    /** destructor */
    ~regression() {};
    
    /** Train on a specified set, causes creation if not created */
    bool train(const std::vector<trainingExample> &trainingSet);
    
};

#endif
