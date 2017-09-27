//
//  baseModel.h
//  RapidLib
//
//  Created by mzed on 05/09/2016.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//


#ifndef baseModel_h
#define baseModel_h

#include <vector>
#include "trainingExample.h"

#ifndef EMSCRIPTEN
#include "json.h"
#endif

/** Base class for wekinator models. Implemented by NN and KNN classes */
template<typename T>
class baseModel {
public:
    virtual ~baseModel() {};
    virtual T run(const std::vector<T> &inputVector) = 0;
    virtual void train(const std::vector<trainingExample<T> > &trainingSet) = 0;
    virtual void reset() = 0;;
    virtual int getNumInputs() const = 0;
    virtual std::vector<int> getWhichInputs() const = 0;
    
#ifndef EMSCRIPTEN
    virtual void getJSONDescription(Json::Value &currentModel) = 0;
    
protected:
    template<typename TT>
    Json::Value vector2json(TT vec) {
        Json::Value toReturn;
        for (int i = 0; i < vec.size(); ++i) {
            toReturn.append(vec[i]);
        }
        return toReturn;
    }
#endif
    
};
#endif
