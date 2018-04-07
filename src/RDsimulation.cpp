//
//  RDsimulation.cpp
//  reaction-diffusion
//
//  Created by m0Rya on 2018/04/05.
//

#include "RDsimulation.hpp"

RDsimulation::RDsimulation(){
    
    vox.setVoxelSize(ofVec3f(1.0, voxelHeight, 1.0));
    
}


RDsimulation::~RDsimulation(){
    //delete[] verts;
    delete[] colors;
}


void RDsimulation::init(){
    
    numCalc = 0;
    simulating = true;
    
    crntAB.clear();
    nextAB.clear();
    
    crntAB.resize((width+2) * (height+2));
    nextAB.resize((width+2) * (height+2));
    
    for(int i=0; i<width+2; i++){
        for(int j=0; j<height+2; j++){
            
            crntAB[i*(height+2) + j] = ofVec2f(1.0, 0);
            nextAB[i*(height+2) + j] = ofVec2f(1.0, 0);
            
        }
    }
    
    
    
    //first b virus
    
    ofVec2f pos = ofVec2f((width+2)/2, (height+2)/2);
    int boxSize = 2;
    
    for(int i=pos.x-boxSize; i<pos.x+boxSize; i++){
        for(int j=pos.y-boxSize; j<pos.y+boxSize; j++){
            crntAB[i*(height+2) + j].y = 1.0;
            nextAB[i*(height+2) + j].y = 1.0;
        }
    }
     
    
    /*
    for(int i=10; i<width-10; i++){
        
        crntAB[i*(height+2) + height/2].y = 1.0;
        nextAB[i*(height+2) + height/2].y = 1.0;
    
    }
    */
    
    
    pos = ofVec2f((width+2)*2/5, (height+2)*2/5);
    
    for(int i=pos.x-boxSize; i<pos.x+boxSize; i++){
        for(int j=pos.y-boxSize; j<pos.y+boxSize; j++){
            crntAB[i*(height+2) + j].y = 1.0;
            nextAB[i*(height+2) + j].y = 1.0;
        }
    }
    
    pos = ofVec2f((width+2)*3/5, (height+2)*2/5);
    
    for(int i=pos.x-boxSize; i<pos.x+boxSize; i++){
        for(int j=pos.y-boxSize; j<pos.y+boxSize; j++){
            crntAB[i*(height+2) + j].y = 1.0;
            nextAB[i*(height+2) + j].y = 1.0;
        }
    }
    
    pos = ofVec2f((width+2)*2/5, (height+2)*3/5);
    
    for(int i=pos.x-boxSize; i<pos.x+boxSize; i++){
        for(int j=pos.y-boxSize; j<pos.y+boxSize; j++){
            crntAB[i*(height+2) + j].y = 1.0;
            nextAB[i*(height+2) + j].y = 1.0;
        }
    }
    
    pos = ofVec2f((width+2)*3/5, (height+2)*3/5);
    
    for(int i=pos.x-boxSize; i<pos.x+boxSize; i++){
        for(int j=pos.y-boxSize; j<pos.y+boxSize; j++){
            crntAB[i*(height+2) + j].y = 1.0;
            nextAB[i*(height+2) + j].y = 1.0;
        }
    }
    
     
    
    
    //init end
    
    
    verts = new ofVec2f[width*height];
    colors = new ofFloatColor[width*height];
    
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            verts[i*height + j] = ofVec2f(i,j);
            colors[i*height + j] = ofFloatColor(1., 1., 1.);
        }
    }
    
    myVbo.setVertexData(verts, width*height, GL_DYNAMIC_DRAW);
    myVbo.setColorData(colors, width*height, GL_DYNAMIC_DRAW);
    
    delete[] verts;
    //delete[] colors;
    
}


void RDsimulation::calc(){
    float diff = 0.0;
    
    if(simulating){
        //vector<bool> tmpVoxel;
        vector<bool> preForStl;
        
        for(int i=1; i<width+1; i++){
            for(int j=1; j<height+1; j++){
                
                float crntA = crntAB[i*(height+2) + j].x;
                float crntB = crntAB[i*(height+2) + j].y;
                
                if(crntA == 0 && crntB == 0 && laplacian(i,j,'a') == 0 && laplacian(i,j,'b') ==0){
                    continue;
                }
                
                nextAB[i*(height+2) + j].x = crntA + (dA * laplacian(i,j,'a') - crntA*crntB*crntB + feed*(1.0-crntA));
                
                nextAB[i*(height+2) + j].y = crntB + (dB * laplacian(i,j,'b') + crntA*crntB*crntB - (kill + feed) * crntB);
                
                
                if(nextAB[i*(height+2) +j].x < 0) nextAB[i*(height+2) +j].x = 0;
                if(nextAB[i*(height+2) +j].x > 1.0) nextAB[i*(height+2) +j].x = 1.0;
                
                if(nextAB[i*(height+2) +j].y < 0) nextAB[i*(height+2) +j].y = 0;
                if(nextAB[i*(height+2) +j].y > 1.0) nextAB[i*(height+2) +j].y = 1.0;
                
                diff += abs(crntAB[i*(height+2)+j].x - nextAB[i*(height+2)+j].x);
                diff += abs(crntAB[i*(height+2)+j].y - nextAB[i*(height+2)+j].y);
                
                //ofVec2f tmp = crntAB[i*(height+2)+j];
                //crntAB[i*(height+2)+j] = nextAB[i*(height+2)+j];
                //nextAB[i*(height+2)+j] = tmp;
                crntAB[i*(height+2)+j] = nextAB[i*(height+2)+j];
                
                
                
                
                
                if(ofGetFrameNum()%skipTime == 0){
                    float a = nextAB[i*(height+2) + j].x;
                    float b = nextAB[i*(height+2) + j].y;
                    int c = int(floor((a-b)*255));
                    
                    if(c < 20){
                        voxData.push_back(ofVec3f(i-width/2, numCalc*voxelHeight, j-height/2));
                        preForStl.push_back(true);
                    }else{
                        preForStl.push_back(false);
                    }
                    
                    
                }
                
                
            }
        }
        if(ofGetFrameNum()%skipTime == 0){
            numCalc++;
            forStl.push_back(preForStl);
        }
        
       
        /*
        if(diff < 0.3){
            simulating = false;
            cout << "simulation end\n" ;
        }
         */
        
       
        
    }
    
    //cout << voxData.size() << endl;
    
    
}


float RDsimulation::laplacian(int i, int j, char flg){
    
    float result;
    
    if(flg == 'a'){
        result = 0.0;
        
        result -= crntAB[i*(height+2) + j].x;
        result += crntAB[(i+1)*(height+2) + j].x * adj;
        result += crntAB[(i-1)*(height+2) + j].x * adj;
        result += crntAB[i*(height+2) + j+1].x * adj;
        result += crntAB[i*(height+2) + j-1].x * adj;
        result += crntAB[(i-1)*(height+2) + j-1].x * dia;
        result += crntAB[(i+1)*(height+2) + j-1].x * dia;
        result += crntAB[(i-1)*(height+2) + j+1].x * dia;
        result += crntAB[(i+1)*(height+2) + j+1].x * dia;
        
        return result;
        
    }else if(flg == 'b'){
        result = 0.0;
        
        result -= crntAB[i*(height+2) + j].y;
        result += crntAB[(i+1)*(height+2) + j].y * adj;
        result += crntAB[(i-1)*(height+2) + j].y * adj;
        result += crntAB[i*(height+2) + j+1].y * adj;
        result += crntAB[i*(height+2) + j-1].y * adj;
        result += crntAB[(i-1)*(height+2) + j-1].y * dia;
        result += crntAB[(i+1)*(height+2) + j-1].y * dia;
        result += crntAB[(i-1)*(height+2) + j+1].y * dia;
        result += crntAB[(i+1)*(height+2) + j+1].y * dia;
        
        return result;
        
    }else{
        return 0.0;
        
    }
    
    
}

void RDsimulation::set3D_laminated(){
    vox.voxelize(voxData);
    
}


void RDsimulation::draw(){
    
    for(int i=1; i<width+1; i++){
        for(int j=1; j<height+1; j++){
            float a = nextAB[i*(height+2) + j].x;
            float b = nextAB[i*(height+2) + j].y;
            int c = int(floor((a-b)*255));
            
            float tmp = ofMap(c, 0, 255, 0, 1.0);
            colors[(i-1)*height + j-1] = ofFloatColor(tmp, tmp, tmp);
        }
    }
    
    myVbo.updateColorData(colors, width*height);
    glDisable(GL_LIGHTING);
    myVbo.draw(GL_POINTS, 0, width*height);
    
}

void RDsimulation::draw3D_laminated(){
    
    vox.draw();
}

bool RDsimulation::getSimulating(){
    return simulating;
}

void RDsimulation::exportStlFile(string fileName){
    string stlAscii = "solid rd\n";
    
    

    
    cout << forStl.size() << endl;
    
    

    
    for(int i=0; i<forStl.size(); i++){
        
        for(int j=0; j<width; j++){
            for(int k=0; k<height; k++){
                
                bool crnt = forStl[i][j*height + k];
                
                if(crnt == true){
                    
                    
                    
                    
                    if(k == 0){
                        int index[6] = {0, 1, 2, 0, 2, 3};
                        
                        stlAscii += writeWithMesh(ofVec3f(-1.0, 0, 0), i, j, k, index);
                        
                        
                        
                    }else if(!forStl[i][j*height + k - 1]){
                        
                        int index[6] = {0, 1, 2, 0, 2, 3};
                        
                        stlAscii += writeWithMesh(ofVec3f(-1.0, 0, 0), i, j, k, index);
                        
                        
                    }
                    
                    
                    if(k == height-1){
                        int index[6] = {4,6,5,4,7,6};
                        
                        stlAscii += writeWithMesh(ofVec3f(1.0, 0, 0), i, j, k, index);
                        
                    }else if(!forStl[i][j*height + k+1]){
                        
                        int index[6] = {4,6,5,4,7,6};
                        
                        stlAscii += writeWithMesh(ofVec3f(1.0, 0, 0), i, j, k, index);
                        
                    }
                    
                    
                    
                    if(j == 0){
                        
                        int index[6] = {1,5,2,2,5,6};
                        
                        stlAscii +=  writeWithMesh(ofVec3f(0, -1.0, 0), i, j, k, index);
                    
                    }else if(!forStl[i][(j-1)*height + k]){
                        
                        int index[6] = {1,5,2,2,5,6};
                        
                        stlAscii += writeWithMesh(ofVec3f(0, -1.0, 0), i, j, k, index);
                        
                    }
                    
                    if(j == width-1){
                        int index[6] = {0,3,4,4,3,7};
                        
                        stlAscii += writeWithMesh(ofVec3f(0, 1.0, 0), i, j, k, index);
                        
                    }else if(!forStl[i][(j+1)*height + k]){
                        
                        int index[6] = {0,3,4,4,3,7};
                        
                        stlAscii +=  writeWithMesh(ofVec3f(0, 1.0, 0), i, j, k, index);
                        
                    }
                    
                    
                    if(i == 0){
                        int index[6] = {1,0,5,5,0,4};
                        
                        stlAscii += writeWithMesh(ofVec3f(0, 0, -1.0), i, j, k, index);
                        
                    }else if(!forStl[i-1][j*height + k]){
                        int index[6] = {1,0,5,5,0,4};
                        
                        stlAscii += writeWithMesh(ofVec3f(0, 0, -1.0), i, j, k, index);
                        
                    }
                    
                    if(i == forStl.size()-1){
                        int index[6] = {3,2,6,3,6,7};
                        
                        stlAscii += writeWithMesh(ofVec3f(0, 0, 1.0), i, j, k, index);
                        
                    }else if(!forStl[i+1][j*height + k]){
                        
                        int index[6] = {3,2,6,3,6,7};
                        
                        stlAscii +=  writeWithMesh(ofVec3f(0, 0, 1.0), i, j, k, index);
                        
                    }
                     
                    
                    
                    
                    
                }
            }
        }
    }
    
    
    stlAscii += "endsolid rd\n";

    
    
    ofBuffer buf(stlAscii.c_str(), stlAscii.size());
    ofBufferToFile("rd4.stl", buf);
    
    
    
    
    
    
    cout << "stl file is exported\n";
}

string RDsimulation::writeWithMesh(ofVec3f nor, int i, int j, int k, int index[6]){
    string result = "";
    
    //i:z, j:w, k:h
    //i *= 0.2;
    
    
    
    float w = 0.5;
    float h = 0.5;
    float d = voxelHeight/2;
    
    ofVec3f tmp[8];
    tmp[0] = ofVec3f(k-h, j+w, i*voxelHeight-d);
    tmp[1] = ofVec3f(k-h, j-w, i*voxelHeight-d);
    tmp[2] = ofVec3f(k-h, j-w, i*voxelHeight+d);
    tmp[3] = ofVec3f(k-h, j+w, i*voxelHeight+d);
    
    tmp[4] = ofVec3f(k+h, j+w, i*voxelHeight-d);
    tmp[5] = ofVec3f(k+h, j-w, i*voxelHeight-d);
    tmp[6] = ofVec3f(k+h, j-w, i*voxelHeight+d);
    tmp[7] = ofVec3f(k+h, j+w, i*voxelHeight+d);
    
    
    result = "facet normal " + ofToString(nor.x) + " " + ofToString(nor.y) + " " + ofToString(nor.z) + "\nouter loop\n";
    for(int f=0; f<3; f++){
        result += "vertex " + ofToString(tmp[index[f]].x) + " " + ofToString(tmp[index[f]].y) + " " + ofToString(tmp[index[f]].z) + "\n";
    }
    
    result += "endloop\nendfacet\n";
    
    result += "facet normal " + ofToString(nor.x) + " " + ofToString(nor.y) + " " + ofToString(nor.z) + "\nouter loop\n";
    for(int f=3; f<6; f++){
        result += "vertex " + ofToString(tmp[index[f]].x) + " " + ofToString(tmp[index[f]].y) + " " + ofToString(tmp[index[f]].z) + "\n";
    }
    result += "endloop\nendfacet\n";
    
    
    return result;
    
}
