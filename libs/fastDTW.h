//
//  fastDTW.h
//  RapidAPI
//
//  Created by mzed on 07/09/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#ifndef fastDTW_h
#define fastDTW_h

#include <vector>
#include "warpPath.h"

class fastDTW {
public:
    fastDTW();
    ~fastDTW();
    
    double getCost(const std::vector<std::vector<double>> &seriesX, const std::vector<std::vector<double > > &seriesY, int searchRadius);
    warpInfo fullFastDTW(const std::vector<std::vector<double>> &seriesX, const std::vector<std::vector<double > > &seriesY, int searchRadius);

    warpPath getWarpPath(const std::vector<std::vector<double>> &seriesX, const std::vector<std::vector<double > > &seriesY, int searchRadius);
    
private:
    std::vector<std::vector<double> > downsample(const std::vector<std::vector<double>> &series, double resolution);
    
};


#endif /* fastDTW_h */
