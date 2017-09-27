//
//  classification.h
//  RapidLib
//
//  Created by mzed on 26/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//


#include <vector>
#include "classification.h"
#ifdef EMSCRIPTEN
#include "emscripten/classificationEmbindings.h"
#endif

template<typename T>
classification<T>::classification() {
    modelSet<T>::numInputs = 0;
    modelSet<T>::numOutputs = 0;
    modelSet<T>::created = false;
    classificationType = knn; //this is the default algorithm
};

template<typename T>
classification<T>::classification(classificationTypes classification_type) {
    modelSet<T>::numInputs = 0;
    modelSet<T>::numOutputs = 0;
    modelSet<T>::created = false;
    classificationType = classification_type;
};

template<typename T>
classification<T>::classification(const int &num_inputs, const int &num_outputs) { //TODO: this feature isn't really useful
    modelSet<T>::numInputs = num_inputs;
    modelSet<T>::numOutputs = num_outputs;
    modelSet<T>::created = false;
    std::vector<int> whichInputs;
    for (int i = 0; i < modelSet<T>::numInputs; ++i) {
        whichInputs.push_back(i);
    }
    std::vector<trainingExample<T> > trainingSet;
    for (int i = 0; i < modelSet<T>::numOutputs; ++i) {
       modelSet<T>::myModelSet.push_back(new knnClassification<T>(modelSet<T>::numInputs, whichInputs, trainingSet, 1));
    }
    modelSet<T>::created = true;
};

template<typename T>
classification<T>::classification(const std::vector<trainingExample<T> > &trainingSet) {
    modelSet<T>::numInputs = 0;
    modelSet<T>::numOutputs = 0;
    modelSet<T>::created = false;
    train(trainingSet);
};

template<typename T>
bool classification<T>::train(const std::vector<trainingExample<T> > &trainingSet) {
    //TODO: time this process?
    modelSet<T>::myModelSet.clear();
    //create model(s) here
    modelSet<T>::numInputs = int(trainingSet[0].input.size());
    for (int i = 0; i < modelSet<T>::numInputs; ++i) {
        modelSet<T>::inputNames.push_back("inputs-" + std::to_string(i + 1));
    }
    modelSet<T>::numOutputs = int(trainingSet[0].output.size());
    for ( auto example : trainingSet) {
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
        if (classificationType == svm) {
            modelSet<T>::myModelSet.push_back(new svmClassification<T>(modelSet<T>::numInputs));
        } else {
            modelSet<T>::myModelSet.push_back(new knnClassification<T>(modelSet<T>::numInputs, whichInputs, trainingSet, 1));
        }
    }
    modelSet<T>::created = true;
    return modelSet<T>::train(trainingSet);
}

template<typename T>
std::vector<int> classification<T>::getK() {
    std::vector<int> kVector;
    for (baseModel<T>* model : modelSet<T>::myModelSet) {
        knnClassification<T>* kNNModel = dynamic_cast<knnClassification<T>*>(model); //FIXME: I really dislike this design
        kVector.push_back(kNNModel->getK());
    }
    return kVector;
}

template<typename T>
void classification<T>::setK(const int whichModel, const int newK) {
    knnClassification<T>* kNNModel = dynamic_cast<knnClassification<T>*>(modelSet<T>::myModelSet[whichModel]); //FIXME: I really dislike this design
    kNNModel->setK(newK);
}

//explicit instantiation
template class classification<double>;
template class classification<float>;
