//
//  trainingExample.h
//  RapidLib
//
//  Created by Michael Zbyszynski on 22/11/2017.
//  Copyright © 2016 Goldsmiths. All rights reserved.
//

#ifndef trainingExample_h
#define trainingExample_h

#include <vector>
#include <string>

/** This is used by both NN and KNN models for training */
template<typename T>
struct trainingExample {
  std::vector<T> input;
  std::vector<T> output;
};

/** This is used by DTW models for training */
template<typename T>
struct trainingSeries {
    std::vector<std::vector<T> > input;
    std::string label;
};

#endif
