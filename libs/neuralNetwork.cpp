#include <math.h>
#include <random>
#include <algorithm>
#include <vector>

#include "neuralNetwork.h"
#ifdef EMSCRIPTEN
#include "emscripten/nnEmbindings.h"
#endif

void neuralNetwork::initTrainer() {
    //initialize deltas
    deltaWeights = std::vector<std::vector<std::vector<double> > >(numHiddenLayers, std::vector<std::vector<double> >(numHiddenNodes, std::vector<double>((numInputs + 1), 0)));
    deltaHiddenOutput = std::vector<double>((numHiddenNodes + 1), 0);
}

/*!
 * This is the constructor for a model imported from JSON.
 */

neuralNetwork::neuralNetwork(const int &num_inputs,
                             const std::vector<int> &which_inputs,
                             const int &num_hidden_layers,
                             const int &num_hidden_nodes,
                             const std::vector<double> &_weights,
                             const std::vector<double> &w_hidden_output,
                             const std::vector<double> &in_ranges,
                             const std::vector<double> &in_bases,
                             const double &out_range,
                             const double &out_base
                             )
:
numInputs(num_inputs),
whichInputs(which_inputs),
numHiddenLayers(num_hidden_layers),
numHiddenNodes(num_hidden_nodes),
wHiddenOutput(w_hidden_output),
inRanges(in_ranges),
inBases(in_bases),
outRange(out_range),
outBase(out_base),
learningRate(LEARNING_RATE),
momentum(MOMENTUM),
numEpochs(NUM_EPOCHS),
outputErrorGradient(0)
{
    bool randomize = _weights.size() ? false : true;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-0.5,0.5);
    //winding up a long vector from javascript
    int count = 0;
    for (int i = 0; i < numHiddenLayers; ++i) {
        std::vector<std::vector<double>> layer;
        for (int j = 0; j < numHiddenNodes; ++j){
            std::vector<double> node;
            int numConnections = (i == 0) ? numInputs : numHiddenNodes;
            for(int k = 0; k <= numConnections; ++k){
                if (randomize) {
                    node.push_back(distribution(generator));
                } else {
                    node.push_back( _weights[count]);
                }
                count++;
            }
            layer.push_back(node);
        }
        weights.push_back(layer);
    }
    
    if (randomize) {
        for (int i = 0; i <= numHiddenNodes; ++i) {
            wHiddenOutput.push_back(distribution(generator));
        }
    }
    
    for (int i = 0; i < inRanges.size(); ++i) {
        if (inRanges[i] == 0.) {
            inRanges[i] = 1.0; //Prevent divide by zero later.
        }
    }
    
    //trainer -- do we really need this?
    initTrainer();
}



/*!
 * This is the constructor for a model that needs to be trained.
 */

neuralNetwork::neuralNetwork(const int &num_inputs,
                             const std::vector<int> &which_inputs,
                             const int &num_hidden_layers,
                             const int &num_hidden_nodes
                             )
:
numInputs(num_inputs),
whichInputs(which_inputs),
numHiddenLayers(num_hidden_layers),
numHiddenNodes(num_hidden_nodes),
learningRate(LEARNING_RATE),
momentum(MOMENTUM),
numEpochs(NUM_EPOCHS),
outputErrorGradient(0)
{
    //randomize weights
    reset();
    
    //trainer
    initTrainer();
}

/*!
 * This destructor is not needed.
 */
neuralNetwork::~neuralNetwork() {
}

void neuralNetwork::reset() {
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-0.5,0.5);
    
    weights.clear();
    for (int i = 0; i < numHiddenLayers; ++i) {
        std::vector<std::vector<double>> layer;
        for (int j = 0; j < numHiddenNodes; ++j){
            std::vector<double> node;
            int numConnections = (i == 0) ? numInputs : numHiddenNodes;
            for(int k = 0; k <= numConnections; ++k){
                node.push_back(distribution(generator));
            }
            layer.push_back(node);
        }
        weights.push_back(layer);
    }
    
    wHiddenOutput.clear();
    for (int i = 0; i <= numHiddenNodes; ++i) {
        wHiddenOutput.push_back(distribution(generator));
    }
}

inline double neuralNetwork::getHiddenErrorGradient(int layer, int neuron) {
    double weightedSum = 0;
    if (numHiddenLayers == 1 || layer == 0) {
        double wGradient = wHiddenOutput[neuron] * outputErrorGradient;
        return hiddenNeurons[layer][neuron] * (1 - hiddenNeurons[layer][neuron]) * wGradient;
    }
    if (layer == numHiddenLayers - 1) {
        for (int i = 0; i < numHiddenNodes; ++i) {
            weightedSum += wHiddenOutput[i] * outputErrorGradient;
        }
    } else {
        for (int i = 0; i < numHiddenNodes; ++i) {
            weightedSum += deltaWeights[layer +1][neuron][i] * outputErrorGradient;
        }
    }
    return hiddenNeurons[layer][neuron] * (1 - hiddenNeurons[layer][neuron]) * weightedSum;
}

inline double neuralNetwork::activationFunction(double x) {
    //sigmoid
    if (x < -45) { //from weka, to combat overflow
        x = 0;
    } else if (x > 45) {
        x = 1;
    } else {
        x = 1/(1 + exp(-x));
    }
    return x;
}

int neuralNetwork::getNumInputs() const {
    return numInputs;
}

std::vector<int> neuralNetwork::getWhichInputs() const {
    return whichInputs;
}

int neuralNetwork::getNumHiddenLayers() const {
    return numHiddenLayers;
}

void neuralNetwork::setNumHiddenLayers(int num_hidden_layers) {
    numHiddenLayers = num_hidden_layers;
    reset();
    initTrainer();
}

int neuralNetwork::getNumHiddenNodes() const {
    return numHiddenNodes;
}

void neuralNetwork::setEpochs(const int &epochs) {
    numEpochs = epochs;
}

std::vector<double> neuralNetwork::getWeights() const{
    std::vector<double> flatWeights;
    for (int i = 0; i < weights.size(); ++i) {
        for (int j = 0; j < weights[i].size(); ++j) {
            for (int k = 0; k < weights[i][j].size(); ++k) {
                flatWeights.push_back(weights[i][j][k]);
            }
        }
    }
    return flatWeights;
}

std::vector<double> neuralNetwork::getWHiddenOutput() const {
    return wHiddenOutput;
}

std::vector<double> neuralNetwork::getInRanges() const {
    return inRanges;
}

std::vector<double> neuralNetwork::getInBases() const {
    return inBases;
}

double neuralNetwork::getOutRange() const {
    return outRange;
}

double neuralNetwork::getOutBase() const {
    return outBase;
}

#ifndef EMSCRIPTEN

void neuralNetwork::getJSONDescription(Json::Value &jsonModelDescription) {
    jsonModelDescription["modelType"] = "Neural Network";
    jsonModelDescription["numInputs"] = numInputs;
    jsonModelDescription["whichInputs"] = vector2json(whichInputs);
    jsonModelDescription["numHiddenLayers"] = numHiddenLayers;
    jsonModelDescription["numHiddenNodes"] = numHiddenNodes;
    jsonModelDescription["numHiddenOutputs"] = 1;
    jsonModelDescription["inRanges"] = vector2json(inRanges);
    jsonModelDescription["inBases"] = vector2json(inBases);
    jsonModelDescription["outRange"] = outRange;
    jsonModelDescription["outBase"] = outBase;
    
    //Create Nodes
    Json::Value nodes;
    
    //Output Node
    Json::Value outNode;
    outNode["name"] = "Linear Node 0";
    for (int i = 0; i < numHiddenNodes; ++i) {
        std::string nodeName = "Node " + std::to_string(i + 1);
        outNode[nodeName] = wHiddenOutput[i];
    }
    outNode["Threshold"] = wHiddenOutput[numHiddenNodes];
    nodes.append(outNode);
    
    //Input nodes
    for (int i = 0; i < weights.size(); ++i) { //layers
        for (int j = 0; j < weights[i].size(); ++j) { //hidden nodes
            Json::Value tempNode;
            tempNode["name"] = "Sigmoid Node " + std::to_string((i * numHiddenNodes) + j + 1);
            for (int k = 0; k < weights[i][j].size() - 1; ++ k) { //inputs + threshold aka bias
                std::string connectNode = "Attrib inputs-" + std::to_string(k + 1);
                tempNode[connectNode] = weights[i][j][k];
            }
            tempNode["Threshold"] = weights[i][j][weights[i][j].size() - 1];
            nodes.append(tempNode);
        }
    }
    
    jsonModelDescription["nodes"] = nodes;
}
#endif

double neuralNetwork::run(const std::vector<double> &inputVector) {
    std::vector<double> pattern;
    for (int h = 0; h < numInputs; h++) {
        pattern.push_back(inputVector[whichInputs[h]]);
    }
    //set input layer
    inputNeurons.clear();
    for (int i = 0; i < numInputs; ++i) {
        inputNeurons.push_back((pattern[i] - (inBases[i])) / inRanges[i]);
    }
    inputNeurons.push_back(1);
    
    //calculate hidden layers
    hiddenNeurons.clear();
    for (int i = 0; i < numHiddenLayers; ++i) {
        std::vector<double> layer;
        for (int j=0; j < numHiddenNodes; ++j) {
            layer.push_back(0);
            if (i == 0) { //first hidden layer
                for (int k = 0; k <= numInputs; ++k) {
                    layer[j] += inputNeurons[k] * weights[0][j][k];
                }
            } else {
                for (int k = 0; k <= numHiddenNodes; ++k) {
                    layer[j] += hiddenNeurons[i - 1][k] * weights [i][j][k];
                }
            }
            layer[j] = activationFunction(layer[j]);
        }
        layer.push_back(1); //for bias weight
        hiddenNeurons.push_back(layer);
    }
    
    //calculate output
    outputNeuron = 0;
    for (int k=0; k <= numHiddenNodes; ++k){
        outputNeuron += hiddenNeurons[numHiddenLayers - 1][k] * wHiddenOutput[k];
    }
    //if classifier, outputNeuron = activationFunction(outputNeuron), else...
    outputNeuron = (outputNeuron * outRange) + outBase;
    return outputNeuron;
}

void neuralNetwork::train(const std::vector<trainingExample> &trainingSet) {
    initTrainer();
    //setup maxes and mins
    std::vector<double> inMax = trainingSet[0].input;
    std::vector<double> inMin = trainingSet[0].input;
    double outMin = trainingSet[0].output[0];
    double outMax = trainingSet[0].output[0];
    for (int ti = 1; ti < (int) trainingSet.size(); ++ti) {
        for (int i = 0; i < numInputs; ++i) {
            if (trainingSet[ti].input[i] > inMax[i]) {
                inMax[i] = trainingSet[ti].input[i];
            }
            if (trainingSet[ti].input[i] < inMin[i]) {
                inMin[i] = trainingSet[ti].input[i];
            }
            if (trainingSet[ti].output[0] > outMax) {
                outMax = trainingSet[ti].output[0];
            }
            if (trainingSet[ti].output[0] < outMin) {
                outMin = trainingSet[ti].output[0];
            }
        }
    }
    inRanges.clear();
    inBases.clear();
    for (int i = 0; i < numInputs; ++i) {
        inRanges.push_back((inMax[i] - inMin[i]) * 0.5);
        inBases.push_back((inMax[i] + inMin[i]) * 0.5);
    }
    for (int i = 0; i < inRanges.size(); ++i) {
        if (inRanges[i] == 0.) {
            inRanges[i] = 1.0; //Prevent divide by zero later.
        }
    }
    outRange = (outMax - outMin) * 0.5;
    outBase = (outMax + outMin) * 0.5;
    
    //train
    if (outRange) { //Don't need to do any training if output never changes
        for (int epoch = 0; epoch < numEpochs; ++epoch) {
            //run through every training instance
            for (int ti = 0; ti < (int) trainingSet.size(); ++ti) {
                run(trainingSet[ti].input);
                backpropagate(trainingSet[ti].output[0]);
            }
        }
    }
}

void neuralNetwork::backpropagate(const double &desiredOutput) {
    outputErrorGradient = ((desiredOutput - outBase) / outRange) - ((outputNeuron - outBase)/ outRange); //FIXME: could be tighter -MZ
    
    //correction based on size of last layer. Is this right? -MZ
    double length = 0;
    for (int i = 0; i < numHiddenNodes; ++i) {
        length += hiddenNeurons[numHiddenLayers - 1][i] * hiddenNeurons[numHiddenLayers - 1][i];
    }
    length = (length <= 2.0) ? 1.0 : length;
    
    //deltas between hidden and output
    for (int i = 0; i <= numHiddenNodes; ++i) {
        deltaHiddenOutput[i] = (learningRate * (hiddenNeurons[numHiddenLayers - 1][i]/length) * outputErrorGradient) + (momentum * deltaHiddenOutput[i]);
    }
    
    //deltas between hidden
    if (numHiddenLayers > 1) {
        for (int i = numHiddenLayers - 1; i > 0; --i) {
            int numDeltas = (i == 0) ? numInputs : numHiddenNodes;
            for (int j = 0; j < numHiddenNodes; ++j) {
                double hiddenErrorGradient = getHiddenErrorGradient(i, j);
                for (int k = 0; k <= numDeltas; ++k) {
                    deltaWeights[i][j][k] = (learningRate * hiddenNeurons[i][j] * hiddenErrorGradient) + (momentum * deltaWeights[i][j][k]);
                }
            }
        }
    }
    
    //deltas between hidden[0] and input
    for (int j = 0; j < numHiddenNodes; ++j) {
        double hiddenErrorGradient = getHiddenErrorGradient(0, j); //FIXME: This could be folded into the loop above?
        for (int k = 0; k <= numInputs; ++k) {
            deltaWeights[0][j][k] = (learningRate * inputNeurons[k] * hiddenErrorGradient) + (momentum * deltaWeights[0][j][k]);
        }
    }
    
    updateWeights();
}

void neuralNetwork::updateWeights() {
    //hidden to hidden weights
    for (int i = 0; i < numHiddenLayers; ++i) {
        int numDeltas = (i == 0) ? numInputs : numHiddenNodes;
        for (int j = 0; j < numHiddenNodes; ++j) {
            for (int k = 0; k <= numDeltas; ++k) {
                weights[i][j][k] += deltaWeights[i][j][k];
            }
        }
    }
    //hidden to output weights
    for (int i = 0; i <= numHiddenNodes; ++i) {
        wHiddenOutput[i] += deltaHiddenOutput[i];
    }
}
