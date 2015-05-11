#include "ofApp.h"

#include <iostream>
#include <complex.h>

#include <math.h>


//-----------------------------------------------------------------------------------------
//
void ofApp::setup()
{
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
   // default_random_engine generator;
    //normal_distribution<double> distribution(0,1);
    
	// Give us a starting point for the camera
	camera.setNearClip(0.01f);
	camera.setPosition( 0, 5 , -10 );
    camera.setGlobalPosition(0, 8, 30);
	camera.setMovementMaxSpeed( 0.1f );
    W = 100;
    H = 100;
    
    
    for(int x = 0; x < W; x++){
        for (int y = 0; y< H; y++){
            
            ofPoint vert = ofPoint(x - W/2 , y - H/2, 0);
            mesh.addVertex(vert);
            mesh.addColor( ofColor( 0, 0, 0 ) );
        }
        
        
    }
    
    
    for (int x = 0 ; x < W-1; x ++) {
        for (int y= 0; y < H-1; y++) {
            
            
            
            int index1 = x * W + y;
            int index2 = (x+1) * W + y;
            int index3 = x * W+(y+1);
            int index4 = (x+1) * W + (y + 1);
            
            
            mesh.addTriangle(index1, index2, index3);
            mesh.addTriangle(index3, index2, index4);
        }
        
    }
    
    setNormals(mesh);
    
}

//-----------------------------------------------------------------------------------------
//
void ofApp::update()
{
    float time = ofGetElapsedTimef();	//Get time
    //Change vertices
    
    for (int y=0; y<H; y++) {
        for (int x=0; x<W; x++) {
            int i = x + W * y;			//Vertex index
            ofPoint p = mesh.getVertex( i );


            freq = sqrt(g * k);
            float height = amp * sin(k * x - freq * time);
            p.z  = 4 * ofSignedNoise(x * .005, y * .02, height );
            mesh.setVertex( i, p );
            
            //Change color of vertex
            mesh.setColor( i, ofColor( 43, 100, ofMap(p.z,-4, 4, 170, 255) ));
        }
    }
    setNormals( mesh );
 
}

//-----------------------------------------------------------------------------------------
//
void ofApp::draw()
{
	ofBackgroundGradient( ofColor::skyBlue, ofColor(255), OF_GRADIENT_LINEAR);
	
	ofEnableDepthTest();
	
	camera.begin();
	
//    ofSetColor( ofColor(60));
//		ofPushMatrix();
//			ofRotate(90, 0, 0, -1);
//            ofDrawGridPlane(30);
//        ofPopMatrix();
    
    
    ofPushMatrix();
    ofRotate(90, 1, 0, 0);


        mesh.draw();
    
    ofPopMatrix();
	
	camera.end();

	ofSetColor( ofColor::white );
	ofDisableDepthTest();


	fontSmall.drawStringShadowed(ofToString(ofGetFrameRate(),2), ofGetWidth()-35, ofGetHeight() - 6, ofColor::whiteSmoke, ofColor::black );

}

//-----------------------------------------------------------------------------------------//
//Universal function which sets normals for the triangle mesh
void ofApp::setNormals( ofMesh &mesh ){
    
    //The number of the vertices
    int nV = mesh.getNumVertices();
    
    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;
    
    vector<ofPoint> norm( nV ); //Array for the normals
    
    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t=0; t<nT; t++) {
        
        //Get indices of the triangle t
        int i1 = mesh.getIndex( 3 * t );
        int i2 = mesh.getIndex( 3 * t + 1 );
        int i3 = mesh.getIndex( 3 * t + 2 );
        
        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex( i1 );
        const ofPoint &v2 = mesh.getVertex( i2 );
        const ofPoint &v3 = mesh.getVertex( i3 );
        
        //Compute the triangle's normal
        ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
        
        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }
    
    //Normalize the normal's length
    for (int i=0; i<nV; i++) {
        norm[i].normalize();
    }
    
    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals( norm );
}





void ofApp::keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}
