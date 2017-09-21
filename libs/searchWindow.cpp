//
//  searchWindow.cpp
//  RapidAPI
//
//  Created by mzed on 14/09/2017.
//  Copyright © 2017 Goldsmiths. All rights reserved.
//


#include "searchWindow.h"

searchWindow::searchWindow(const std::vector<std::vector<double>> &seriesX, const std::vector<std::vector<double>> &seriesY, const std::vector<std::vector<double>> &shrunkenX, const std::vector<std::vector<double>> &shrunkenY, warpPath shrunkenWarpPath, int searchRadius) :
minValues(seriesX.size(), -1), maxValues(seriesX.size(), 0), maxY(int(seriesY.size() - 1)), size(0) {
    
    //Current location of higher resolution path
    int currentX = shrunkenWarpPath.xIndices[0];
    int currentY = shrunkenWarpPath.yIndices[0];
    
    //Last evaluated part of low resolution path
    int lastWarpedX = std::numeric_limits<int>::max();
    int lastWarpedY = std::numeric_limits<int>::max();
    
    //project each part of the low-res path to high res cells
    for (int i = 0; i < shrunkenWarpPath.xIndices.size(); ++i) {
        
        int warpedX = shrunkenWarpPath.xIndices[i];
        int warpedY = shrunkenWarpPath.yIndices[i];
        
        int blockXSize = 2; //TODO: This could be something other than 2?
        int blockYSize = 2;
        
        if (warpedX > lastWarpedX) {
            currentX += 2; //TODO: this should be the block size
        }
        if (warpedY > lastWarpedY) {
            currentY += 2;
        }
        
        if ((warpedX > lastWarpedX) && (warpedY > lastWarpedY))
        {
            markVisited(currentX-1, currentY);
            markVisited(currentX, currentY-1);
        }
        
        for (int j = 0; j < blockXSize; ++j) {
            markVisited(currentX + j, currentY);
            markVisited(currentX + j, currentY + blockYSize - 1); //TODO: These are redundant?
        }
        
        lastWarpedX = warpedX;
        lastWarpedY = warpedY;
    }
    if (searchRadius > 0) {
        expandWindow(1);
        expandWindow(searchRadius-1);
    }
}

void searchWindow::markVisited(int col, int row) {
    if (row <= maxY) { //FIXME: This is kind of a hack. row shouln't be > maxY
        if (minValues[col] == -1) {
            minValues[col] = row;
            maxValues[col] = row;
            size++;
        } else if (minValues[col] > row) {
            size += minValues[col] - row;
            minValues[col] = row;
        } else if (maxValues[col] < row) {
            size += row - maxValues[col];
            maxValues[col] = row;
        }
    }
}

void searchWindow::expandWindow(int radius) {
    if (radius > 0) {
        
        //Add all cells in the current window to a vector.
        std::vector<std::pair<int, int>> windowCells;
        for (int currentX = 0; currentX < minValues.size(); ++currentX) {
            for (int currentY = minValues[currentX]; currentY <= maxValues[currentX]; ++currentY) {
                std::pair<int, int> currentCell = std::make_pair(currentX, currentY);
                windowCells.push_back(currentCell);
            }
        }
        //minX = 0;
        int maxX = int(minValues.size() - 1);
        
        for (int cell = 0; cell < windowCells.size(); ++cell) {
            std::pair<int, int> currentCell = windowCells[cell]; //TODO: is pair necessary? easier to make currentX and currentY?
            
            if (currentCell.first != 0 && currentCell.second != maxY) { //move to upper left if possible
                //expand until edges are met
                int targetX = currentCell.first - radius;
                int targetY = currentCell.second + radius;
                
                if (targetX >= 0 && targetY <= maxY) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = std::max(0 - targetX, targetY - maxY);
                    markVisited(targetX + cellsPastEdge, targetY + cellsPastEdge);
                }
            }
            
            if (currentCell.second != maxY) { //move up if possible
                int targetX = currentCell.first;
                int targetY = currentCell.second + radius;
                if (targetY <= maxY) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = targetY - maxY;
                    markVisited(targetX, targetY - cellsPastEdge);
                }
            }
            
            if (currentCell.first != maxX && currentCell.second != maxY) { //move upper right if possible
                int targetX = currentCell.first + radius;
                int targetY = currentCell.second + radius;
                if (targetX <= maxX && targetY <= maxY) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = std::max(targetX - maxX, targetY - maxY);
                    markVisited(targetX - cellsPastEdge, targetY - cellsPastEdge);
                }
            }
            
            if (currentCell.first != 0) { //move left if possible
                int targetX = currentCell.first - radius;
                int targetY = currentCell.second;
                if (targetX != 0) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = (0 - targetX);
                    markVisited(targetX + cellsPastEdge, targetY);
                }
            }
            
            if (currentCell.first != maxX) { //move right if possible
                int targetX = currentCell.first + radius;
                int targetY = currentCell.second;
                if (targetX <= maxX) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = (targetX - maxX);
                    markVisited(targetX - cellsPastEdge, targetY);
                }
            }
            
            if (currentCell.first != 0 && currentCell.second != 0) { //move to lower left if possible
                int targetX = currentCell.first - radius;
                int targetY = currentCell.second - radius;
                
                if (targetX >= 0 && targetY >= 0) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = std::max(0 - targetX, 0 - targetY);
                    markVisited(targetX + cellsPastEdge, targetY + cellsPastEdge);
                }
            }
            
            if (currentCell.second != 0) { //move down if possible
                int targetX = currentCell.first;
                int targetY = currentCell.second - radius;
                if (targetY >= 0) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = 0 - targetY;
                    markVisited(targetX, targetY + cellsPastEdge);
                }
            }
            
            if (currentCell.first != maxX && currentCell.second != 0) { //move lower right if possible
                int targetX = currentCell.first + radius;
                int targetY = currentCell.second - radius;
                if (targetX <= maxX && targetY >= 0) {
                    markVisited(targetX, targetY);
                } else {
                    int cellsPastEdge = std::max(targetX - maxX, 0 - targetY);
                    markVisited(targetX - cellsPastEdge, targetY + cellsPastEdge);
                }
            }
        }
    }
}

