#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "Octree.h"
#include <glm/gtx/intersect.hpp>
#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "ray.h"

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
    void dragEvent2(ofDragInfo dragInfo);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void drawAxis(ofVec3f);
    void initLightingAndMaterials();
    void savePicture();
    void toggleWireframeMode();
    void togglePointsDisplay();
    void toggleSelectTerrain();
    void setCameraTarget();
    bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
    bool raySelectWithOctree(ofVec3f &pointRet);
    glm::vec3 getMousePointOnPlane(glm::vec3 p , glm::vec3 n);
    
    ofxAssimpModelLoader mars, lander;
    ofLight light;
    
    bool bLanderSelected = false;
    
    glm::vec3 mouseDownPos, mouseLastPos;
    bool bInDrag = false;
    bool bTimingInfo;

    ofxIntSlider numLevels;
    //ofxToggle TimingInfo;
    ofxPanel gui;

    bool bAltKeyDown;
    bool bCtrlKeyDown;
    bool bWireframe;
    bool bDisplayPoints;
    bool bPointSelected;
    bool bHide;
    bool pointSelected = false;
    bool bDisplayLeafNodes = false;
    bool bDisplayOctree = false;
    bool bDisplayBBoxes = false;
    
    bool bLanderLoaded;
    bool bTerrainSelected;
    vector<Box> bboxList;
    ofVec3f selectedPoint;
    ofVec3f intersectPoint;

    float buildTime;
    float searchTime;
    
    const float selectionRange = 4.0;
    
    //setup function
    void landerSetup();
    void loadfile();
    
    
    //for lander system, Vehicle Maneuvering using Physics
    void landerMove();
    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool thrust = false;
    bool rotateLeft = false;
    bool rotateRight = false;
    float fuel = 120;
    float begin = 0;
    
    // physics based movement
    GravityForce *gravity;
    TurbulenceForce *turbForce;
    ParticleEmitter behavior;
    //exhaust system
    TurbulenceForce *exhaustForce;
    ParticleEmitter exhaust;
    //bool thrustOn = false;
    //Explosion Effect
    ImpulseRadialForce *explosionForce;
    ParticleEmitter explosion;
    bool crash = false;
    
    // integrator for rotation
    glm::mat4 rot;
    float rotation;
    
    // Get heading vector by using a matrix rotation
    bool showHeading = false;
    ofVec3f heading() {
        return glm::rotate(glm::radians(rotation), glm::vec3(0, 1, 0)) * glm::vec4(-1, 0, 0, 1);
    }
    // Get side vector by using a matrix rotation
    bool showSide = false;
    ofVec3f side() {
        return glm::rotate(glm::radians(rotation), glm::vec3(0, 1, 0)) * glm::vec4(0, 0, 1, 1);
    }
    
    //for altitude check, Altitude Sensor (Above Ground Level)
    float altitude;
    bool turnOnAGL = false;
    ofVec3f landerBot;
    
    bool detectAltitude (ofVec3f &pointRet);
    
    //visual effects rendered with OpenGL Shader
    ofImage background;
    void loadVbo();
    void drawShader();
    void loadEx();
    void drawEx();
    
    // textures
    ofTexture  particleTex, expl;
    // shaders
    ofVbo vbo;
    ofShader shader;
    
    //multiple cameras
    ofEasyCam cam;
    ofCamera trackCam; // Tracking camera, constantly on lander
    ofCamera landerCam; // Camera on the ship
    ofCamera *theCam;
    void cameraSetup();
    
    //Game Lighting
    ofLight areaLight1, areaLight2;           // Ambient lighting, to light the entire scene
    ofLight landerLight;                    // Light to illuminate lander.
    ofLight landing;                        // landing Lights to highlight landing spots
    bool lightOn = false;
    void lightSetup();
    
    //Collision Detection and Response
    ofMesh mesh;
    bool col = false;
    bool drawColl = false;
    ofVec3f normalVec = ofVec3f(0, 0 , 0);
    ofVec3f impulse = ofVec3f(0, 0, 0);
    float bound;
    Box boundingBox, landerBounds;
    Box testBox;
    vector<Box> colBoxList;
    vector<int> normalList;
    Octree octree;
    TreeNode selectedNode;
    void collision();
    
    //sound effect
    ofSoundPlayer explosionSound;
    ofSoundPlayer backGroundSound;
    ofSoundPlayer thrustSound;
    
    //other
    void updateLander();
    float landingDistacne;
    bool gameStart = false;
    bool gameEnd = false;
    bool showControl = false;
    bool landingStatus = false;
    int landingType;
    ofTrueTypeFont font1;
    ofTrueTypeFont font2;
};
