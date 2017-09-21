//
//  fastDTW.cpp
//  RapidAPI
//
//  Created by mzed on 07/09/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//

#include "fastDTW.h"
#include "dtw.h"

fastDTW::fastDTW() {};

fastDTW::~fastDTW() {};

warpInfo fastDTW::fullFastDTW(const std::vector<std::vector<double>> &seriesX, const std::vector<std::vector<double > > &seriesY, int searchRadius){
    
#ifndef EMSCRIPTEN
    if (seriesY.size() > seriesX.size()) {
        return fullFastDTW(seriesY, seriesX, searchRadius); //TODO: I'm not sure why I need this. Also, not sure why it fails with Emscripten.
    }
#endif
    
    dtw dtw;
    searchRadius = (searchRadius < 0) ? 0 : searchRadius;
    int minSeries = searchRadius + 2;
    if (seriesX.size() <= minSeries || seriesY.size() <= minSeries) {
        return dtw.dynamicTimeWarp(seriesX, seriesY);
    }
    
    double resolution = 2.0;//TODO: Just hardcode this?
    std::vector<std::vector<double>> shrunkenX = downsample(seriesX, resolution);
    std::vector<std::vector<double>> shrunkenY = downsample(seriesY, resolution);
    
    //some nice recursion here
    searchWindow window(seriesX, seriesY, shrunkenX, shrunkenY, getWarpPath(shrunkenX, shrunkenY, searchRadius), searchRadius);
    
    return dtw.constrainedDTW(seriesX, seriesY, window);
};

double fastDTW::getCost(const std::vector<std::vector<double>> &seriesX, const std::vector<std::vector<double > > &seriesY, int searchRadius){
    warpInfo info = fullFastDTW(seriesX, seriesY, searchRadius);
    return info.cost;
};

warpPath fastDTW::getWarpPath(const std::vector<std::vector<double>> &seriesX, const std::vector<std::vector<double > > &seriesY, int searchRadius){
    warpInfo info = fullFastDTW(seriesX, seriesY, searchRadius);
    return info.path;
};

std::vector<std::vector<double> > fastDTW::downsample(const std::vector<std::vector<double>> &series, double resolution) {
    std::vector<std::vector<double> > shrunkenSeries;
    for (int i = 0; i < series.size(); ++i) {
        if (i % 2 == 0) {
            shrunkenSeries.push_back(series[i]);
        } else {
            int shrunkIndex = int(i/2);
            for (int j = 0; j < series[i].size(); ++j) {
                shrunkenSeries[shrunkIndex][j] = (shrunkenSeries[shrunkIndex][j] + series[i][j]) / 2;
            }
        }
    }
    //TODO: implement downsampling by resolution
    return shrunkenSeries;
}