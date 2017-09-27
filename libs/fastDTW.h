//
//  fastDTW.h
//  RapidLib
//
//  Created by mzed on 07/09/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#ifndef fastDTW_h
#define fastDTW_h

#include <vector>
#include "warpPath.h"

template<typename T>
class fastDTW {
public:
    fastDTW();
    ~fastDTW();
    
    static T getCost(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY, int searchRadius);
    static warpInfo fullFastDTW(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY, int searchRadius);

    static warpPath getWarpPath(const std::vector<std::vector<T>> &seriesX, const std::vector<std::vector<T > > &seriesY, int searchRadius);
    
private:
    static std::vector<std::vector<T> > downsample(const std::vector<std::vector<T>> &series, T resolution);
    
};


#endif /* fastDTW_h */
