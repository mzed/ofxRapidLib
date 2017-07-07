//
//  dtw.h
//  RapidAPI
//
//  Created by mzed on 07/06/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#ifndef dtw_h
#define dtw_h

#include <vector>

class dtw {
    
public:
    dtw();
    ~dtw();
    
    void setSeries(std::vector<std::vector<double>> newSeries);
    double run(std::vector<std::vector<double>> inputSeries);
    void reset();
    
private:
    std::vector<std::vector<double>> storedSeries;
    int numFeatures;
    inline double distanceFunction(std::vector<double> seriesX, std::vector<double> seriesY);

};

#endif /* dtw_h */
