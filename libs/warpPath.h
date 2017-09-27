//
//  warpPath.h
//  RapidLib
//
//  Created by mzed on 13/09/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#ifndef warpPath_h
#define warpPath_h

#include <vector>

class warpPath {
public:
    warpPath();
    ~warpPath();
    std::vector<int> xIndices;
    std::vector<int> yIndices;
    
    void add(int x, int y);

};

struct warpInfo {
public:
    warpPath path;
    double cost;
    
};
#endif /* warpPath_h */
