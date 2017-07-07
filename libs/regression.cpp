#include <vector>
#include "regression.h"


#ifdef EMSCRIPTEN
#include "emscripten/regressionEmbindings.h"
#endif

regression::regression() {
    numInputs = 0;
    numOutputs = 0;
    created = false;
};

regression::regression(const int &num_inputs, const int &num_outputs) {
    numInputs = num_inputs;
    numOutputs = num_outputs;
    created = false;
    std::vector<int> whichInputs;
    for (int i = 0; i < numInputs; ++i) {
        whichInputs.push_back(i);
    }
    for (int i = 0; i < numOutputs; ++i) {
        myModelSet.push_back(new neuralNetwork(numInputs, whichInputs, 1, numInputs));
    }
    created = true;
};

regression::regression(const std::vector<trainingExample> &training_set) {
    numInputs = 0;
    numOutputs = 0;
    created = false;
    train(training_set);
};

bool regression::train(const std::vector<trainingExample> &training_set) {
    //TODO: time this process?
    if (training_set.size() > 0) {
        if (created) {
            return modelSet::train(training_set);
        } else {
            //create model(s) here
            numInputs = int(training_set[0].input.size());
            for (int i = 0; i < numInputs; ++i) {
                inputNames.push_back("inputs-" + std::to_string(i + 1));
            }
            numOutputs = int(training_set[0].output.size());
            for ( auto example : training_set) {
                if (example.input.size() != numInputs) {
                    return false;
                }
                if (example.output.size() != numOutputs) {
                    return false;
                }
            }
            std::vector<int> whichInputs;
            for (int j = 0; j < numInputs; ++j) {
                whichInputs.push_back(j);
            }
            for (int i = 0; i < numOutputs; ++i) {
                myModelSet.push_back(new neuralNetwork(numInputs, whichInputs, 1, numInputs));
            }
            created = true;
            return modelSet::train(training_set);
        }
    }
    return false;
}
