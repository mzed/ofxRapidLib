#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "modelSet.h"

#ifndef EMSCRIPTEN
#include "json.h"
#else
#include "emscripten/modelSetEmbindings.h"
#endif

/** No arguments, don't create any models yet */
modelSet::modelSet() :
numInputs(0),
numOutputs(0),
created(false)
{
};

modelSet::~modelSet() {
    for (std::vector<baseModel*>::iterator i = myModelSet.begin(); i != myModelSet.end(); ++i) {
        delete *i;
    }
};

bool modelSet::train(const std::vector<trainingExample> &training_set) {
    for (trainingExample example : training_set) {
        if (example.input.size() != numInputs) {
            return false;
        }
        if (example.output.size() != numOutputs) {
            return false;
        }
    }
    for (int i = 0; i < myModelSet.size(); ++i) {
        std::vector<trainingExample> modelTrainingSet; //just one output
        for (trainingExample example : training_set) {
            std::vector<double> tempDouble;
            for (int j = 0; j < numInputs; ++j) {
                tempDouble.push_back(example.input[j]);
            }
            trainingExample tempObj = {tempDouble, std::vector<double> {example.output[i]}};
            modelTrainingSet.push_back(tempObj);
        }
        myModelSet[i]->train(modelTrainingSet);
    }
    created = true;
    return true;
}

bool modelSet::reset() {
    for (std::vector<baseModel*>::iterator i = myModelSet.begin(); i != myModelSet.end(); ++i) {
        delete *i;
    }
    myModelSet.clear();
    numInputs = 0;
    numOutputs = 0;
    created = false;
    return true;
}

std::vector<double> modelSet::run(const std::vector<double> &inputVector) {
    std::vector<double> returnVector;
    if (created && inputVector.size() == numInputs) {
        for (auto model : myModelSet) {
            returnVector.push_back(model->run(inputVector));
        }
    } else {
        returnVector.push_back(0);
    }
    return returnVector;
}



#ifndef EMSCRIPTEN
//In emscripten, we do the JSON parsing with native JavaScript
std::vector<double> json2vector(Json::Value json) {
    std::vector<double> returnVec;
    for (unsigned int i = 0; i < json.size(); ++i) {
        returnVec.push_back(json[i].asDouble());
    }
    return returnVec;
}

Json::Value modelSet::parse2json() {
    Json::Value root;
    Json::Value metadata;
    Json::Value modelSet;
    
    metadata["creator"] = "Rapid API C++";
    metadata["version"] = "v0.1.1"; //TODO: This should be a macro someplace
    metadata["numInputs"] = numInputs;
    Json::Value inputNamesJSON;
    for (int i = 0; i < inputNames.size(); ++i) {
        inputNamesJSON.append(inputNames[i]);
    }
    metadata["inputNames"] = inputNamesJSON;
    metadata["numOutputs"] = numOutputs;
    root["metadata"] = metadata;
    for (auto model : myModelSet) {
        Json::Value currentModel;
        currentModel["inputNames"] = inputNamesJSON; //TODO: implment this feature
        model->getJSONDescription(currentModel);
        modelSet.append(currentModel);
    }
    root["modelSet"] = modelSet;
    return root;
}

std::string modelSet::getJSON() {
    Json::Value root = parse2json();
    return root.toStyledString();
}

void modelSet::writeJSON(const std::string &filepath) {
    Json::Value root = parse2json();
    std::ofstream jsonOut;
    jsonOut.open (filepath);
    Json::StyledStreamWriter writer;
    writer.write(jsonOut, root);
    jsonOut.close();
    
}

bool modelSet::putJSON(const std::string &jsonMessage) {
    Json::Value parsedFromString;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonMessage, parsedFromString);
    if (parsingSuccessful)
    {
        json2modelSet(parsedFromString);
    }
    return parsingSuccessful;
}

void modelSet::json2modelSet(const Json::Value &root) {
    numInputs = root["metadata"]["numInputs"].asInt();
    for (unsigned int i = 0; i < root["metadata"]["inputNames"].size(); ++i) {
        inputNames.push_back(root["metadata"]["inputNames"][i].asString());
    }
    numOutputs = root["metadata"]["numOutputs"].asInt();
    
    for (const Json::Value& model : root["modelSet"]) {
        int modelNumInputs = model["numInputs"].asInt();
        std::vector<int> whichInputs;
        std::vector<std::string> modelInputNames;
        for (unsigned int i = 0; i < model["inputNames"].size(); ++i) {
            modelInputNames.push_back(model["inputNames"][i].asString());
        }
        for (int i = 0; i < inputNames.size(); ++i) {
            if (std::find(modelInputNames.begin(), modelInputNames.end(), inputNames[i]) != modelInputNames.end())
            {
                whichInputs.push_back(i);
            }
        }
        if (model["modelType"].asString() == "Neural Network") {
            int numHiddenLayers = model["numHiddenLayers"].asInt();
            int numHiddenNodes = model["numHiddenNodes"].asInt();
            std::vector<double> weights;            std::vector<double> wHiddenOutput;
            int nodeIndex = 0;
            for (const Json::Value& node : model["nodes"]) {
                if (node["name"].asString() == "Linear Node 0") {
                    for (int i = 1; i <= numHiddenNodes; ++i) {
                        std::string whichNode = "Node " + std::to_string(i + (numHiddenNodes * (numHiddenLayers - 1)));
                        wHiddenOutput.push_back(node[whichNode].asDouble());
                    }
                    wHiddenOutput.push_back(node["Threshold"].asDouble());
                } else { //FIXME: this will break if nodes are out of order
		  int currentLayer = (int) floor((nodeIndex - 1.0)/ (double)numHiddenNodes);
                    if (currentLayer < 1) { //Nodes connected to input
                        for (int i = 0; i < numInputs; ++i) {
                            std::string whichNode = "Attrib " + model["inputNames"][i].asString();
                            weights.push_back(node[whichNode].asDouble());
                        }
                    } else { //Hidden Layers
                        for (int i = 0; i < numHiddenNodes; ++i) {
                            std::string whichNode = "Node " + std::to_string(i + (numHiddenNodes * (currentLayer - 1)));
                            weights.push_back(node[whichNode].asDouble());
                        }                    }
                    weights.push_back(node["Threshold"].asDouble());
                }
                nodeIndex++;
            }
            std::vector<double> inBases = json2vector(model["inBases"]);
            std::vector<double> inRanges = json2vector(model["inRanges"]);
            double outRange = model["outRange"].asDouble();
            double outBase = model["outBase"].asDouble();
            
            myModelSet.push_back(new neuralNetwork(modelNumInputs, whichInputs, numHiddenLayers, numHiddenNodes, weights, wHiddenOutput, inRanges, inBases, outRange, outBase));
        } else if (model["modelType"].asString() == "kNN Classificiation") {
            std::vector<trainingExample> trainingSet;
            const Json::Value examples = model["examples"];
            for (unsigned int i = 0; i < examples.size(); ++i) {
                trainingExample tempExample;
                tempExample.input = json2vector(examples[i]["features"]);
                tempExample.output.push_back(examples[i]["class"].asDouble());
                trainingSet.push_back(tempExample);
            }
            int k = model["k"].asInt();
            myModelSet.push_back(new knnClassification(modelNumInputs, whichInputs, trainingSet, k));
        }
    }
    created = true;
}

bool modelSet::readJSON(const std::string &filepath) {
    Json::Value root;
    std::ifstream file(filepath);
    file >> root;
    json2modelSet(root);
    return created; //TODO: check something first
}
#endif
