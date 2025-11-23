#include <GL/glut.h>
#include <cmath>
#include <string>

// Camera variables
float camX = 0.0f, camY = 5.0f, camZ = 40.0f;
float camYaw = 0.0f, camPitch = 0.0f;
float moveSpeed = 0.5f;

// Animation variables
float studentWalkCycle = 0.0f;
float cloudPos = -50.0f;
float flagAngle = 0.0f;
bool isNight = false;
bool animationPaused = false;

// Colors
float skyR = 0.53f, skyG = 0.81f, skyB = 0.92f;

// Function prototypes
void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void timer(int value);
void drawText(float x, float y, float z, const char* text);
void setupLighting();
void drawGround();
void drawMainGate();
void drawBuilding(float x, float z, float width, float height, float depth, float r, float g, float b);
void drawPlayground();
void drawTree(float x, float z);
void drawBench(float x, float z);
void drawStreetLight(float x, float z);
void drawStudent(float x, float z, float walkPhase);
void drawFlag(float x, float z);
void drawNameBoard();
void drawCar(float x, float z);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("3D DIU Mini Campus - Interactive Simulation");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, timer, 0);
    
    glutMainLoop();
    return 0;
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    glClearColor(skyR, skyG, skyB, 1.0f);
    setupLighting();
}

void setupLighting() {
    if (isNight) {
        GLfloat ambientLight[] = {0.1f, 0.1f, 0.15f, 1.0f};
        GLfloat diffuseLight[] = {0.3f, 0.3f, 0.4f, 1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    } else {
        GLfloat ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f};
        GLfloat diffuseLight[] = {0.8f, 0.8f, 0.7f, 1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    }
    
    GLfloat lightPosition[] = {50.0f, 100.0f, 50.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Camera setup
    float lookX = camX + sin(camYaw * M_PI / 180.0f);
    float lookY = camY + sin(camPitch * M_PI / 180.0f);
    float lookZ = camZ - cos(camYaw * M_PI / 180.0f);
    gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0, 1, 0);
    
    setupLighting();
    
    // Draw ground
    drawGround();
    
    // Draw main gate
    drawMainGate();
    
    // Draw name board
    drawNameBoard();
    
    // Draw academic buildings
    drawBuilding(-20, -15, 12, 15, 8, 0.9f, 0.85f, 0.7f);  // AB-1
    drawBuilding(8, -15, 10, 20, 8, 0.85f, 0.9f, 0.75f);   // AB-2
    drawBuilding(-25, -40, 15, 12, 10, 0.8f, 0.88f, 0.7f); // AB-3
    
    // Draw playground
    drawPlayground();
    
    // Draw trees
    for (int i = 0; i < 8; i++) {
        drawTree(-35 + i * 10, 10);
        drawTree(-35 + i * 10, -50);
    }
    
    // Draw benches
    drawBench(-10, 5);
    drawBench(5, 5);
    drawBench(15, -35);
    
    // Draw street lights
    drawStreetLight(-15, 0);
    drawStreetLight(15, 0);
    drawStreetLight(-15, -25);
    drawStreetLight(15, -25);
    
    // Draw animated students
    drawStudent(-8 + sin(studentWalkCycle) * 5, -10, studentWalkCycle);
    drawStudent(12 + cos(studentWalkCycle * 0.7f) * 4, -20, studentWalkCycle * 0.7f);
    drawStudent(-15 + sin(studentWalkCycle * 0.5f) * 6, -30, studentWalkCycle * 0.5f);
    
    // Draw flag
    drawFlag(25, -10);
    
    // Draw car
    drawCar(-5 + sin(studentWalkCycle * 0.3f) * 3, 3);
    
    // Draw clouds
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(cloudPos, 25, -30);
    glutSolidSphere(3, 15, 15);
    glTranslatef(4, 0, 0);
    glutSolidSphere(3.5, 15, 15);
    glTranslatef(4, 0, 0);
    glutSolidSphere(3, 15, 15);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(cloudPos - 30, 28, -20);
    glutSolidSphere(2.5, 15, 15);
    glTranslatef(3, 0, 0);
    glutSolidSphere(3, 15, 15);
    glTranslatef(3, 0, 0);
    glutSolidSphere(2.5, 15, 15);
    glPopMatrix();
    
    glutSwapBuffers();
}

void drawGround() {
    // Ground plane
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.5f, 0.3f);
    glVertex3f(-100, 0, 50);
    glVertex3f(100, 0, 50);
    glVertex3f(100, 0, -100);
    glVertex3f(-100, 0, -100);
    glEnd();
    
    // Road
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex3f(-8, 0.01f, 50);
    glVertex3f(8, 0.01f, 50);
    glVertex3f(8, 0.01f, -100);
    glVertex3f(-8, 0.01f, -100);
    glEnd();
    
    // Road markings
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 20; i++) {
        glBegin(GL_QUADS);
        glVertex3f(-0.2f, 0.02f, 40 - i * 8);
        glVertex3f(0.2f, 0.02f, 40 - i * 8);
        glVertex3f(0.2f, 0.02f, 37 - i * 8);
        glVertex3f(-0.2f, 0.02f, 37 - i * 8);
        glEnd();
    }
}

void drawMainGate() {
    // Left pillar
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(-10, 4, 15);
    glScalef(1.5f, 8, 1.5f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Right pillar
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(10, 4, 15);
    glScalef(1.5f, 8, 1.5f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Top arch
    glPushMatrix();
    glColor3f(0.2f, 0.6f, 0.3f);
    glTranslatef(0, 8, 15);
    glScalef(20, 1, 2);
    glutSolidCube(1);
    glPopMatrix();
    
    // DIU logo (simplified as colored circle)
    glPushMatrix();
    glColor3f(0.9f, 0.7f, 0.1f);
    glTranslatef(0, 8.5f, 15);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.3, 1.2, 20, 20);
    glPopMatrix();
    
    // Gate bars
    glColor3f(0.2f, 0.2f, 0.2f);
    for (int i = -4; i <= 4; i++) {
        glPushMatrix();
        glTranslatef(i * 2, 3, 15);
        glScalef(0.2f, 6, 0.2f);
        glutSolidCube(1);
        glPopMatrix();
    }
}

void drawNameBoard() {
    // Board stand
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    glTranslatef(0, 4, 12);
    glScalef(0.5f, 8, 0.5f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Board
    glPushMatrix();
    glColor3f(0.2f, 0.5f, 0.2f);
    glTranslatef(0, 7, 12);
    glScalef(18, 3, 0.3f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Text would go here (simplified)
}

void drawBuilding(float x, float z, float width, float height, float depth, float r, float g, float b) {
    // Main building block
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(x, height / 2, z);
    glScalef(width, height, depth);
    glutSolidCube(1);
    glPopMatrix();
    
    // Windows
    glColor3f(0.4f, 0.6f, 0.8f);
    int floors = (int)(height / 3);
    int windowsPerFloor = (int)(width / 2);
    
    for (int floor = 0; floor < floors; floor++) {
        for (int win = 0; win < windowsPerFloor; win++) {
            glPushMatrix();
            glTranslatef(x - width / 2 + 1 + win * 2, 2 + floor * 3, z + depth / 2 + 0.1f);
            glScalef(0.8f, 1.2f, 0.1f);
            glutSolidCube(1);
            glPopMatrix();
        }
    }
    
    // Door
    glPushMatrix();
    glColor3f(0.3f, 0.2f, 0.1f);
    glTranslatef(x, 1.5f, z + depth / 2 + 0.1f);
    glScalef(1.5f, 3, 0.1f);
    glutSolidCube(1);
    glPopMatrix();
}

void drawPlayground() {
    // Playground grass
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 0.2f);
    glVertex3f(15, 0.01f, -25);
    glVertex3f(35, 0.01f, -25);
    glVertex3f(35, 0.01f, -45);
    glVertex3f(15, 0.01f, -45);
    glEnd();
    
    // Boundary lines
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(15, 0.02f, -25);
    glVertex3f(35, 0.02f, -25);
    glVertex3f(35, 0.02f, -45);
    glVertex3f(15, 0.02f, -45);
    glEnd();
    
    // Goal posts
    glColor3f(1.0f, 1.0f, 1.0f);
    // Left goal
    glPushMatrix();
    glTranslatef(15, 2, -35);
    glScalef(0.3f, 4, 0.3f);
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(15, 2, -31);
    glScalef(0.3f, 4, 0.3f);
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(15, 4, -33);
    glScalef(0.3f, 0.3f, 4);
    glutSolidCube(1);
    glPopMatrix();
    
    // Right goal
    glPushMatrix();
    glTranslatef(35, 2, -35);
    glScalef(0.3f, 4, 0.3f);
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(35, 2, -31);
    glScalef(0.3f, 4, 0.3f);
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(35, 4, -33);
    glScalef(0.3f, 0.3f, 4);
    glutSolidCube(1);
    glPopMatrix();
}

void drawTree(float x, float z) {
    // Trunk
    glPushMatrix();
    glColor3f(0.4f, 0.2f, 0.1f);
    glTranslatef(x, 2, z);
    glScalef(0.5f, 4, 0.5f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Leaves
    glPushMatrix();
    glColor3f(0.1f, 0.5f, 0.1f);
    glTranslatef(x, 5, z);
    glutSolidSphere(2, 15, 15);
    glPopMatrix();
}

void drawBench(float x, float z) {
    // Legs
    glColor3f(0.3f, 0.3f, 0.3f);
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(x + i * 2 - 1, 0.5f, z);
        glScalef(0.2f, 1, 0.2f);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    // Seat
    glPushMatrix();
    glColor3f(0.5f, 0.3f, 0.1f);
    glTranslatef(x, 1.2f, z);
    glScalef(3, 0.2f, 1);
    glutSolidCube(1);
    glPopMatrix();
    
    // Back
    glPushMatrix();
    glColor3f(0.5f, 0.3f, 0.1f);
    glTranslatef(x, 2, z - 0.4f);
    glScalef(3, 1.5f, 0.2f);
    glutSolidCube(1);
    glPopMatrix();
}

void drawStreetLight(float x, float z) {
    // Pole
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    glTranslatef(x, 4, z);
    glScalef(0.3f, 8, 0.3f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Light
    glPushMatrix();
    if (isNight) {
        glColor3f(1.0f, 1.0f, 0.7f);
    } else {
        glColor3f(0.9f, 0.9f, 0.9f);
    }
    glTranslatef(x, 8, z);
    glutSolidSphere(0.8f, 15, 15);
    glPopMatrix();
}

void drawStudent(float x, float z, float walkPhase) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    
    // Body
    glPushMatrix();
    glColor3f(0.2f, 0.3f, 0.8f);
    glTranslatef(0, 2, 0);
    glScalef(0.8f, 1.5f, 0.5f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Head
    glPushMatrix();
    glColor3f(0.9f, 0.7f, 0.6f);
    glTranslatef(0, 3.2f, 0);
    glutSolidSphere(0.5f, 15, 15);
    glPopMatrix();
    
    // Legs (animated)
    float legSwing = sin(walkPhase * 5) * 0.3f;
    
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f);
    glTranslatef(-0.3f, 0.8f, legSwing);
    glScalef(0.3f, 1.5f, 0.3f);
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f);
    glTranslatef(0.3f, 0.8f, -legSwing);
    glScalef(0.3f, 1.5f, 0.3f);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
}

void drawFlag(float x, float z) {
    // Pole
    glPushMatrix();
    glColor3f(0.7f, 0.7f, 0.7f);
    glTranslatef(x, 5, z);
    glScalef(0.2f, 10, 0.2f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Flag (animated)
    glPushMatrix();
    glTranslatef(x, 8, z);
    glRotatef(sin(flagAngle) * 10, 0, 0, 1);
    
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.5f, 0.2f);
    glVertex3f(0, 1, 0);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, -1, 0);
    glVertex3f(3, 0, 0);
    glEnd();
    
    glPopMatrix();
}

void drawCar(float x, float z) {
    // Car body
    glPushMatrix();
    glColor3f(0.8f, 0.1f, 0.1f);
    glTranslatef(x, 1, z);
    glScalef(3, 1.2f, 2);
    glutSolidCube(1);
    glPopMatrix();
    
    // Car top
    glPushMatrix();
    glColor3f(0.7f, 0.1f, 0.1f);
    glTranslatef(x - 0.3f, 2, z);
    glScalef(1.8f, 1, 1.6f);
    glutSolidCube(1);
    glPopMatrix();
    
    // Wheels
    glColor3f(0.1f, 0.1f, 0.1f);
    float wheelPositions[4][2] = {{-1, 0.8f}, {1, 0.8f}, {-1, -0.8f}, {1, -0.8f}};
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(x + wheelPositions[i][0], 0.5f, z + wheelPositions[i][1]);
        glRotatef(90, 0, 0, 1);
        glutSolidTorus(0.1, 0.4, 10, 10);
        glPopMatrix();
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W':
            camX += sin(camYaw * M_PI / 180.0f) * moveSpeed;
            camZ -= cos(camYaw * M_PI / 180.0f) * moveSpeed;
            break;
        case 's': case 'S':
            camX -= sin(camYaw * M_PI / 180.0f) * moveSpeed;
            camZ += cos(camYaw * M_PI / 180.0f) * moveSpeed;
            break;
        case 'a': case 'A':
            camX -= cos(camYaw * M_PI / 180.0f) * moveSpeed;
            camZ -= sin(camYaw * M_PI / 180.0f) * moveSpeed;
            break;
        case 'd': case 'D':
            camX += cos(camYaw * M_PI / 180.0f) * moveSpeed;
            camZ += sin(camYaw * M_PI / 180.0f) * moveSpeed;
            break;
        case 'n': case 'N':
            isNight = !isNight;
            if (isNight) {
                glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
            } else {
                glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
            }
            break;
        case 'p': case 'P':
            animationPaused = !animationPaused;
            break;
        case 'r': case 'R':
            camX = 0.0f; camY = 5.0f; camZ = 40.0f;
            camYaw = 0.0f; camPitch = 0.0f;
            break;
        case 27: // ESC
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            camPitch += 2.0f;
            if (camPitch > 89.0f) camPitch = 89.0f;
            break;
        case GLUT_KEY_DOWN:
            camPitch -= 2.0f;
            if (camPitch < -89.0f) camPitch = -89.0f;
            break;
        case GLUT_KEY_LEFT:
            camYaw -= 2.0f;
            break;
        case GLUT_KEY_RIGHT:
            camYaw += 2.0f;
            break;
        case GLUT_KEY_PAGE_UP:
            camY += 0.5f;
            break;
        case GLUT_KEY_PAGE_DOWN:
            camY -= 0.5f;
            if (camY < 1.0f) camY = 1.0f;
            break;
    }
    glutPostRedisplay();
}

void timer(int value) {
    if (!animationPaused) {
        studentWalkCycle += 0.03f;
        cloudPos += 0.05f;
        if (cloudPos > 50) cloudPos = -50;
        flagAngle += 0.05f;
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}