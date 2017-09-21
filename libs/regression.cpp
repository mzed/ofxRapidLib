#include <vector>
#include "regression.h"


#ifdef EMSCRIPTEN
#include "emscripten/regressionEmbindings.h"
#endif

regression::regression() {
    numInputs = 0;
    numOutputs = 0;
    numHiddenLayers = 1;
    numEpochs = 500;
    created = false;
};

regression::regression(const int &num_inputs, const int &num_outputs) {
    numInputs = num_inputs;
    numOutputs = num_outputs;
    numHiddenLayers = 1;
    numEpochs = 500;
    created = false;
    std::vector<int> whichInputs;
    for (int i = 0; i < numInputs; ++i) {
        whichInputs.push_back(i);
    }
    for (int i = 0; i < numOutputs; ++i) {
        myModelSet.push_back(new neuralNetwork(numInputs, whichInputs, numHiddenLayers, numInputs));
    }
    created = true;
};

regression::regression(const std::vector<trainingExample> &training_set) {
    numInputs = 0;
    numOutputs = 0;
    created = false;
    train(training_set);
};

std::vector<int> regression::getNumHiddenLayers() {
    std::vector<int> vecNumHiddenLayers;
    if (std::begin(myModelSet) != std::end(myModelSet)) {
        for (baseModel* model : myModelSet) {
            neuralNetwork* nnModel = dynamic_cast<neuralNetwork*>(model); //FIXME: I really dislike this design
            vecNumHiddenLayers.push_back(nnModel->getNumHiddenLayers());
        }
    } else {
        vecNumHiddenLayers = { numHiddenLayers };
    }
    return vecNumHiddenLayers;
}

void regression::setNumHiddenLayers(const int &num_hidden_layers){
    numHiddenLayers = num_hidden_layers;
    //Set any existing models
    if (std::begin(myModelSet) != std::end(myModelSet)) {
        for (baseModel* model : myModelSet) {
            neuralNetwork* nnModel = dynamic_cast<neuralNetwork*>(model); //FIXME: I really dislike this design
            nnModel->setNumHiddenLayers(num_hidden_layers);
        }
    }
}

void regression::setNumEpochs(const int &epochs) {
    numEpochs = epochs;
    //set any existing models
    if (std::begin(myModelSet) != std::end(myModelSet)) {
        for (baseModel* model : myModelSet) {
            neuralNetwork* nnModel = dynamic_cast<neuralNetwork*>(model); //FIXME: I really dislike this design
            nnModel->setEpochs(epochs);
        }
    }
}


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
                myModelSet.push_back(new neuralNetwork(numInputs, whichInputs, numHiddenLayers, numInputs));
            }
            if (numEpochs != 500) {
                for (baseModel* model : myModelSet) {
                    neuralNetwork* nnModel = dynamic_cast<neuralNetwork*>(model); //FIXME: I really dislike this design
                    nnModel->setEpochs(numEpochs);
                }
            }
            created = true;
            return modelSet::train(training_set);
        }
    }
    return false;
}
