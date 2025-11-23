#include <GL/glut.h>
#include <cmath>
#include <iostream>

// === CAMERA & MOVEMENT ===
float camX = 0.0f, camY = 8.0f, camZ = 50.0f;
float camYaw = 0.0f, camPitch = 0.0f;
float moveSpeed = 0.6f;

// === ANIMATION VARIABLES ===
float studentWalkCycle = 0.0f;
float cloudPos = -50.0f;
float flagAngle = 0.0f;
float carPos = -30.0f;
float busPos = 40.0f;
float birdWingAngle = 0.0f;
float birdPosX = -40.0f;
float birdPosZ = -20.0f;
float fountainHeight = 0.0f;
float windmillAngle = 0.0f;
bool isNight = false;
bool animationPaused = false;
int timeOfDay = 0; // 0=day, 1=sunset, 2=night

// === MOVING OBJECTS STATE ===
struct MovingStudent {
    float x, z, angle, speed, pathRadius;
};
MovingStudent students[5];

struct MovingCar {
    float x, z, angle, speed;
    float r, g, b;
};
MovingCar cars[3];

// Function prototypes
void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void timer(int value);
void setupLighting();
void drawGround();
void drawRoads();
void drawMainGate();
void drawBuilding(float x, float z, float width, float height, float depth, float r, float g, float b);
void drawPlayground();
void drawTree(float x, float z);
void drawBench(float x, float z);
void drawStreetLight(float x, float z, bool lit);
void drawStudent(float x, float z, float angle, float walkPhase);
void drawFlag(float x, float z);
void drawNameBoard();
void drawCar(float x, float z, float angle, float r, float g, float b);
void drawBus(float x, float z, float angle);
void drawBird(float x, float y, float z, float wingAngle);
void drawFountain(float x, float z);
void drawWindmill(float x, float z);
void drawClouds();
void initMovingObjects();
void updateMovingObjects();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1400, 800);
    glutCreateWindow("🎓 3D DIU Campus - Advanced Animation & Movement");

    init();
    initMovingObjects();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, timer, 0);

    std::cout << "\n=== 3D DIU CAMPUS SIMULATION ===\n";
    std::cout << "CONTROLS:\n";
    std::cout << "W/A/S/D    - Move camera\n";
    std::cout << "Arrow Keys - Look around\n";
    std::cout << "N          - Cycle Day/Sunset/Night\n";
    std::cout << "P          - Pause/Resume animations\n";
    std::cout << "R          - Reset camera\n";
    std::cout << "ESC        - Exit\n\n";

    glutMainLoop();
    return 0;
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);

    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    setupLighting();
}

void initMovingObjects() {
    // Initialize 5 students walking in different patterns
    students[0] = { -15, -10, 0, 0.08f, 8 };  // Circular path
    students[1] = { 10, -20, 90, 0.06f, 6 };   // Circular path
    students[2] = { -20, -30, 0, 0.05f, 0 };   // Linear path
    students[3] = { 5, -15, 180, 0.07f, 5 };   // Circular path
    students[4] = { -10, -35, 270, 0.04f, 0 }; // Linear path

    // Initialize 3 cars with different colors and speeds
    cars[0] = { -30, 2, 0, 0.15f, 0.8f, 0.1f, 0.1f }; // Red car
    cars[1] = { 20, 2, 180, 0.12f, 0.1f, 0.3f, 0.8f }; // Blue car
    cars[2] = { -10, 2, 0, 0.18f, 0.2f, 0.7f, 0.2f };  // Green car
}

void setupLighting() {
    GLfloat ambientLight[4], diffuseLight[4];

    if (timeOfDay == 0) { // Day
        ambientLight[0] = 0.5f; ambientLight[1] = 0.5f; ambientLight[2] = 0.5f; ambientLight[3] = 1.0f;
        diffuseLight[0] = 1.0f; diffuseLight[1] = 1.0f; diffuseLight[2] = 0.9f; diffuseLight[3] = 1.0f;
    }
    else if (timeOfDay == 1) { // Sunset
        ambientLight[0] = 0.4f; ambientLight[1] = 0.25f; ambientLight[2] = 0.15f; ambientLight[3] = 1.0f;
        diffuseLight[0] = 0.9f; diffuseLight[1] = 0.5f; diffuseLight[2] = 0.3f; diffuseLight[3] = 1.0f;
    }
    else { // Night
        ambientLight[0] = 0.1f; ambientLight[1] = 0.1f; ambientLight[2] = 0.2f; ambientLight[3] = 1.0f;
        diffuseLight[0] = 0.3f; diffuseLight[1] = 0.3f; diffuseLight[2] = 0.5f; diffuseLight[3] = 1.0f;
    }

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

    GLfloat lightPosition[] = { 50.0f, 100.0f, 50.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera
    float lookX = camX + sin(camYaw * M_PI / 180.0f);
    float lookY = camY + sin(camPitch * M_PI / 180.0f);
    float lookZ = camZ - cos(camYaw * M_PI / 180.0f);
    gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0, 1, 0);

    setupLighting();

    // === STATIC ENVIRONMENT ===
    drawGround();
    drawRoads();
    drawMainGate();
    drawNameBoard();

    // Buildings
    drawBuilding(-25, -15, 14, 18, 10, 0.9f, 0.85f, 0.7f);  // AB-1
    drawBuilding(12, -15, 12, 24, 10, 0.85f, 0.9f, 0.75f);  // AB-2
    drawBuilding(-30, -45, 18, 15, 12, 0.8f, 0.88f, 0.7f);  // AB-3
    drawBuilding(25, -45, 15, 20, 10, 0.88f, 0.85f, 0.72f); // AB-4

    drawPlayground();

    // Trees arranged in rows
    for (int i = 0; i < 10; i++) {
        drawTree(-40 + i * 9, 12);
        drawTree(-40 + i * 9, -55);
    }
    for (int i = 0; i < 5; i++) {
        drawTree(-42, -10 - i * 8);
        drawTree(42, -10 - i * 8);
    }

    // Benches
    drawBench(-15, 8);
    drawBench(8, 8);
    drawBench(-12, -25);
    drawBench(18, -35);

    // Street lights
    bool lightsOn = (timeOfDay == 2);
    drawStreetLight(-15, 5, lightsOn);
    drawStreetLight(15, 5, lightsOn);
    drawStreetLight(-15, -20, lightsOn);
    drawStreetLight(15, -20, lightsOn);
    drawStreetLight(-15, -40, lightsOn);
    drawStreetLight(15, -40, lightsOn);

    // Special features
    drawFountain(0, -25);
    drawWindmill(35, 8);
    drawFlag(30, -12);

    // === ANIMATED MOVING OBJECTS ===

    // Moving students
    for (int i = 0; i < 5; i++) {
        drawStudent(students[i].x, students[i].z, students[i].angle, studentWalkCycle);
    }

    // Moving cars
    for (int i = 0; i < 3; i++) {
        drawCar(cars[i].x, cars[i].z, cars[i].angle, cars[i].r, cars[i].g, cars[i].b);
    }

    // Moving bus
    drawBus(busPos, 2, 180);

    // Flying birds
    drawBird(birdPosX, 15, birdPosZ, birdWingAngle);
    drawBird(birdPosX + 5, 17, birdPosZ - 3, -birdWingAngle);
    drawBird(birdPosX + 10, 16, birdPosZ - 6, birdWingAngle);

    // Clouds
    drawClouds();

    glutSwapBuffers();
}

void drawGround() {
    glBegin(GL_QUADS);
    glColor3f(0.25f, 0.55f, 0.25f);
    glVertex3f(-100, 0, 50);
    glVertex3f(100, 0, 50);
    glVertex3f(100, 0, -100);
    glVertex3f(-100, 0, -100);
    glEnd();
}

void drawRoads() {
    // Main road
    glBegin(GL_QUADS);
    glColor3f(0.25f, 0.25f, 0.25f);
    glVertex3f(-10, 0.01f, 50);
    glVertex3f(10, 0.01f, 50);
    glVertex3f(10, 0.01f, -100);
    glVertex3f(-10, 0.01f, -100);
    glEnd();

    // Road markings
    glColor3f(1.0f, 1.0f, 0.8f);
    for (int i = 0; i < 25; i++) {
        glBegin(GL_QUADS);
        glVertex3f(-0.3f, 0.02f, 45 - i * 6);
        glVertex3f(0.3f, 0.02f, 45 - i * 6);
        glVertex3f(0.3f, 0.02f, 42 - i * 6);
        glVertex3f(-0.3f, 0.02f, 42 - i * 6);
        glEnd();
    }
}

void drawMainGate() {
    // Left pillar
    glPushMatrix();
    glColor3f(0.95f, 0.95f, 0.95f);
    glTranslatef(-12, 5, 18);
    glScalef(2, 10, 2);
    glutSolidCube(1);
    glPopMatrix();

    // Right pillar
    glPushMatrix();
    glColor3f(0.95f, 0.95f, 0.95f);
    glTranslatef(12, 5, 18);
    glScalef(2, 10, 2);
    glutSolidCube(1);
    glPopMatrix();

    // Top arch
    glPushMatrix();
    glColor3f(0.15f, 0.65f, 0.25f);
    glTranslatef(0, 10, 18);
    glScalef(24, 1.5f, 2.5f);
    glutSolidCube(1);
    glPopMatrix();

    // DIU logo
    glPushMatrix();
    glColor3f(1.0f, 0.8f, 0.1f);
    glTranslatef(0, 10.8f, 18);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.4, 1.5, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2f, 0.6f, 0.3f);
    glTranslatef(0, 10.8f, 18);
    glRotatef(90, 1, 0, 0);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
}

void drawNameBoard() {
    glPushMatrix();
    glColor3f(0.25f, 0.25f, 0.25f);
    glTranslatef(0, 4.5f, 14);
    glScalef(0.6f, 9, 0.6f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.15f, 0.55f, 0.2f);
    glTranslatef(0, 8, 14);
    glScalef(22, 3.5f, 0.4f);
    glutSolidCube(1);
    glPopMatrix();
}

void drawBuilding(float x, float z, float width, float height, float depth, float r, float g, float b) {
    // Main structure
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(x, height / 2, z);
    glScalef(width, height, depth);
    glutSolidCube(1);
    glPopMatrix();

    // Windows
    glColor3f(0.3f, 0.5f, 0.7f);
    int floors = (int)(height / 3.5f);
    int windowsPerFloor = (int)(width / 2.5f);

    for (int floor = 0; floor < floors; floor++) {
        for (int win = 0; win < windowsPerFloor; win++) {
            glPushMatrix();
            glTranslatef(x - width / 2 + 1.5f + win * 2.5f, 2.5f + floor * 3.5f, z + depth / 2 + 0.15f);
            glScalef(1.0f, 1.5f, 0.1f);
            glutSolidCube(1);
            glPopMatrix();
        }
    }

    // Door
    glPushMatrix();
    glColor3f(0.25f, 0.15f, 0.05f);
    glTranslatef(x, 1.8f, z + depth / 2 + 0.15f);
    glScalef(2, 3.5f, 0.1f);
    glutSolidCube(1);
    glPopMatrix();
}

void drawPlayground() {
    glBegin(GL_QUADS);
    glColor3f(0.15f, 0.65f, 0.15f);
    glVertex3f(18, 0.01f, -28);
    glVertex3f(40, 0.01f, -28);
    glVertex3f(40, 0.01f, -50);
    glVertex3f(18, 0.01f, -50);
    glEnd();

    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(18, 0.02f, -28);
    glVertex3f(40, 0.02f, -28);
    glVertex3f(40, 0.02f, -50);
    glVertex3f(18, 0.02f, -50);
    glEnd();

    // Goal posts
    glColor3f(1.0f, 1.0f, 1.0f);
    float goalPosts[2][2] = { {18, -39}, {40, -39} };
    for (int i = 0; i < 2; i++) {
        for (int j = -2; j <= 2; j += 4) {
            glPushMatrix();
            glTranslatef(goalPosts[i][0], 2.5f, goalPosts[i][1] + j);
            glScalef(0.3f, 5, 0.3f);
            glutSolidCube(1);
            glPopMatrix();
        }
        glPushMatrix();
        glTranslatef(goalPosts[i][0], 5, goalPosts[i][1]);
        glScalef(0.3f, 0.3f, 4.5f);
        glutSolidCube(1);
        glPopMatrix();
    }
}

void drawTree(float x, float z) {
    glPushMatrix();
    glColor3f(0.35f, 0.2f, 0.1f);
    glTranslatef(x, 2.5f, z);
    glScalef(0.6f, 5, 0.6f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.1f, 0.5f, 0.1f);
    glTranslatef(x, 6, z);
    glutSolidSphere(2.5f, 20, 20);
    glPopMatrix();
}

void drawBench(float x, float z) {
    glColor3f(0.25f, 0.25f, 0.25f);
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(x + i * 2.5f - 1.25f, 0.6f, z);
        glScalef(0.25f, 1.2f, 0.25f);
        glutSolidCube(1);
        glPopMatrix();
    }

    glPushMatrix();
    glColor3f(0.5f, 0.3f, 0.1f);
    glTranslatef(x, 1.4f, z);
    glScalef(3.5f, 0.3f, 1.2f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, 2.2f, z - 0.5f);
    glScalef(3.5f, 1.8f, 0.3f);
    glutSolidCube(1);
    glPopMatrix();
}

void drawStreetLight(float x, float z, bool lit) {
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    glTranslatef(x, 4.5f, z);
    glScalef(0.35f, 9, 0.35f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    if (lit) {
        glColor3f(1.0f, 1.0f, 0.6f);
    }
    else {
        glColor3f(0.8f, 0.8f, 0.8f);
    }
    glTranslatef(x, 9, z);
    glutSolidSphere(1.0f, 20, 20);
    glPopMatrix();
}

void drawStudent(float x, float z, float angle, float walkPhase) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glRotatef(angle, 0, 1, 0);

    // Body
    glPushMatrix();
    glColor3f(0.2f, 0.4f, 0.8f);
    glTranslatef(0, 2.2f, 0);
    glScalef(1.0f, 1.8f, 0.6f);
    glutSolidCube(1);
    glPopMatrix();

    // Head
    glPushMatrix();
    glColor3f(0.9f, 0.7f, 0.5f);
    glTranslatef(0, 3.6f, 0);
    glutSolidSphere(0.6f, 20, 20);
    glPopMatrix();

    // Backpack
    glPushMatrix();
    glColor3f(0.6f, 0.2f, 0.2f);
    glTranslatef(0, 2.5f, -0.5f);
    glScalef(0.7f, 1.2f, 0.4f);
    glutSolidCube(1);
    glPopMatrix();

    // Legs (animated)
    float legSwing = sin(walkPhase * 6) * 0.4f;

    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.3f);
    glTranslatef(-0.35f, 0.9f, legSwing);
    glScalef(0.35f, 1.8f, 0.35f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.35f, 0.9f, -legSwing);
    glScalef(0.35f, 1.8f, 0.35f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}

void drawFlag(float x, float z) {
    glPushMatrix();
    glColor3f(0.6f, 0.6f, 0.6f);
    glTranslatef(x, 6, z);
    glScalef(0.25f, 12, 0.25f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, 10, z);
    glRotatef(sin(flagAngle) * 15, 0, 0, 1);

    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.6f, 0.25f);
    glVertex3f(0, 1.5f, 0);
    glColor3f(0.9f, 0.1f, 0.1f);
    glVertex3f(0, -1.5f, 0);
    glVertex3f(4, 0, 0);
    glEnd();

    glPopMatrix();
}

void drawCar(float x, float z, float angle, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glRotatef(angle, 0, 1, 0);

    // Body
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(0, 1.2f, 0);
    glScalef(4, 1.5f, 2.2f);
    glutSolidCube(1);
    glPopMatrix();

    // Top
    glPushMatrix();
    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f);
    glTranslatef(-0.5f, 2.4f, 0);
    glScalef(2.5f, 1.2f, 2.0f);
    glutSolidCube(1);
    glPopMatrix();

    // Wheels
    glColor3f(0.1f, 0.1f, 0.1f);
    float wheels[4][2] = { {-1.2f, 1.0f}, {1.2f, 1.0f}, {-1.2f, -1.0f}, {1.2f, -1.0f} };
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(wheels[i][0], 0.6f, wheels[i][1]);
        glRotatef(90, 0, 0, 1);
        glutSolidTorus(0.15, 0.5, 10, 15);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawBus(float x, float z, float angle) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glRotatef(angle, 0, 1, 0);

    // Body
    glPushMatrix();
    glColor3f(0.9f, 0.7f, 0.1f);
    glTranslatef(0, 2, 0);
    glScalef(8, 3, 3);
    glutSolidCube(1);
    glPopMatrix();

    // Windows
    glColor3f(0.3f, 0.5f, 0.7f);
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(-2.5f + i * 1.8f, 2.5f, 1.55f);
        glScalef(1.2f, 1.0f, 0.1f);
        glutSolidCube(1);
        glPopMatrix();
    }

    // Wheels
    glColor3f(0.1f, 0.1f, 0.1f);
    float wheels[4][2] = { {-2.5f, 1.3f}, {-0.8f, 1.3f}, {0.8f, 1.3f}, {2.5f, 1.3f} };
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(wheels[i][0], 0.7f, wheels[i][1]);
        glRotatef(90, 0, 0, 1);
        glutSolidTorus(0.2, 0.6, 12, 15);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawBird(float x, float y, float z, float wingAngle) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Body
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    glScalef(0.8f, 0.5f, 0.5f);
    glutSolidSphere(0.5f, 10, 10);
    glPopMatrix();

    // Wings
    glPushMatrix();
    glRotatef(wingAngle, 0, 0, 1);
    glTranslatef(0.6f, 0, 0);
    glScalef(1.2f, 0.1f, 0.6f);
    glColor3f(0.2f, 0.2f, 0.2f);
    glutSolidSphere(0.4f, 8, 8);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-wingAngle, 0, 0, 1);
    glTranslatef(-0.6f, 0, 0);
    glScalef(1.2f, 0.1f, 0.6f);
    glutSolidSphere(0.4f, 8, 8);
    glPopMatrix();

    glPopMatrix();
}

void drawFountain(float x, float z) {
    // Base
    glPushMatrix();
    glColor3f(0.7f, 0.7f, 0.7f);
    glTranslatef(x, 0.5f, z);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(2, 1, 20, 10);
    glPopMatrix();

    // Water jets
    glColor3f(0.3f, 0.6f, 0.9f);
    for (int i = 0; i < 6; i++) {
        float angle = i * 60.0f * M_PI / 180.0f;
        glPushMatrix();
        glTranslatef(x + cos(angle) * 1.2f, 1 + fountainHeight, z + sin(angle) * 1.2f);
        glutSolidSphere(0.2f, 10, 10);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(x, 1 + fountainHeight * 1.5f, z);
    glutSolidSphere(0.3f, 10, 10);
    glPopMatrix();
}

void drawWindmill(float x, float z) {
    // Tower
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(x, 5, z);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(1.5, 10, 20, 10);
    glPopMatrix();

    // Rotating blades
    glPushMatrix();
    glTranslatef(x, 10, z);
    glRotatef(windmillAngle, 0, 0, 1);

    // 3 blades
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef(i * 120, 0, 0, 1);
        glColor3f(0.2f, 0.3f, 0.7f);
        glTranslatef(0, 2.5, 0);
        glScalef(0.4f, 5, 0.1f);
        glutSolidCube(1);
        glPopMatrix();
    }

    // Center hub
    glColor3f(0.3f, 0.3f, 0.3f);
    glutSolidSphere(0.5, 15, 15);

    glPopMatrix();
}

void drawClouds() {
    glColor3f(1.0f, 1.0f, 1.0f);

    // Cloud 1
    glPushMatrix();
    glTranslatef(cloudPos, 28, -35);
    glutSolidSphere(3, 15, 15);
    glTranslatef(4, 0, 0);
    glutSolidSphere(3.5, 15, 15);
    glTranslatef(4, 0, 0);
    glutSolidSphere(3, 15, 15);
    glPopMatrix();

    // Cloud 2
    glPushMatrix();
    glTranslatef(cloudPos - 35, 30, -20);
    glutSolidSphere(2.5, 15, 15);
    glTranslatef(3, 0, 0);
    glutSolidSphere(3, 15, 15);
    glTranslatef(3, 0, 0);
    glutSolidSphere(2.5, 15, 15);
    glPopMatrix();

    // Cloud 3
    glPushMatrix();
    glTranslatef(cloudPos + 20, 32, -45);
    glutSolidSphere(2.8, 15, 15);
    glTranslatef(3.5, 0, 0);
    glutSolidSphere(3.2, 15, 15);
    glTranslatef(3.5, 0, 0);
    glutSolidSphere(2.8, 15, 15);
    glPopMatrix();
}

void updateMovingObjects() {
    if (animationPaused) return;

    // Update students
    for (int i = 0; i < 5; i++) {
        if (students[i].pathRadius > 0) {
            // Circular motion
            students[i].angle += students[i].speed * 2;
            float centerX = (i == 0) ? -15 : (i == 1) ? 10 : 5;
            float centerZ = (i == 0) ? -10 : (i == 1) ? -20 : -15;
            students[i].x = centerX + cos(students[i].angle * M_PI / 180.0f) * students[i].pathRadius;
            students[i].z = centerZ + sin(students[i].angle * M_PI / 180.0f) * students[i].pathRadius;
        }
        else {
            // Linear motion
            students[i].z += students[i].speed;
            if (students[i].z > 15) {
                students[i].z = -50;
            }
        }
    }

    // Update cars on road
    for (int i = 0; i < 3; i++) {
        if (cars[i].angle == 0) {
            cars[i].z -= cars[i].speed;
            if (cars[i].z < -50) {
                cars[i].z = 40;
            }
        }
        else {
            cars[i].z += cars[i].speed;
            if (cars[i].z > 40) {
                cars[i].z = -50;
            }
        }
    }

    // Update bus
    busPos -= 0.08f;
    if (busPos < -50) {
        busPos = 50;
    }

    // Update birds
    birdPosX += 0.15f;
    birdPosZ += 0.05f;
    if (birdPosX > 50) {
        birdPosX = -50;
        birdPosZ = -30 + (rand() % 20);
    }
    birdWingAngle = sin(studentWalkCycle * 10) * 30;

    // Update fountain
    fountainHeight = abs(sin(studentWalkCycle * 2)) * 2;

    // Update windmill
    windmillAngle += 1.5f;

    // Update flag
    flagAngle += 0.08f;

    // Update clouds
    cloudPos += 0.08f;
    if (cloudPos > 60) {
        cloudPos = -60;
    }

    // Update walk cycle
    studentWalkCycle += 0.04f;
    if (studentWalkCycle > 2 * M_PI) {
        studentWalkCycle = 0;
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
        timeOfDay = (timeOfDay + 1) % 3;
        if (timeOfDay == 0) {
            glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
            std::cout << ">> Switched to DAY mode\n";
        }
        else if (timeOfDay == 1) {
            glClearColor(0.9f, 0.6f, 0.3f, 1.0f);
            std::cout << ">> Switched to SUNSET mode\n";
        }
        else {
            glClearColor(0.05f, 0.05f, 0.2f, 1.0f);
            std::cout << ">> Switched to NIGHT mode\n";
        }
        break;
    case 'p': case 'P':
        animationPaused = !animationPaused;
        std::cout << (animationPaused ? ">> Animations PAUSED\n" : ">> Animations RESUMED\n");
        break;
    case 'r': case 'R':
        camX = 0.0f; camY = 8.0f; camZ = 50.0f;
        camYaw = 0.0f; camPitch = 0.0f;
        std::cout << ">> Camera RESET\n";
        break;
    case 27: // ESC
        std::cout << "\n>> Exiting DIU Campus Simulation...\n";
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        camPitch += 3.0f;
        if (camPitch > 89.0f) camPitch = 89.0f;
        break;
    case GLUT_KEY_DOWN:
        camPitch -= 3.0f;
        if (camPitch < -89.0f) camPitch = -89.0f;
        break;
    case GLUT_KEY_LEFT:
        camYaw -= 3.0f;
        break;
    case GLUT_KEY_RIGHT:
        camYaw += 3.0f;
        break;
    case GLUT_KEY_PAGE_UP:
        camY += 0.8f;
        break;
    case GLUT_KEY_PAGE_DOWN:
        camY -= 0.8f;
        if (camY < 1.5f) camY = 1.5f;
        break;
    }
    glutPostRedisplay();
}

void timer(int value) {
    updateMovingObjects();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}