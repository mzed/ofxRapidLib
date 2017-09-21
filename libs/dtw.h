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
#include "warpPath.h"
#include "searchWindow.h"

class dtw {
public:
    dtw();
    ~dtw();
    
    /* Calculates and returns a simple cost value between two input series */
    double getCost(const std::vector<std::vector<double>> &seriesX, const std::vector<std::vector<double > > &seriesY);
    
    /* Calculates both cost and the warp path */
    warpInfo dynamicTimeWarp(const std::vector<std::vector<double> > &seriesX, const std::vector<std::vector<double> > &seriesY); //This returns everything, including a path
    
    /* Calculates both the cost and the warp path, with a given window as a constraint */
    warpInfo constrainedDTW(const std::vector<std::vector<double> > &seriesX, const std::vector<std::vector<double> > &seriesY, searchWindow window); //This takes a window object
    
    /* Returns the warp path that was found by one of the above methods. */
    warpPath getPath();
    
private:
    inline double distanceFunction(const std::vector<double> &pointX, const std::vector<double> &point);
    std::vector<std::vector<double> > costMatrix;
    warpPath warpPath;
    double cost;
    
    void calculatePath(int seriesXsize, int seriesYsize);
};

#endif /* dtw_h */
