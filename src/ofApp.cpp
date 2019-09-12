#include "ofApp.h"
using namespace ofxCv;
using namespace cv;
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    ofSetBackgroundColor(0, 0, 0);
    
    camWidth = 640;  // try to grab at this size.
    camHeight = 480;
    
    //get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);
    
    vidImg.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
    videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
    videoTexture.allocate(videoInverted);
    
    
    colorImage.allocate(camWidth,camHeight);
    
    grayImage.allocate(camWidth, camHeight);
    grayThreshNear.allocate(camWidth, camHeight);
    grayThreshFar.allocate(camWidth, camHeight);
    
    
    imitate(previous, grayImage);
    imitate(diff, grayImage);
    
    
    
    width = 3840;
    height = 960;


    syphonFbo.allocate(width,height);
    
    syphonServer.setName("texForArena");
    
    words.load("words.jpg");
    
    dataImg.allocate(3840,960,OF_IMAGE_COLOR);
    
    for (int i = 0; i < words.getWidth(); i++) {
        for (int j = 0; j < words.getHeight(); j++) {
            ofColor c;
            ofColor c1 = words.getColor(i,j);
            
            c.r = 0;
            c.g = 0;
            c.b = (c1.r + c1.g + c1.b)/3;
            
            dataImg.setColor(i, j, c);
        }
    }
    
    
    
    gui.setup();

    
    
    gui.add(minAreaRadius.set("min area",1,1,300));
    gui.add(maxAreaRadius.set("max area",10,1,800));
    gui.add(trackingThreshold.set("tracking thresh",1,1,100));

    gui.add(nearThreshold.set("near",230,1,255));
    gui.add(farThreshold.set("far",70,1,255));
    gui.add(bThreshWithOpenCV.set("use opencv", false));
    
    
    if (!ofFile("settings.xml"))
        gui.saveToFile("settings.xml");
    
    gui.loadFromFile("settings.xml");
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    vidGrabber.update();
    
    if(vidGrabber.isFrameNew()){
        
        videoTexture = vidGrabber.getTexture();
        ofPixels & pixels = vidGrabber.getPixels();
        vidImg.setFromPixels(pixels);
        
        colorImage.setFromPixels(pixels);

    }
    grayImage = colorImage;
    
    if(bThreshWithOpenCV) {
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
//        grayThreshNear.threshold(nearThreshold, true);
//        grayThreshFar.threshold(farThreshold);
//        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
    } else {
        
        // or we do it ourselves - show people how they can work with the pixels
//        ofPixels & pix = grayImage.getPixels();
//        int numPixels = pix.size();
//        for(int i = 0; i < numPixels; i++) {
//            if(pix[i] < nearThreshold && pix[i] > farThreshold) {
//                pix[i] = 255;
//            } else {
//                pix[i] = 0;
//            }
//        }
        
    }
    
    // update the cv images
    grayImage.flagImageChanged();
    
    // get diff
    absdiff(grayImage, previous, diff);
    diff.update();
    copy(grayImage, previous);
    blur(diff,10);

    
    grayImage.blur();
    
    contourFinder.setMinAreaRadius(minAreaRadius);
    contourFinder.setMaxAreaRadius(maxAreaRadius);
    contourFinder.setThreshold(trackingThreshold);

    contourFinder.findContours(grayImage);

    
    

    
    for (int i = 0; i < diff.getWidth(); i++) {
        for (int j = 0; j < diff.getHeight(); j++) {
            ofColor c = diff.getColor(i, j);
            
            ofColor c1;
            c1.r = c.r;
            c1.g = 0;
            
            
            c1.b = dataImg.getColor(i*2,j*2).b;
            dataImg.setColor(i*2, 2*j, c1);
            
            c1.b = dataImg.getColor(i*2+1,j*2).b;
            dataImg.setColor(i*2+1, 2*j, c1);
            
            
            c1.b = dataImg.getColor(i*2,j*2+1).b;
            dataImg.setColor(i*2, 2*j+1, c1);
            
            c1.b = dataImg.getColor(i*2+1,j*2+1).b;
            dataImg.setColor(i*2+1, 2*j+1, c1); // dataImg is 3840x960 , need double the diff with height and width
            // 0 -> 0,1
            // 1 -> 2,3
            
        
        }
    }
    dataImg.update();
    
    syphonFbo.begin();
    ofClear(0,0,0);

    dataImg.draw(0, 0,3840,960);
    syphonFbo.end();
    syphonServer.publishTexture(&syphonFbo.getTexture());
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    grayImage.draw(640,0,640,480);
    diff.draw(640, 480);
    
//    vidGrabber.getTexture().draw(0, 0);
    contourFinder.draw();
//    colorImage.draw(0, 480);
    gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
