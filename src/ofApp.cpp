#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofBackground(255);
    ofSetFrameRate(60);
    
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            
            crntAB[i][j] = ofVec2f(1.0, 0);
            newAB[i][j] = ofVec2f(1.0, 0);
        }
    }
    
    
    //initialize
    int boxSize = 5;
    int tmpX = 51;
    int tmpY = 51;
    
    for(int i=tmpX-boxSize; i<tmpX+boxSize; i++){
        for(int j=tmpY-boxSize; j<tmpY+boxSize; j++){
            crntAB[i][j].y = 1.0;
            newAB[i][j].y = 1.0;
        }
    }
    
    
    
    for(int i=0; i<width-2; i++){
        for(int j=0; j<height-2; j++){
            verts[i*(height-2) + j] = ofVec2f(i,j);
            col[i*(height-2) + j] = ofFloatColor(1,1,1);
        }
    }
    
    vbo.setVertexData(verts, 40000, GL_DYNAMIC_DRAW);
    vbo.setColorData(col, 40000, GL_DYNAMIC_DRAW);
    
    
    //3d
    ofSetVerticalSync(true);
    
    light.setPointLight();
    light.setScale(100);
    light.setPosition(-100, 300, -100);
    
    light.setAmbientColor(ofFloatColor(0.5, 0.5, 0.5, 1.0));
    light.setDiffuseColor(ofFloatColor(0.5, 0.5, 1.0));
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
    
   
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0; i<10; i++){
        calcRD();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //ofBackground(255);
    
    if(voxelView == false){
        ofBackground(255);
        ofDisableDepthTest();
        light.disable();
        
        for(int i=1; i<width-1; i++){
            for(int j=1; j<height-1; j++){
                float a = newAB[i][j].x;
                float b = newAB[i][j].y;
                int c = int(floor((a-b) * 255));
                
                //ofSetColor(c, c, c);
                //ofDrawRectangle(i-1,j-1,1,1);
                
                float tmp = ofMap(c, 0, 255, 0, 1.0);
                col[(i-1)*(height-2) + (j-1)] = ofFloatColor(tmp,tmp,tmp);
            }
        }
        
        
        //vbo.updateVertexData(verts, 40000);
        vbo.updateColorData(col, 40000);
        vbo.draw(GL_POINTS, 0, 40000);
        
    }else{
        ofEnableDepthTest();
        light.enable();
        
        ofBackground(50);
        
        cam.begin();
        
        
        for(int i=0; i<voxelData.size(); i++){
            for(int j=0; j<voxelData[i].size(); j++){
                
                if(voxelData[i][j] == true){
                    ofDrawBox((j%(width-2) - (width-2)/2),  i*0.2 - int(voxelData.size()/2)*0.2, (int(j/(width-2)) - (height-2)/2), 1, 0.2, 1);
                }
                
            }
        }
        
        
        //voxelVbo.draw(GL_POINTS, 0, numPoints);
        //voxelVbo.draw(GL_TRIANGLES, 0, numPoints);
        //voxelVbo.draw(GL_QUADS, 0, numPoints);
        
        

        cam.end();
        
        
        
        
        
    }
    
    
    
    
    //cout << ofGetFrameRate() << endl;
}

//--------------------------------------------------------------

void ofApp::calcRD(){
    if(stopRD == true){
        return;
    }
    
    float diff = 0.0;
    vector<bool> tmpVoxel;
    
    for(int i=1; i<width-1; i++){
        for(int j=1; j<height-1; j++){
            
            
            newAB[i][j].x = crntAB[i][j].x + (Da * laplacian(i,j,'a') - crntAB[i][j].x * crntAB[i][j].y * crntAB[i][j].y + feed*(1-crntAB[i][j].x));
            
            newAB[i][j].y = crntAB[i][j].y + (Db * laplacian(i,j,'b') + crntAB[i][j].x * crntAB[i][j].y * crntAB[i][j].y - (k + feed) * crntAB[i][j].y);
            
            
            //crntAB[i][j] = newAB[i][j];
            
            if(newAB[i][j].x < 0) newAB[i][j].x = 0;
            if(newAB[i][j].x > 1) newAB[i][j].x = 1;
            if(newAB[i][j].y < 0) newAB[i][j].y = 0;
            if(newAB[i][j].y > 1) newAB[i][j].y = 1;
            
            
            
            //crntAB[i][j] = newAB[i][j];
            
            float diffA = abs(crntAB[i][j].x - newAB[i][j].x);
            float diffB = abs(crntAB[i][j].y - newAB[i][j].y);
            diff += diffA;
            diff += diffB;
            
            ofVec2f tmp = crntAB[i][j];
            crntAB[i][j] = newAB[i][j];
            newAB[i][j] = tmp;
            
            
            //make voxel
            if(ofGetFrameNum()%10 == 0){
                float a = newAB[i][j].x;
                float b = newAB[i][j].y;
                int c = int(floor((a-b) * 255));
                if(c < 20){
                    tmpVoxel.push_back(true);
                }else{
                    tmpVoxel.push_back(false);
                }
            }
            
            
            
        }
    }
    
    if(ofGetFrameNum() % 10 == 0){
    voxelData.push_back(tmpVoxel);
    cout << voxelData.size() << endl;
    }
    
    //if diff value is under 0.5, it almost done
    //cout << "diff value: " << diff << endl;
    if(diff < 0.5){
        stopRD = true;
    }
    
}
//--------------------------------------------------------------

float ofApp::laplacian(int i, int j, char flg){
    float adj = 0.2;
    float dia = 0.05;
    
    float result = 0;
    
    if(flg == 'a'){
        result = 0.0;
        
        result -= crntAB[i][j].x;
        result += crntAB[i+1][j].x*adj;
        result += crntAB[i-1][j].x*adj;
        result += crntAB[i][j+1].x*adj;
        result += crntAB[i][j-1].x*adj;
        result += crntAB[i-1][j-1].x*dia;
        result += crntAB[i+1][j-1].x*dia;
        result += crntAB[i-1][j+1].x*dia;
        result += crntAB[i+1][j+1].x*dia;
        
        return result;
    }else if(flg == 'b'){
        result = 0.0;
        
        result -= crntAB[i][j].y;
        result += crntAB[i+1][j].y*adj;
        result += crntAB[i-1][j].y*adj;
        result += crntAB[i][j+1].y*adj;
        result += crntAB[i][j-1].y*adj;
        result += crntAB[i-1][j-1].y*dia;
        result += crntAB[i+1][j-1].y*dia;
        result += crntAB[i-1][j+1].y*dia;
        result += crntAB[i+1][j+1].y*dia;
        
        return result;
    }
    
    /*
     for(int a=-1; a<1; a++){
     for(int b=-1; b<1; b++){
     
     //center
     if( a == 0 && b == 0){
     
     if(flg == 'a'){
     result -= crntAB[i][j].x;
     }else if(flg == 'b'){
     result -= crntAB[i][j].y;
     }
     
     //not kad
     }else if( (abs(a) + abs(b)) %2 == 1){
     
     if(flg == 'a'){
     result += crntAB[i+a][j+b].x * adj;
     
     }else if(flg == 'b'){
     result += crntAB[i+a][j+b].y * adj;
     
     }
     
     //kado
     }else{
     if(flg == 'a'){
     result += crntAB[i+a][j+b].x * dia;
     
     }else if(flg == 'b'){
     result += crntAB[i+a][j+b].y * dia;
     
     }
     
     
     
     }
     }
     }
     
     
     return result;
     */
    
    
    
    
    
    
}
//--------------------------------------------------------------


void ofApp::keyPressed(int key){
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == ' '){
        voxelView = !voxelView;
        stopRD = !stopRD;
        
        
        
        
        vector<ofVec3f> tmpPoint;
        ofVec3f *tmpP;
        ofVec3f *normal;
        
        
        for(int i=0; i<voxelData.size(); i++){
            for(int j=0; j<voxelData[i].size(); j++){
                
                if(voxelData[i][j] == true){
                    //ofDrawBox((j%(width-2) - (width-2)/2),  i*0.2 - int(voxelData.size()/2)*0.2, (int(j/(width-2)) - (height-2)/2), 1, 0.2, 1);
                    
                    tmpPoint.push_back(ofVec3f((j%(width-2) - (width-2)/2),  i*0.2 - int(voxelData.size()/2)*0.2, (int(j/(width-2)) - (height-2)/2)));
                }
            }
        }
        
        numPoints = tmpPoint.size()*24;
        tmpP = new ofVec3f[tmpPoint.size()*24];
        normal = new ofVec3f[tmpPoint.size()*24];
        
        for(int i=0; i<tmpPoint.size(); i++){
            tmpP[i*24 + 0] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 1] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 2] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z - 0.5);
            tmpP[i*24 + 3] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z - 0.5);
            
            normal[i*24 + 0] = ofVec3f(-1, 0, 0);
            normal[i*24 + 1] = ofVec3f(-1, 0, 0);
            normal[i*24 + 2] = ofVec3f(-1, 0, 0);
            normal[i*24 + 3] = ofVec3f(-1, 0, 0);
            
            
            tmpP[i*24 + 4] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z - 0.5);
            tmpP[i*24 + 5] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z - 0.5);
            tmpP[i*24 + 6] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z - 0.5);
            tmpP[i*24 + 7] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z - 0.5);
            
            normal[i*24 + 4] = ofVec3f(0, 0, -1);
            normal[i*24 + 5] = ofVec3f(0, 0, -1);
            normal[i*24 + 6] = ofVec3f(0, 0, -1);
            normal[i*24 + 7] = ofVec3f(0, 0, -1);
            
            
            tmpP[i*24 + 8] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z - 0.5);
            tmpP[i*24 + 9] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z - 0.5);
            tmpP[i*24 + 10] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 11] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z + 0.5);
            
            normal[i*24 + 8] = ofVec3f(1, 0, 0);
            normal[i*24 + 9] = ofVec3f(1, 0, 0);
            normal[i*24 + 10] = ofVec3f(1, 0, 0);
            normal[i*24 + 11] = ofVec3f(1, 0, 0);
            
            tmpP[i*24 + 12] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 13] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 14] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 15] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z + 0.5);
            
            normal[i*24 + 12] = ofVec3f(0, 0, 1);
            normal[i*24 + 13] = ofVec3f(0, 0, 1);
            normal[i*24 + 14] = ofVec3f(0, 0, 1);
            normal[i*24 + 15] = ofVec3f(0, 0, 1);
            
            
            tmpP[i*24 + 16] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z - 0.5);
            tmpP[i*24 + 17] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 18] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 19] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y + 0.1, tmpPoint[i].z - 0.5);
            
            normal[i*24 + 16] = ofVec3f(0, 1, 0);
            normal[i*24 + 17] = ofVec3f(0, 1, 0);
            normal[i*24 + 18] = ofVec3f(0, 1, 0);
            normal[i*24 + 19] = ofVec3f(0, 1, 0);
            
            tmpP[i*24 + 20] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z - 0.5);
            tmpP[i*24 + 21] = ofVec3f(tmpPoint[i].x - 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 22] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z + 0.5);
            tmpP[i*24 + 23] = ofVec3f(tmpPoint[i].x + 0.5, tmpPoint[i].y - 0.1, tmpPoint[i].z - 0.5);
            
            normal[i*24 + 20] = ofVec3f(0, -1, 0);
            normal[i*24 + 21] = ofVec3f(0, -1, 0);
            normal[i*24 + 22] = ofVec3f(0, -1, 0);
            normal[i*24 + 23] = ofVec3f(0, -1, 0);
   
            
            
        }
        
        voxelVbo.setVertexData(tmpP, tmpPoint.size()*24, GL_DYNAMIC_DRAW);
        voxelVbo.setNormalData(normal, tmpPoint.size()*24, GL_DYNAMIC_DRAW);
        

        
        delete[] tmpP;
        delete[] normal;
        
        
        
        
        
        
        
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
