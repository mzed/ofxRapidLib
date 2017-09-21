//
//  searchWindow.h
//  RapidAPI
//
//  Created by mzed on 14/09/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#ifndef searchWindow_h
#define searchWindow_h

#include <vector>
#include "warpPath.h"

class searchWindow {
public:
    searchWindow(const std::vector<std::vector<double>> &seriesX,
                 const std::vector<std::vector<double>> &seriesY,
                 const std::vector<std::vector<double>> &shrunkenX,
                 const std::vector<std::vector<double>> &shrunkenY,
                 warpPath shrunkenWarpPath,
                 int searchRadius);
    
    std::vector<int> minValues;
    std::vector<int> maxValues;

private:
    int maxY;
    int size;
    void markVisited(int col, int row);
    void expandWindow(int searchRadius);
};
    
#endif /* searchWindow_h */
