#ifndef trainingExample_h
#define trainingExample_h

#include <vector>

/** This is used by both NN and KNN models for training */
struct trainingExample {
  std::vector<double> input;
  std::vector<double> output;
};

#endif
