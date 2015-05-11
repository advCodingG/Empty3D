#include "ofApp.h"



//-----------------------------------------------------------------------------------------
//
void ofApp::setup()
{
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
    //resolution
    W = 300;
    H = 300;
    
	camera.setNearClip(0.01f);
    //the starting point of the camera
    camera.setPosition( 0, 20 , 180 );
	camera.setMovementMaxSpeed( 0.1f );
   
    //material and light setup
    material.setShininess( 100 );
    material.setSpecularColor(ofColor(40, 100,255,  250));
    
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(40.f/255.f,110/255.f,135.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(.2, .2, .2) );
    
    //initialize points and color to the mesh
    for(int x = 0; x < W; x++){
        for (int y = 0; y< H; y++){
            
            ofPoint vert = ofPoint(x - W/2 , y - H/2, 0);
            mesh.addVertex(vert);
            mesh.addColor( ofColor( 0, 0, 0 ) );
        }
        
        
    }
    
    //initializ small triangles that make up the mesh
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
    float time = ofGetElapsedTimef();

    for (int y=0; y<H; y++) {
        for (int x=0; x<W; x++) {
            int i = x + W * y;			//Vertex index
            ofPoint p = mesh.getVertex( i );

            //the following equation is based on the linear theory of ocean surface waves
            //read more details on http://oceanworld.tamu.edu/resources/ocng_textbook/chapter16/chapter16_01.htm
            
            freq = sqrt(g * k);
            float height = amp * sin(k * x - freq * time * 2);

            // the z position of each mesh vertex are determined by the x position, the height that's caculated by the linear theory of the ocean suface wave, and ofSignedNoise, which returns a number between -1 and 1
            p.z  = (W-x)/7.f * ofSignedNoise(height * .001, y*.007, height);
            
          //  p.z  = (W-x)/15.f * height * ofSignedNoise(height * .001, y*.007 );
                
            mesh.setVertex( i, p );
            
            //The color of vertex changes as the z position goes up and down
            mesh.setColor(i, ofFloatColor( (abs(height) + .5) * 30/255.f,(abs(height) + .5) * 80/255.f,(abs(height) + .5)*100/255.f));
            
//            mesh.setColor( i, ofFloatColor(
//                                ofMap(height, amp, -amp, 60, 100)/255.f,
//                               1- ofMap(height, amp, -amp, 200, 250)/255.f,
//                                ofMap(height, amp, -amp, 200, 250)/255.f
//                                           ));
       
            
        }
    }
    setNormals( mesh );
    
    //the light moves position and changes color
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
 
}

//-----------------------------------------------------------------------------------------
//
void ofApp::draw()
{
	ofBackgroundGradient( ofColor::skyBlue, ofColor(255), OF_GRADIENT_LINEAR);
	
	ofEnableDepthTest();
    
    ofEnableLighting();
    pointLight.enable();
   pointLight2.enable();
   pointLight3.enable();
	camera.begin();
	
    //draw a grid
//    ofSetColor( ofColor(60));
//		ofPushMatrix();
//			ofRotate(90, 0, 0, -1);
//            ofDrawGridPlane(300);
//        ofPopMatrix();
    
    material.begin();
    
    //rotate the plane so that y axis becomes the z axis
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
