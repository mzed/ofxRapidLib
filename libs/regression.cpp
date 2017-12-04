/**
 * @file regression.cpp
 * RapidLib
 *
 * @author Michael Zbsyzynski
 * @date 26 Sep 2016
 * @copyright Copyright © 2016 Goldsmiths. All rights reserved.
 */

#include <vector>
#include <iostream>
#include <time.h>
#include "regression.h"


#ifdef EMSCRIPTEN
#include "emscripten/regressionEmbindings.h"
#endif

template<typename T>
regressionTemplate<T>::regressionTemplate() {
    modelSet<T>::numInputs = -1;
    modelSet<T>::numOutputs = -1;
    numHiddenLayers = 1;
    numEpochs = 500;
    modelSet<T>::created = false;
};

template<typename T>
regressionTemplate<T>::regressionTemplate(const int &num_inputs, const int &num_outputs) {
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
regressionTemplate<T>::regressionTemplate(const std::vector<trainingExampleTemplate<T> > &training_set) {
    modelSet<T>::numInputs = -1;
    modelSet<T>::numOutputs = -1;
    modelSet<T>::created = false;
    train(training_set);
};

template<typename T>
std::vector<int> regressionTemplate<T>::getNumHiddenLayers() {
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
void regressionTemplate<T>::setNumHiddenLayers(const int &num_hidden_layers){
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
void regressionTemplate<T>::setNumEpochs(const int &epochs) {
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
bool regressionTemplate<T>::train(const std::vector<trainingExampleTemplate<T> > &training_set) {
    clock_t timer;
    timer = clock();
    modelSet<T>::reset();
    if (training_set.size() > 0) {
        //create model(s) here
        modelSet<T>::numInputs = int(training_set[0].input.size());
        modelSet<T>::numOutputs = int(training_set[0].output.size());
        for (int i = 0; i < modelSet<T>::numInputs; ++i) {
            modelSet<T>::inputNames.push_back("inputs-" + std::to_string(i + 1));
        }
        modelSet<T>::numOutputs = int(training_set[0].output.size());
        for ( auto example : training_set) {
            if (example.input.size() != modelSet<T>::numInputs) {
                throw std::length_error("unequal feature vectors in input.");
                return false;
            }
            if (example.output.size() != modelSet<T>::numOutputs) {
                throw std::length_error("unequal output vectors.");
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
        timer = clock() - timer;
        std::cout << "Regression trained in " << (float)timer/CLOCKS_PER_SEC << " ms." << std::endl;
        return modelSet<T>::train(training_set);
    }
    throw std::length_error("empty training set.");
    return false;
}

//explicit instantiation
template class regressionTemplate<double>;
template class regressionTemplate<float>;
