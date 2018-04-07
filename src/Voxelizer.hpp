//
//  Voxelizer.hpp
//  reaction-diffusion
//
//  Created by m0Rya on 2018/04/05.
//

#ifndef Voxelizer_hpp
#define Voxelizer_hpp

#include <stdio.h>
#include <ofMain.h>

class Voxelizer{
    
private:
    ofVbo myVbo;
    ofVec3f voxelSize = ofVec3f(0.5, 0.5, 0.5);
    int numVerts = 0;
    
public:
    
    void setVoxelSize(ofVec3f _size);
    void voxelize(vector<ofVec3f> pointCloud);
    void voxelize(vector<ofVec3f> pointCloud, vector<ofFloatColor> colorColoud);
    void draw();
    
};

#endif /* Voxelizer_hpp */
