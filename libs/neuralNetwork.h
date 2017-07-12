#ifndef neuralNetwork_h
#define neuralNetwork_h
#include <vector>
#include "baseModel.h"

#ifndef EMSCRIPTEN
#include "json.h"
#endif

#define LEARNING_RATE 0.3
#define MOMENTUM 0.2
#define NUM_EPOCHS 500

/*! Class for implementing a Neural Network.
 *
 * This class includes both running and training, and constructors for reading trained models from JSON.
 */
class neuralNetwork : public baseModel {
    
public:
    /** This is the constructor for building a trained model from JSON. */
    neuralNetwork(const int &num_inputs,
                  const std::vector<int> &which_inputs,
                  const int &num_hidden_layers,
                  const int &num_hidden_nodes,
                  const std::vector<double> &weights,
                  const std::vector<double> &wHiddenOutput,
                  const std::vector<double> &inRanges,
                  const std::vector<double> &inBases,
                  const double &outRange,
                  const double &outBase);
    
    /** This constructor creates a neural network that needs to be trained.
     *
     * @param num_inputs is the number of inputs the network will process
     * @param which_inputs is an vector of which values in the input vector are being fed to the network. ex: {0,2,4}
     * @param num_hidden_layer is the number of hidden layers in the network. Must be at least 1.
     * @param num_hidden_nodes is the number of hidden nodes in each hidden layer. Often, this is the same as num_inputs
     *
     * @return A neuralNetwork instance with randomized weights and no normalization values. These will be set or adjusted during training.
     */
    neuralNetwork(const int &num_inputs,
                  const std::vector<int> &which_inputs,
                  const int &num_hidden_layer,
                  const int &num_hidden_nodes);
    
    /** destructor */
    ~neuralNetwork();
    
    /** Generate an output value from a single input vector.
     * @param A standard vector of doubles that feed-forward regression will run on.
     * @return A single double, which is the result of the feed-forward operation
     */
    double run(const std::vector<double> &inputVector);
    
    void reset();
    
    int getNumInputs() const;
    std::vector<int> getWhichInputs() const;
    int getNumHiddenLayers() const;
    int getNumHiddenNodes() const;
    
    std::vector<double> getWeights() const;
    std::vector<double> getWHiddenOutput() const;
    
    std::vector<double> getInRanges() const;
    std::vector<double> getInBases() const;
    double getOutRange() const;
    double getOutBase() const;

#ifndef EMSCRIPTEN
    void getJSONDescription(Json::Value &currentModel);
#endif

    
private:
    /** Parameters that describe the topography of the model */
    int numInputs;
    std::vector<int> whichInputs;
    int numHiddenLayers;
    int numHiddenNodes;
    
    /** Neurons: state is updated on each process(). */
    std::vector<double> inputNeurons;
    std::vector<std::vector<double> > hiddenNeurons;
    double outputNeuron;
    
    /** Weights between layers and nodes are kept here. */
    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<double> wHiddenOutput;
    
    /** Normalization parameters */
    std::vector<double> inRanges;
    std::vector<double> inBases;
    double outRange;
    double outBase;
    
    /** Sigmoid function for activating hidden nodes. */
    inline double activationFunction(double);
    
    /** These pertain to the training, and aren't need to run a trained model */
public:
    /** Train a model using backpropagation.
     *
     * @param The training set is a vector of training examples that contain both a vector of input values and a double specifying desired output.
     *
     */
    void train(const std::vector<trainingExample> &trainingSet);
    
private:
    /** Parameters that influence learning */
    double learningRate;
    double momentum;
    int numEpochs;
    
    /** These deltas are applied to the weights in the network */
    std::vector<std::vector< std::vector<double> > > deltaWeights;
    std::vector<double> deltaHiddenOutput;
    
    /** Parameters and functions for calculating amount of change for each weight */
    std::vector<double> hiddenErrorGradients;
    double outputErrorGradient;
    inline double getHiddenErrorGradient(int layer, int neuron);
    
    void initTrainer();
    
    /** Propagate output error back through the network.
     * @param The desired output of the network is fed into the function, and compared with the actual output
     */
    void backpropagate(const double &desiredOutput);
    
    /** Apply corrections to network weights, based on output error */
    void updateWeights();
};

#endif
