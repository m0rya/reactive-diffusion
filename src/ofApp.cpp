#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofBackground(255);
    ofSetFrameRate(60);
    
    
    
    //3d
    ofSetVerticalSync(true);
    
    light.setPointLight();
    light.setScale(100);
    light.setPosition(-100, 300, -100);
    
    light.setAmbientColor(ofFloatColor(0.5, 0.5, 0.5, 1.0));
    light.setDiffuseColor(ofFloatColor(0.5, 0.5, 1.0));
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
    
    rd.init();
    
    string hoge = "hello world\n";
    
    //cout << ofToHex(hoge) << endl;
    
    const char *converted = ofToHex(hoge).c_str();
    
    cout << converted << endl;
    
    
    
   
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    for(int i=0; i<10; i++){
        rd.calc();
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //ofBackground(255);
    
    if(voxelView == false){
        ofBackground(255);
        ofDisableDepthTest();
        light.disable();
   
        
        rd.draw();
        
    }else{
        ofEnableDepthTest();
        light.enable();
        
        ofBackground(50);
        
        cam.begin();
        
        
        /*
        for(int i=0; i<voxelData.size(); i++){
            for(int j=0; j<voxelData[i].size(); j++){
                
                if(voxelData[i][j] == true){
                    ofDrawBox((j%(width-2) - (width-2)/2),  i*0.2 - int(voxelData.size()/2)*0.2, (int(j/(width-2)) - (height-2)/2), 1, 0.2, 1);
                }
                
            }
        }
         */
        
        
        //voxelVbo.draw(GL_POINTS, 0, numPoints);
        //voxelVbo.draw(GL_TRIANGLES, 0, numPoints);
        //voxelVbo.draw(GL_QUADS, 0, numPoints);
        rd.draw3D_laminated();
        
        

        cam.end();
        
        
        
        
        
    }
    
    
    
    
    //cout << ofGetFrameRate() << endl;
}

//--------------------------------------------------------------



void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == ' '){
        voxelView = !voxelView;
        
        if(voxelView){
            rd.set3D_laminated();
            //rd.exportStlFile("rd.stl");
        }
        //stopRD = !stopRD;
        
        
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
