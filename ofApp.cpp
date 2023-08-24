
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
// 
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>


#include "ofApp.h"
#include "Util.h"

//--------------------------------------------------------------
// setup the lander force
//
void ofApp::landerSetup(){
    if (lander.loadModel("geo/ship.obj")) {
        lander.setScaleNormalization(false);
        //lander.setPosition(ofRandom(0, 150), 50, ofRandom(-50, 50));
        lander.setPosition(25, 50, -25);
        bLanderLoaded = true;
    }
    else cout << "Error: Can't load model:" << " geo/monster.obj" << endl;
    
    // Player particle for movement
    gravity = new GravityForce(ofVec3f(0, -3.721, 0));
    behavior.sys->addForce(gravity);
    //turbForce = new TurbulenceForce(ofVec3f(-7, 0, -7), ofVec3f(7, 0, 7));
    //behavior.sys->addForce(turbForce);
    behavior.setVelocity(ofVec3f(0, 0, 0));
    behavior.setOneShot(true);
    behavior.setEmitterType(DirectionalEmitter);
    behavior.setLifespan(-1);
    fuel = 120.0;
    begin = ofGetElapsedTimef();
    altitude = lander.getPosition().y;
        
    // Exhaust emitter
    exhaustForce = new TurbulenceForce(ofVec3f(-1, 0, -1), ofVec3f(1, 0, 1));
    exhaust.sys->addForce(exhaustForce);
    exhaust.setEmitterType(RadialEmitter);
    exhaust.setVelocity(ofVec3f(0, 10, 0));
    exhaust.setGroupSize(10);
    exhaust.setParticleRadius(.3);
    exhaust.setRandomLife(true);
    exhaust.setLifespanRange(ofVec2f(.03, .2));
    exhaust.setRate(1);
    exhaust.setOneShot(true);
    
    // Explosion Emitter
    explosionForce = new ImpulseRadialForce(2000.0);
    explosionForce->setHeight(1.0);
    explosion.sys->addForce(explosionForce);
    explosion.setEmitterType(SphereEmitter);
    explosion.setOneShot(true);
    explosion.setVelocity(ofVec3f(0, 0, 0));
    explosion.setGroupSize(1000);
    //explosion.setRate(1);
    explosion.setLifespan(0.08);
    explosion.setParticleRadius(0.2);
}

//--------------------------------------------------------------
// load images and model
//
void ofApp::loadfile(){
    font1.load("verdana.ttf", 50, true, true);
    font1.setLineHeight(550.0f);
    font1.setLetterSpacing(1.037);
    
    font2.load("verdana.ttf", 25, true, true);
    font2.setLineHeight(25.0f);
    font2.setLetterSpacing(1.035);
    
    //load backgroung images and 3D model
    background.load("images/Background.png");
    //mars.loadModel("geo/mars-low-5x-v2.obj");
    mars.loadModel("geo/moon-houdini.obj");
    mars.setScaleNormalization(false);
    
    // texture loading
    ofDisableArbTex();     // disable rectangular textures
    
    // load textures
    if (!ofLoadImage(particleTex, "images/dot.png")) {
        cout << "Particle Texture File not found" << endl;
        ofExit();
    }
    if (!ofLoadImage(expl, "images/fire.png")) {
        cout << "Particle Texture File not found" << endl;
        ofExit();
    }

    // load the shader
    #ifdef TARGET_OPENGLES
        shader.load("shaders_gles/shader");
    #else
        shader.load("shaders/shader");
    #endif
}

//--------------------------------------------------------------
// setup lighting
//
void ofApp::lightSetup(){
    // setup rudimentary lighting
    initLightingAndMaterials();
    
    areaLight1.setup();
    areaLight1.enable();
    areaLight1.setAreaLight(100, 100);
    areaLight1.setAmbientColor(ofFloatColor(.06, .06, .06));
    areaLight1.setDiffuseColor(ofFloatColor(.6, .6, .6));
    areaLight1.setSpecularColor(ofFloatColor(.6, .6, .6));
    areaLight1.setPosition(0, 135, 0);

    areaLight2.setup();
    areaLight2.enable();
    areaLight2.setAreaLight(100, 100);
    areaLight2.setAmbientColor(ofFloatColor(.03, .03, .03));
    areaLight2.setDiffuseColor(ofFloatColor(0.3, 0.3, 0.3));
    areaLight2.setSpecularColor(ofFloatColor(0.3, 0.3, 0.3));
    areaLight2.setPosition(0, 80, 0);

    landerLight.setup();
    //landerLight.enable();
    landerLight.setSpotlight();
    landerLight.setScale(.05);
    landerLight.setSpotlightCutOff(15);
    landerLight.setAttenuation(2, .01, .01);
    landerLight.setAmbientColor(ofColor::lightYellow);
    landerLight.setDiffuseColor(ofColor::lightSkyBlue);
    landerLight.setSpecularColor(ofColor::orangeRed);
    landerLight.rotate(-90, ofVec3f(1, 0, 0));
    landerLight.setPosition(lander.getPosition().x, lander.getPosition().y + 6, lander.getPosition().z);

    landing.setup();
    landing.enable();
    landing.setSpotlight();
    landing.setScale(.05);
    landing.setSpotlightCutOff(45);
    landing.setAttenuation(1, .01, .01);
    landing.setAmbientColor(ofColor::greenYellow);
    landing.setDiffuseColor(ofColor::green);
    landing.setSpecularColor(ofColor::gold);
    landing.rotate(-90, ofVec3f(1, 0, 0));
    landing.setPosition(5, 55, 5);
}

//--------------------------------------------------------------
// camera setup
//
void ofApp::cameraSetup(){
    theCam = &cam;
    cam.setDistance(50);
    cam.setNearClip(.1);
    cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
    cam.setPosition(20, 30, -20);
    cam.lookAt(lander.getPosition());
    cam.disableMouseInput();
    
    trackCam.setNearClip(0.1);
    trackCam.setFov(65.5);
        
    landerCam.setNearClip(0.1);
    landerCam.setFov(65.5);
    
    ofEnableSmoothing();
    ofEnableDepthTest();
}

//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    //setup camera
    cameraSetup();
    
    //setup light
    lightSetup();
    
    //load file
    loadfile();
    
    //setup lander
    landerSetup();
    
    // Sounds effect
    explosionSound.load("sounds/explosion.mp3");
    explosionSound.setLoop(true);
    explosionSound.setVolume(0.4);
        
    backGroundSound.load("sounds/background.mp3");
    backGroundSound.setLoop(true);
    backGroundSound.setVolume(0.6);

    thrustSound.load("sounds/thrust.wav");
    thrustSound.setVolume(0.4);

	// create sliders for testing
	//
	gui.setup();
	gui.add(numLevels.setup("Number of Octree Levels", 5, 1, 10));
    //gui.add(TimingInfo.setup("Timing Info", false));
	bHide = false;
    gameStart = false;
    gameEnd = false;

	//  Create Octree for testing.
	//
    mesh = mars.getMesh(0);
    float begin = ofGetElapsedTimeMillis();
	octree.create(mesh, 20);
    float end = ofGetElapsedTimeMillis();
    
    buildTime = end - begin;
    //testFile << "Build the tree time: " << buildTime << " ms" << endl;
	//cout << "Number of Verts: " << mars.getMesh(0).getNumVertices() << endl;

	testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));
    
}
 
//--------------------------------------------------------------
// physics simulation for the lander
//
void ofApp::landerMove(){
    float framerate = ofGetFrameRate();
    float dt = 1.0 / framerate;
    
    // angular motion
    rotation = behavior.sys->particles[0].rotation;
    lander.setRotation(0, rotation, 0, 1, 0);
    if (moveLeft || moveRight || moveForward || moveBackward || thrust || rotateLeft || rotateRight) {
        begin = ofGetElapsedTimef() - begin;
        fuel -= begin;
        begin = ofGetElapsedTimef();
    }
    if(!gameStart) fuel = 120.0;
    if(fuel < 0) fuel = 0;
    if (fuel > 0 && !gameEnd) {
        if (moveLeft) {
            ofVec3f move = 7 * side();
            behavior.sys->particles.at(0).forces += move;
        }
        if (moveRight) {
            ofVec3f move = -7 * side();
            behavior.sys->particles.at(0).forces += move;
        }
        if (moveForward) {
            ofVec3f move = 7 * heading();
            behavior.sys->particles.at(0).forces += move;
        }
        if (moveBackward) {
            ofVec3f move = -7 * heading();
            behavior.sys->particles.at(0).forces += move;
        }
        if (rotateLeft) {
            behavior.sys->particles[0].angularForces = 15;
        }
        if (rotateRight) {
            behavior.sys->particles[0].angularForces = -15;
        }
        if (thrust) {
            behavior.sys->particles.at(0).forces.y += 10;
            exhaust.sys->reset();
            exhaust.start();
        }
        else {
            exhaust.stop();
        }
    }
}
//--------------------------------------------------------------
// check for colision between lander box and terrain
//
void ofApp::collision(){
    ofVec3f min = lander.getSceneMin() + lander.getPosition();
    ofVec3f max = lander.getSceneMax() + lander.getPosition();

    landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

    colBoxList.clear();
    normalList.clear();
    octree.intersect(landerBounds, octree.root, colBoxList, normalList);
    if(colBoxList.size() > 0) col = true;
    else col = false;
    
    if(col) {
        for (int i = 0; i < normalList.size(); i++) {
            normalVec += mesh.getNormal(normalList.at(i));
        }
        ofVec3f gra = ofVec3f(0, -3.721, 0);
        ofVec3f vel = behavior.sys->particles.at(0).velocity + gra;
        normalVec.normalize();
        impulse = (1 + .85) * (-vel.dot(normalVec) * normalVec);
        behavior.sys->particles.at(0).forces += impulse/2;
        gravity->set(ofVec3f(0, 0, 0));
        bound = impulse.length();
        if(bound > 13){
            crash = true;
            explosionSound.play();
            gameEnd = true;
        }
        else {
            //landingStatus = true;
            gameEnd = true;
        }
    }
    else gravity->set(ofVec3f(0, -3.721, 0));
    
}
//--------------------------------------------------------------
// update lander behavior
//
void ofApp::updateLander(){
    behavior.setPosition(ofVec3f(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z));
    behavior.start();
    behavior.update();
    
    ofVec3f lander_Pos = behavior.sys->particles.at(0).position;
    exhaust.setPosition(ofVec3f(lander.getPosition().x, lander.getPosition().y - 1, lander.getPosition().z));
    exhaust.update();
    
    explosion.setPosition(lander_Pos);
    explosion.update();
    lander.setPosition(lander_Pos.x, lander_Pos.y, lander_Pos.z);
}
//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
    if(gameStart){
        //upadate lander attribute
        updateLander();
        //lander behavior with physic
        landerMove();
        //check for colision
        collision();
        //update altitude
        landerBot = ofVec3f(lander.getPosition().x, lander.getPosition().y - 3, lander.getPosition().z);
        if(turnOnAGL) {
            ofVec3f point;
            detectAltitude(point);
            altitude = landerBot.distance(point);
        }
        
        //cameras
        trackCam.setPosition(glm::vec3(lander.getPosition().x + 25, lander.getPosition().y + 15, lander.getPosition().z));
        trackCam.lookAt(lander.getPosition());
        
        //ofVec3f cameradir = lander.getPosition() + heading();
        landerCam.setPosition(glm::vec3(lander.getPosition().x, lander.getPosition().y - 2, lander.getPosition().z));
        landerCam.lookAt(glm::vec3(lander.getPosition().x, lander.getPosition().y - 5, lander.getPosition().z));
        
        if (bPointSelected) {
            cam.lookAt(selectedPoint);
        }
    }
    
    if(gameEnd) {
        backGroundSound.stop();
        landingDistacne = landerBot.distance(ofVec3f(0, 0, 0));
        if(crash) {
            landingType = 4;
            explosion.sys->reset();
            explosion.start();
        }
        else if(landingDistacne >= 50) landingType = 3;
        else if(landingDistacne > 10) landingType = 2;
        else landingType = 1;
    }
    
    // Adjust lander light position
    landerLight.setPosition(lander.getPosition().x, lander.getPosition().y + 6, lander.getPosition().z);
    if(lightOn) landerLight.enable();
    else landerLight.disable();
}

// load vertex buffer in preparation for rendering
//
void ofApp::loadVbo() {
    if (exhaust.sys->particles.size() < 1) return;

    vector<ofVec3f> sizes;
    vector<ofVec3f> points;
    for (int i = 0; i < exhaust.sys->particles.size(); i++) {
        points.push_back(exhaust.sys->particles[i].position);
        sizes.push_back(ofVec3f(10));
    }
    
    // upload the data to the vbo
    //
    int total = (int)points.size();
    vbo.clear();
    vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}
void ofApp::loadEx() {
    if (explosion.sys->particles.size() < 1) return;

    vector<ofVec3f> exSize;
    vector<ofVec3f> exPoint;
    if (explosion.sys->particles.size() < 1) return;
    for (int i = 0; i < explosion.sys->particles.size(); i++) {
        exPoint.push_back(explosion.sys->particles[i].position);
        exSize.push_back(ofVec3f(10));
    }
    
    // upload the data to the vbo
    //
    int extotal = (int)exPoint.size();
    vbo.clear();
    vbo.setVertexData(&exPoint[0], extotal, GL_STATIC_DRAW);
    vbo.setNormalData(&exSize[0], extotal, GL_STATIC_DRAW);
}

void ofApp::drawShader(){
//    loadVbo();
    glDepthMask(GL_FALSE);
    ofSetColor(255, 100, 90);

    // this makes everything look glowy :)
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();


    // begin drawing in the camera
    shader.begin();
    theCam->begin();

    loadVbo();
    loadEx();
    // draw particle emitter here..
    //exhaust.draw();
    //explosion.draw();
    particleTex.bind();
    vbo.draw(GL_POINTS, 0, (int)exhaust.sys->particles.size());
    particleTex.unbind();
    
    expl.bind();
    vbo.draw(GL_POINTS, 0, (int)explosion.sys->particles.size());
    expl.unbind();

    //  end drawing in the camera
    theCam->end();
    shader.end();

    ofDisablePointSprites();
    ofDisableBlendMode();
    ofEnableAlphaBlending();
    
    glDepthMask(GL_TRUE);
}

//--------------------------------------------------------------
void ofApp::draw() {

	glDepthMask(false);
    ofSetColor(ofColor::white);
    background.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	glDepthMask(true);

    theCam->begin();
	ofPushMatrix();
    if(showHeading) {
        ofSetColor(ofColor::red);
        ofDrawLine(lander.getPosition(), lander.getPosition() + heading() * 10);
    }
    if(showSide) {
        ofSetColor(ofColor::green);
        ofDrawLine(lander.getPosition(), lander.getPosition() + side() * 10);
    }
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
		if (bLanderLoaded) {
			lander.drawWireframe();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
		}
		//if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();
		ofMesh mesh;
		if (bLanderLoaded) {
			lander.drawFaces();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
			if (bDisplayBBoxes) {
				ofNoFill();
				ofSetColor(ofColor::white);
				for (int i = 0; i < lander.getNumMeshes(); i++) {
					ofPushMatrix();
					ofMultMatrix(lander.getModelMatrix());
					ofRotate(-90, 1, 0, 0);
					Octree::drawBox(bboxList[i]);
					ofPopMatrix();
				}
			}

			if (bLanderSelected) {

				ofVec3f min = lander.getSceneMin() + lander.getPosition();
				ofVec3f max = lander.getSceneMax() + lander.getPosition();

				Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
				ofSetColor(ofColor::white);
                ofNoFill();
				Octree::drawBox(bounds);

				// draw colliding boxes
				//
				ofSetColor(ofColor::yellow);
				for (int i = 0; i < colBoxList.size(); i++) {
					Octree::drawBox(colBoxList[i]);
				}
			}
		}
	}
	//if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    

	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
	}
    
    // highlight selected point (draw sphere around selected point)
    //
    if (bPointSelected) {
        ofSetColor(ofColor::blue);
        ofDrawSphere(selectedPoint, .1);
    }

	// recursively draw octree
	//
	ofDisableLighting();
    if(bDisplayOctree){
        int level = 0;
        ofNoFill();
        octree.draw(octree.root, numLevels, level);
    }
    
	if (bDisplayLeafNodes) {
        ofNoFill();
        ofSetColor(ofColor::aquamarine);
		octree.drawLeafNodes(octree.root);
		//cout << "num leaf: " << octree.numLeaf << endl;
    }
	else if (bDisplayOctree) {
		ofNoFill();
		ofSetColor(ofColor::white);
		octree.draw(numLevels, 0);
	}

	// if point selected, draw a sphere
	//
//	if (pointSelected) {
//		ofVec3f p = octree.mesh.getVertex(selectedNode.points[0]);
//		ofVec3f d = p - cam.getPosition();
//		ofSetColor(ofColor::lightGreen);
//		ofDrawSphere(p, .02 * d.length());
//	}
    
    if(drawColl){
        ofSetColor(ofColor::white);
        ofNoFill();
        Octree::drawBox(landerBounds);

        // draw colliding boxes
        //
        ofSetColor(ofColor::yellow);
        for (int i = 0; i < colBoxList.size(); i++) {
            Octree::drawBox(colBoxList[i]);
        }
        if(col) {
            ofSetColor(ofColor::red);
            ofDrawLine(lander.getPosition(), lander.getPosition() + normalVec * 10);
        }
    }
    
    //exhaust.draw();
    //explosion.draw();

	ofPopMatrix();
    theCam->end();
    
    //draw the rocket exhaust
    drawShader();
    
    string str;
    str += "Frame Rate: " + std::to_string(ofGetFrameRate());
    ofSetColor(ofColor::white);
    ofDrawBitmapString(str, ofGetWindowWidth() -170, 15);
    
    if (turnOnAGL) {
        string str1;
        str1 += "Altitude: " + std::to_string(altitude);
        ofSetColor(ofColor::white);
        ofDrawBitmapString(str1, ofGetWindowWidth() -170, 30);
    }
    if(gameStart){
        string str2;
        str2 += "Fuel: " + std::to_string(fuel);
        ofSetColor(ofColor::white);
        ofDrawBitmapString(str2, ofGetWindowWidth() -370, 15);
    }
    
    glDepthMask(false);
    //if (!bHide) gui.draw();
    if(showControl){
        ofDrawBitmapString("1: Easy Cam", 15, 15);
        ofDrawBitmapString("2: Cam to lander", 15, 35);
        ofDrawBitmapString("3: Cam from Lander", 15, 55);
        ofDrawBitmapString("A: Turn on AGL", 15, 75);
        ofDrawBitmapString("B: Shown collision box and normal vector", 15, 95);
        ofDrawBitmapString("H: Show heading and side vector", 15, 115);
        ofDrawBitmapString("L: Turn on lander light", 15, 135);
        ofDrawBitmapString("W: Thrust", 15, 155);
        ofDrawBitmapString("Q: Rotate Left", 15, 175);
        ofDrawBitmapString("E: Rotate right", 15, 195);
        ofDrawBitmapString("Up key: Move Forward", 15, 215);
        ofDrawBitmapString("Down key: Move Back", 15, 235);
        ofDrawBitmapString("Left key: Move Left", 15, 255);
        ofDrawBitmapString("Right key: Move right", 15, 275);
        ofDrawBitmapString("Press x hide Control Map", 15, 295);
    }
    else ofDrawBitmapString("Press x to see Control Map", 15, 15);
    
    if (!gameStart) {
        ofSetColor(ofColor::orangeRed);
        font1.drawString("Lander Game", ofGetWidth() / 2 - 220, ofGetHeight() / 2 - 100);
        ofSetColor(ofColor::white);
        font2.drawString("Press Space To Start", ofGetWidth() / 2 - 160, ofGetHeight() / 2 - 50);
    }

    if (gameEnd) {
        ofSetColor(ofColor::orangeRed);
        font1.drawString("Game Over", ofGetWidth() / 2 - 175, ofGetHeight() / 2 - 150);
        ofSetColor(ofColor::whiteSmoke);
        if(landingType == 1) font2.drawString("Perfect landing", ofGetWidth() / 2 - 120, ofGetHeight() / 2 - 50);
        if(landingType == 2) font2.drawString("Try to get closer to the landing lot", ofGetWidth() / 2 - 260, ofGetHeight() / 2 - 50);
        if(landingType == 3) font2.drawString("You lost, too far from the landing lot", ofGetWidth() / 2 - 280, ofGetHeight() / 2 - 50);
        if(landingType == 4) font2.drawString("You lost, your ship crashed", ofGetWidth() / 2 - 175, ofGetHeight() / 2 - 50);
        font2.drawString("Distance to landing lot: "  + ofToString(landingDistacne), ofGetWidth() / 2 - 250, ofGetHeight() / 2 - 100);
    }
    glDepthMask(true);
}


// 
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) {
    case '1':
        theCam = &cam;
    break;
    case '2':
        theCam = &trackCam;
        break;
    case '3':
        theCam = &landerCam;
        break;
	case 'B':
	case 'b':
		//bDisplayBBoxes = !bDisplayBBoxes;
        drawColl = !drawColl;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
        showHeading =  !showHeading;
        showSide = ! showSide;
		break;
    case 'A':
    case 'a':
        turnOnAGL = !turnOnAGL;
        break;
	case 'L':
	case 'l':
		//bDisplayLeafNodes = !bDisplayLeafNodes;
        lightOn = !lightOn;
		break;
	case 'O':
	case 'o':
		bDisplayOctree = !bDisplayOctree;
		break;
	case 'r':
		//cam.reset();
		break;
	case 's':
		//savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
//	case 'w':
//		toggleWireframeMode();
//		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
    case OF_KEY_LEFT:
        moveLeft = true;
        break;
    case OF_KEY_RIGHT:
        moveRight = true;
        break;
    case OF_KEY_UP:
        moveForward = true;
        break;
    case OF_KEY_DOWN:
        moveBackward = true;
        break;
    case 'W':
    case 'w':
        {
            thrust = true;
            if(!gameEnd) thrustSound.play();
        }
        break;
    case 'Q':
    case 'q':
        rotateLeft = true;
        break;
    case 'E':
    case 'e':
        rotateRight = true;
        break;
    case 'X':
    case 'x':
        showControl = !showControl;
        break;
    case ' ':
        gameStart = true;
        backGroundSound.play();
        //gameEnd = false;
        break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	switch (key) {
	
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
    case OF_KEY_LEFT:
        moveLeft = false;
        break;
    case OF_KEY_RIGHT:
        moveRight = false;
        break;
    case OF_KEY_UP:
        moveForward = false;
        break;
    case OF_KEY_DOWN:
        moveBackward = false;
        break;
    case 'W':
    case 'w':
        thrust = false;
        thrustSound.stop();
        break;
    case 'Q':
    case 'q':
        rotateLeft = false;
        break;
    case 'E':
    case 'e':
        rotateRight = false;
        break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	// if rover is loaded, test for selection
	//
	if (bLanderLoaded) {
		glm::vec3 origin = cam.getPosition();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
		if (hit) {
			bLanderSelected = true;
			mouseDownPos = getMousePointOnPlane(lander.getPosition(), cam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		}
		else {
			bLanderSelected = false;
		}
	}
	else {
		ofVec3f p;
        //float begin = ofGetElapsedTimeMillis();
        float begin = ofGetElapsedTimeMicros();
		raySelectWithOctree(p);
        //float end = ofGetElapsedTimeMillis();
        float end = ofGetElapsedTimeMicros();
        if(bTimingInfo) {
            searchTime = end - begin;
        }
	}
}

bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
	ofVec3f mouse(mouseX, mouseY, 0);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	pointSelected = octree.intersect(ray, octree.root, selectedNode);

	if (pointSelected) {
		pointRet = octree.mesh.getVertex(selectedNode.points[0]);
	}
	return pointSelected;
}

//detect the point on the ground 
bool ofApp::detectAltitude (ofVec3f &pointRet){
    Vector3 landerPoint = Vector3(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z);
    Vector3 rayPoint = Vector3(landerBot.x, landerBot.y, landerBot.z);
    Vector3 rayDir = rayPoint - landerPoint;
    
    Ray ray = Ray(rayPoint, rayDir);
    
    pointSelected = octree.intersect(ray, octree.root, selectedNode);

    if (pointSelected) {
        pointRet = octree.mesh.getVertex(selectedNode.points[0]);
    }
    return pointSelected;
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	if (bInDrag) {

		glm::vec3 landerPos = lander.getPosition();

		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
		glm::vec3 delta = mousePos - mouseLastPos;
	
		landerPos += delta;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();
        normalList.clear();
		octree.intersect(bounds, octree.root, colBoxList, normalList);
	

		/*if (bounds.overlap(testBox)) {
			cout << "overlap" << endl;
		}
		else {
			cout << "OK" << endl;
		}*/


	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
}



// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

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
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
	if (lander.loadModel(dragInfo.files[0])) {
		lander.setScaleNormalization(false);
//		lander.setScale(.1, .1, .1);
	//	lander.setPosition(point.x, point.y, point.z);
		lander.setPosition(1, 1, 0);

		bLanderLoaded = true;
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		cout << "Mesh Count: " << lander.getMeshCount() << endl;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0])) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		lander.setPosition(0, 0, 0);
		cout << "Number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//		lander.setRotation(1, 180, 1, 0, 0);

				// We want to drag and drop a 3D object in space so that the model appears 
				// under the mouse pointer where you drop it !
				//
				// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
				// once we find the point of intersection, we can position the lander/lander
				// at that location.
				//

				// Setup our rays
				//
		glm::vec3 origin = cam.getPosition();
		glm::vec3 camAxis = cam.getZAxis();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}

//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}
