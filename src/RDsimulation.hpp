//
//  RDsimulation.hpp
//  reaction-diffusion
//
//  Created by m0Rya on 2018/04/05.
//

#ifndef RDsimulation_hpp
#define RDsimulation_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Voxelizer.hpp"


class RDsimulation{
private:
   
    
    //parameter
    float dA = .6;
    
    float dB = .3;
    double feed = .0545;
    double kill = .062;
    //double feed = .367;
    //double kill = .0649;
    
    float adj = 0.2;
    float dia = 0.05;
    
    
    
    int width = 150;
    int height = 150;
    

    
    vector<ofVec2f> crntAB;
    vector<ofVec2f> nextAB;
    
    ofVec2f *verts;
    ofFloatColor *colors;
    
    
    ofVbo myVbo;
    
    bool simulating = true;
    
    //3d
    Voxelizer vox;
    int numCalc;
    vector<ofVec3f> voxData;
    float voxelHeight = 0.2;
    int skipTime = 10;
    
    vector<vector<bool>> forStl;
    
public:
    RDsimulation();
    ~RDsimulation();
    
    void draw();
    void draw3D_laminated();
    
    void calc();
    void init();
    float laplacian(int i, int j, char flg);
    
    void set3D_laminated();
    
    bool getSimulating();
    
    
    void exportStlFile(string fileName);
    
    string writeWithMesh(ofVec3f nor, int i, int j, int k, int index[6]);
    
   
    
    
    
    
};

#endif /* RDsimulation_hpp */
