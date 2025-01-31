/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) 
{
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource, vector<TileImage>& theTiles)
{
    int rows = theSource.getRows();
    int cols = theSource.getColumns();
    
    MosaicCanvas* canvas = new MosaicCanvas(rows, cols);
    
    // Step 2: Create a vector of Point<3> from the average colors of the TileImages
    vector<Point<3>> tilePoints;
    map<Point<3>, TileImage*> tileMap;
    
    for (TileImage& tile : theTiles) {
        LUVAPixel avgColor = tile.getAverageColor();
        Point<3> point = convertToXYZ(avgColor);
        tilePoints.push_back(point);
        tileMap[point] = &tile;  // Map the Point to the TileImage for easy lookup
    }
    
    // Step 3: Build a KDTree using the tilePoints
    KDTree<3> kdTree(tilePoints);
    
    // Step 4: For each region in the SourceImage, find the best matching TileImage
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Get the average color of the current region in the SourceImage
            LUVAPixel regionAvgColor = theSource.getRegionColor(i, j);
            Point<3> queryPoint = convertToXYZ(regionAvgColor);
            
            // Use the KDTree to find the nearest neighbor (best matching tile)
            Point<3> nearestPoint = kdTree.findNearestNeighbor(queryPoint);
            
            // Get the TileImage corresponding to the nearestPoint
            TileImage* bestTile = tileMap[nearestPoint];
            
            // Step 5: Place the TileImage in the MosaicCanvas at the correct position
            canvas->setTile(i, j, bestTile);
        }
    }
    
    // Step 6: Return the MosaicCanvas pointer
    return canvas;
}

