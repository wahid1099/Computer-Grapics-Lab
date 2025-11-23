#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

#include "camera.h"
#include "lighting.h"
#include "textures.h"
#include "buildings.h"
#include "environment.h"
#include "animations.h"
#include "utils.h"

// Window dimensions
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Global camera object
Camera camera;

// Keyboard state tracking
bool keys[256] = {false};
bool specialKeys[256] = {false};

// Function prototypes
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void specialKeyboardUp(int key, int x, int y);
void timer(int value);
void processKeys();
void init();

void init() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Enable smooth shading
    glShadeModel(GL_SMOOTH);
    
    // Set clear color (sky blue for day)
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    
    // Enable texturing
    glEnable(GL_TEXTURE_2D);
    
    // Initialize lighting
    initLighting();
    
    // Load all textures
    loadAllTextures();
    
    // Enable backface culling for better performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    std::cout << "\n=== DIU Mini Campus 3D ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  W/A/S/D - Move camera" << std::endl;
    std::cout << "  Arrow Keys - Rotate camera" << std::endl;
    std::cout << "  Q/E - Move up/down" << std::endl;
    std::cout << "  N - Toggle day/night mode" << std::endl;
    std::cout << "  P - Pause/resume animations" << std::endl;
    std::cout << "  R - Reset camera" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "========================\n" << std::endl;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)WINDOW_WIDTH / (double)WINDOW_HEIGHT, 0.1, 500.0);
    
    // Set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply camera transformation
    camera.apply();
    
    // Draw skybox first (no depth write)
    glDepthMask(GL_FALSE);
    drawSkybox();
    glDepthMask(GL_TRUE);
    
    // Draw all environment objects
    drawAllEnvironment();
    
    // Draw all buildings
    drawMainGate();
    drawNameBoard();
    drawAcademicBuilding1();
    drawAcademicBuilding2();
    drawAcademicBuilding3();
    
    // Draw all animations
    drawAllAnimations();
    
    glutSwapBuffers();
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / (double)height, 0.1, 500.0);
    
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    keys[key] = true;
    
    // Handle immediate actions
    switch (key) {
        case 27:  // ESC key
            std::cout << "Exiting..." << std::endl;
            exit(0);
            break;
            
        case 'n':
        case 'N':
            if (isNightMode) {
                setDayMode();
                glClearColor(0.53f, 0.81f, 0.92f, 1.0f);  // Sky blue
                std::cout << "Switched to DAY mode" << std::endl;
            } else {
                setNightMode();
                glClearColor(0.05f, 0.05f, 0.15f, 1.0f);  // Dark blue
                std::cout << "Switched to NIGHT mode" << std::endl;
            }
            glutPostRedisplay();
            break;
            
        case 'p':
        case 'P':
            animationPaused = !animationPaused;
            std::cout << "Animations " << (animationPaused ? "PAUSED" : "RESUMED") << std::endl;
            break;
            
        case 'r':
        case 'R':
            camera.reset();
            std::cout << "Camera reset to initial position" << std::endl;
            glutPostRedisplay();
            break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void specialKeyboard(int key, int x, int y) {
    specialKeys[key] = true;
}

void specialKeyboardUp(int key, int x, int y) {
    specialKeys[key] = false;
}

void processKeys() {
    // Camera movement (WASD)
    if (keys['w'] || keys['W']) camera.moveForward();
    if (keys['s'] || keys['S']) camera.moveBackward();
    if (keys['a'] || keys['A']) camera.moveLeft();
    if (keys['d'] || keys['D']) camera.moveRight();
    if (keys['q'] || keys['Q']) camera.moveUp();
    if (keys['e'] || keys['E']) camera.moveDown();
    
    // Camera rotation (Arrow keys)
    if (specialKeys[GLUT_KEY_LEFT]) camera.rotateLeft();
    if (specialKeys[GLUT_KEY_RIGHT]) camera.rotateRight();
    if (specialKeys[GLUT_KEY_UP]) camera.rotateUp();
    if (specialKeys[GLUT_KEY_DOWN]) camera.rotateDown();
}

void timer(int value) {
    // Process continuous key presses
    processKeys();
    
    // Update all animations
    updateAllAnimations();
    
    // Request redisplay
    glutPostRedisplay();
    
    // Set timer for next frame (60 FPS)
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DIU Mini Campus 3D - OpenGL Project");
    
    // Initialize OpenGL settings
    init();
    
    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutTimerFunc(0, timer, 0);
    
    // Start main loop
    glutMainLoop();
    
    return 0;
}
