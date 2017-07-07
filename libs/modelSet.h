#ifndef modelSet_h
#define modelSet_h

#include <vector>
#include "trainingExample.h"
#include "baseModel.h"
#include "neuralNetwork.h"
#include "knnClassification.h"
#include "svmClassification.h"
#ifndef EMSCRIPTEN
#include "json.h"
#endif

/** This class holds a set of models with the same or different algorithms. */

class modelSet {
public:
    modelSet();
    virtual ~modelSet();
    /** Train on a specified set, causes creation if not created */
    virtual bool train(const std::vector<trainingExample> &trainingSet);
    /** reset to pre-training state */
    bool reset();
    /** run regression or classification for each model */
    std::vector<double> run(const std::vector<double> &inputVector);
    
protected:
    std::vector<baseModel*> myModelSet;
    int numInputs;
    std::vector<std::string> inputNames;
    int numOutputs;
    bool created;

#ifndef EMSCRIPTEN //The javascript code will do its own JSON parsing
public:
    /** Get a JSON representation of the model in the form of a styled string */
    std::string getJSON();
    /** Write a JSON model description to specified file path */
    void writeJSON(const std::string &filepath);
    /** configure empty model with string. See getJSON() */
    bool putJSON(const std::string &jsonMessage);
    /** read a JSON file at file path and build a modelSet from it */
    bool readJSON(const std::string &filepath);
        
private:
    Json::Value parse2json();
    void json2modelSet(const Json::Value &root);

#endif
};

#endif
