//
//  regression.cpp
//  RapidLib
//
//  Created by mzed on 26/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#include <vector>
#include "regression.h"


#ifdef EMSCRIPTEN
#include "emscripten/regressionEmbindings.h"
#endif

template<typename T>
regression<T>::regression() {
    modelSet<T>::numInputs = 0;
    modelSet<T>::numOutputs = 0;
    numHiddenLayers = 1;
    numEpochs = 500;
    modelSet<T>::created = false;
};

template<typename T>
regression<T>::regression(const int &num_inputs, const int &num_outputs) {
    modelSet<T>::numInputs = num_inputs;
    modelSet<T>::numOutputs = num_outputs;
    numHiddenLayers = 1;
    numEpochs = 500;
    modelSet<T>::created = false;
    std::vector<int> whichInputs;
    for (int i = 0; i < modelSet<T>::numInputs; ++i) {
        whichInputs.push_back(i);
    }
    for (int i = 0; i < modelSet<T>::numOutputs; ++i) {
        modelSet<T>::myModelSet.push_back(new neuralNetwork<T>(modelSet<T>::numInputs, whichInputs, numHiddenLayers, modelSet<T>::numInputs));
    }
    modelSet<T>::created = true;
};

template<typename T>
regression<T>::regression(const std::vector<trainingExample<T> > &training_set) {
    modelSet<T>::numInputs = 0;
    modelSet<T>::numOutputs = 0;
    modelSet<T>::created = false;
    train(training_set);
};

template<typename T>
std::vector<int> regression<T>::getNumHiddenLayers() {
    std::vector<int> vecNumHiddenLayers;
    if (std::begin(modelSet<T>::myModelSet) != std::end(modelSet<T>::myModelSet)) {
        for (baseModel<T>* model : modelSet<T>::myModelSet) {
            neuralNetwork<T>* nnModel = dynamic_cast<neuralNetwork<T>*>(model); //FIXME: I really dislike this design
            vecNumHiddenLayers.push_back(nnModel->getNumHiddenLayers());
        }
    } else {
        vecNumHiddenLayers = { numHiddenLayers };
    }
    return vecNumHiddenLayers;
}

template<typename T>
void regression<T>::setNumHiddenLayers(const int &num_hidden_layers){
    numHiddenLayers = num_hidden_layers;
    //Set any existing models
    if (std::begin(modelSet<T>::myModelSet) != std::end(modelSet<T>::myModelSet)) {
        for (baseModel<T>* model : modelSet<T>::myModelSet) {
            neuralNetwork<T>* nnModel = dynamic_cast<neuralNetwork<T>*>(model); //FIXME: I really dislike this design
            nnModel->setNumHiddenLayers(num_hidden_layers);
        }
    }
}

template<typename T>
void regression<T>::setNumEpochs(const int &epochs) {
    numEpochs = epochs;
    //set any existing models
    if (std::begin(modelSet<T>::myModelSet) != std::end(modelSet<T>::myModelSet)) {
        for (baseModel<T>* model : modelSet<T>::myModelSet) {
            neuralNetwork<T>* nnModel = dynamic_cast<neuralNetwork<T>*>(model); //FIXME: I really dislike this design
            nnModel->setEpochs(epochs);
        }
    }
}

template<typename T>
bool regression<T>::train(const std::vector<trainingExample<T> > &training_set) {
    //TODO: time this process?
    if (training_set.size() > 0) {
        if (modelSet<T>::created) {
            return modelSet<T>::train(training_set);
        } else {
            //create model(s) here
            modelSet<T>::numInputs = int(training_set[0].input.size());
            for (int i = 0; i < modelSet<T>::numInputs; ++i) {
                modelSet<T>::inputNames.push_back("inputs-" + std::to_string(i + 1));
            }
            modelSet<T>::numOutputs = int(training_set[0].output.size());
            for ( auto example : training_set) {
                if (example.input.size() != modelSet<T>::numInputs) {
                    return false;
                }
                if (example.output.size() != modelSet<T>::numOutputs) {
                    return false;
                }
            }
            std::vector<int> whichInputs;
            for (int j = 0; j < modelSet<T>::numInputs; ++j) {
                whichInputs.push_back(j);
            }
            for (int i = 0; i < modelSet<T>::numOutputs; ++i) {
                modelSet<T>::myModelSet.push_back(new neuralNetwork<T>(modelSet<T>::numInputs, whichInputs, numHiddenLayers, modelSet<T>::numInputs));
            }
            if (numEpochs != 500) {
                for (baseModel<T>* model : modelSet<T>::myModelSet) {
                    neuralNetwork<T>* nnModel = dynamic_cast<neuralNetwork<T>*>(model); //FIXME: I really dislike this design
                    nnModel->setEpochs(numEpochs);
                }
            }
            modelSet<T>::created = true;
            return modelSet<T>::train(training_set);
        }
    }
    return false;
}

//explicit instantiation
template class regression<double>;
template class regression<float>;
