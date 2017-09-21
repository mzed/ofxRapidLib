//
//  seriesClassification.h
//  RapidAPI
//
//  Created by Michael Zbyszynski on 08/06/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#ifndef seriesClassification_hpp
#define seriesClassification_hpp

#include <vector>
#include <string>
#include "fastDTW.h"
#include "trainingExample.h"

class seriesClassification {
    
public:
    seriesClassification();
    ~seriesClassification();
    
    bool train(const std::vector<trainingSeries> &seriesSet);
    void reset();
    std::string run(const std::vector<std::vector<double>> &inputSeries);
    std::vector<double> getCosts();
    
private:
    std::vector<trainingSeries> allTrainingSeries;
    std::vector<double> allCosts;
    
};

#endif
