//
//  seriesClassification.cpp
//  RapidAPI
//
//  Created by mzed on 08/06/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#include <vector>
#include "seriesClassification.h"
#ifdef EMSCRIPTEN
#include "emscripten/seriesClassificationEmbindings.h"
#endif

seriesClassification::seriesClassification() {};

seriesClassification::~seriesClassification() {};

bool seriesClassification::addSeries(const std::vector<std::vector<double>> &newSeries) {
    dtw newDTW;
    newDTW.setSeries(newSeries);
    dtwClassifiers.push_back(newDTW);
    return true;
}

bool seriesClassification::addTrainingSet(const std::vector<trainingExample> &trainingSet) {
    std::vector<std::vector<double>> newSeries;
    for (int i = 0; i < trainingSet.size(); ++i) {
        newSeries.push_back(trainingSet[i].input);
    }
    return addSeries(newSeries);
};

bool seriesClassification::train(const std::vector<std::vector<std::vector<double> > > &newSeriesSet) {
    bool trained = true;
    reset();
    for (int i = 0; i < newSeriesSet.size(); ++i) {
        if (!addSeries(newSeriesSet[i])) {
            trained = false;
        };
    }
    return trained;
}

bool seriesClassification::trainTrainingSet(const std::vector<std::vector<trainingExample> > &seriesSet) {
    bool trained = true;
    reset();
    for (int i = 0; i < seriesSet.size(); ++i) {
        if (!addTrainingSet(seriesSet[i])) {
            trained = false;
        };
    }
    return trained;
}

void seriesClassification::reset() {
    dtwClassifiers.clear();
}

int seriesClassification::run(const std::vector<std::vector<double>> &inputSeries) {
    //TODO: check vector sizes and reject bad data
    int closestSeries = 0;
    allCosts.clear();
    double lowestCost = dtwClassifiers[0].run(inputSeries);
    allCosts.push_back(lowestCost);
    for (int i = 1; i < dtwClassifiers.size(); ++i) {
        double currentCost = dtwClassifiers[i].run(inputSeries);
        allCosts.push_back(currentCost);
        if (currentCost < lowestCost) {
            lowestCost = currentCost;
            closestSeries = i;
        }
    }
    return closestSeries;
};

int seriesClassification::runTrainingSet(const std::vector<trainingExample> &trainingSet) {
    std::vector<std::vector<double>> newSeries;
    for (int i = 0; i < trainingSet.size(); ++i) {
        newSeries.push_back(trainingSet[i].input);
    }
    return run(newSeries);
};

std::vector<double> seriesClassification::getCosts() {
    return allCosts;
}

std::vector<double> seriesClassification::getCosts(const std::vector<trainingExample> &trainingSet) {
    runTrainingSet(trainingSet);
    return allCosts;
}