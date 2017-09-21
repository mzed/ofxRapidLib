//
//  warpPath.cpp
//  RapidAPI
//
//  Created by mzed on 13/09/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#include "warpPath.h"

warpPath::warpPath() {};
warpPath::~warpPath() {};

void warpPath::add(int x, int y) {
        xIndices.insert(xIndices.begin(), x);
        yIndices.insert(yIndices.begin(), y);
};
