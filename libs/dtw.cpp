//
//  dtw.cpp
//  RapidAPI
//
//  Created by mzed on 07/06/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#include <vector>
#include <cmath>
#include "dtw.h"

dtw::dtw() {};

dtw::~dtw() {};

void dtw::setSeries(std::vector<std::vector<double>> newSeries) {
    storedSeries = newSeries;
    numFeatures = int(storedSeries[0].size());
};

inline double dtw::distanceFunction(std::vector<double> x, std::vector<double> y) {
    //TODO: make sure series are same length
    double euclidianDistance = 0;
    for(int j = 0; j < numFeatures ; ++j){
        euclidianDistance = euclidianDistance + pow((x[j] - y[j]), 2);
    }
    euclidianDistance = sqrt(euclidianDistance);
    return euclidianDistance;
};

double dtw::run(std::vector<std::vector<double> > inputSeries) {
    std::vector<std::vector<double> > costMatrix(inputSeries.size(), std::vector<double>(storedSeries.size(), 0));
    int maxInput = int(inputSeries.size()) - 1;
    int maxStored = int(storedSeries.size()) - 1;
    
    //Calculate values for the first column
    costMatrix[0][0] = distanceFunction(inputSeries[0], storedSeries[0]);
    for (int j = 1; j <= maxStored; ++j) {
        costMatrix[0][j] = costMatrix[0][j - 1] + distanceFunction(inputSeries[0], storedSeries[j]);
    }
    
    for (int i = 1; i <= maxInput; ++i) {
        //Bottom row of current column
        costMatrix[i][0] = costMatrix[i - 1][0] + distanceFunction(inputSeries[i], storedSeries[0]);
        for (int j = 1; j <= maxStored; ++j) {
            double minGlobalCost = fmin(costMatrix[i-1][j-1], costMatrix[i][j-1]);
            costMatrix[i][j] = minGlobalCost + distanceFunction(inputSeries[i], storedSeries[j]);
        }
    }
    double minimumCost = costMatrix[maxInput][maxStored];
    return minimumCost;
};