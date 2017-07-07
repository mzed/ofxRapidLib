//
//  seriesClassification.hp
//  RapidAPI
//
//  Created by mzed on 08/06/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#ifndef seriesClassification_hpp
#define seriesClassification_hpp

#include <vector>
#include "dtw.h"
#include "trainingExample.h"

class seriesClassification {
    
public:
    seriesClassification();
    ~seriesClassification();
    
    bool addSeries(const std::vector<std::vector<double>> &newSeries);
    bool addTrainingSet(const std::vector<trainingExample> &trainingSet);
    
    bool train(const std::vector<std::vector<std::vector<double>>> &newSeriesSet);
    bool trainTrainingSet(const std::vector<std::vector<trainingExample>> &seriesSet);
    
    void reset();
    
    int run(const std::vector<std::vector<double>> &inputSeries);
    int runTrainingSet(const std::vector<trainingExample> &inputSet);
    
    std::vector<double> getCosts();
    std::vector<double> getCosts(const std::vector<trainingExample> &inputSet);
    
private:
    std::vector<dtw> dtwClassifiers;
    std::vector<double> allCosts;
    
};

#endif
