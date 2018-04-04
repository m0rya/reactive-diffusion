#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    void calcRD();
    float laplacian(int i, int j, char flg);
    
    
    /*
    int size = 10;
    int res = 250;
    
    int startNums = 5;
    float minRad = 0.4;
    float maxRad = 0.6;
     */
    
    
    const int width = 102;
    const int height = 102;
    
    int numStartB = 5;
    
    /*
    float Da = 1.0;
    float Db = 0.5;
     */
    
    float Da= 0.5;
    float Db = 0.25;
    
    //double feed = 0.055;
    //double k = 0.062;
    
    //double feed = .0367;
    //double k = .0649;
    
    double feed = .0545;
    double k = .062;
    
    ofPixels pixels;
    ofImage image;
    
    

    
    ofVec2f crntAB[202][202];
    ofVec2f newAB[202][202];
    
    ofFloatColor col[40000];
    ofVec2f verts[40000];
    
    
    
    ofVbo vbo;
    
    
 
    //3d
    bool stopRD = false;
    
    ofEasyCam cam;
    ofLight light;
    bool voxelView = false;
    
    vector<vector<bool>> voxelData;
    
    ofVbo voxelVbo;
    int numPoints = 0;
    
    

};
