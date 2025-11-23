#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

// === CAMERA & VIEW ===
float camX = 0.0f, camY = 25.0f, camZ = 60.0f;
float camYaw = 0.0f, camPitch = -15.0f;
float moveSpeed = 0.5f;

// === TIME & LIGHTING ===
int timeOfDay = 0; // 0=day, 1=sunset, 2=night
float skyR = 0.53f, skyG = 0.81f, skyB = 0.92f;

// === TRAFFIC SIGNAL STATE ===
enum SignalState { RED, YELLOW, GREEN };
SignalState mainRoadSignal = GREEN;
SignalState sideRoadSignal = RED;
float signalTimer = 0.0f;
float signalDurations[3] = { 5.0f, 2.0f, 5.0f }; // Red, Yellow, Green durations (seconds)
bool manualMode = false;

// === ANIMATION ===
bool animationPaused = false;
float globalTime = 0.0f;
float cloudPos = -80.0f;
float pedestrianWalk = 0.0f;
int trafficDensity = 1; // 0=light, 1=medium, 2=heavy

// === VEHICLE STRUCTURE ===
struct Vehicle {
    float x, z, speed, length, width;
    float r, g, b;
    int lane; // 0=main road horizontal, 1=side road vertical
    bool waiting;
    int direction; // 0=right, 1=left, 2=up, 3=down
};

std::vector<Vehicle> vehicles;

// === PEDESTRIAN STRUCTURE ===
struct Pedestrian {
    float x, z, speed, walkCycle;
    bool crossing;
    int crossingPoint; // 0=north, 1=south, 2=east, 3=west
};

std::vector<Pedestrian> pedestrians;

// === STATISTICS ===
int carsPassedMainRoad = 0;
int carPassedSideRoad = 0;
int totalWaitTime = 0;

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
void drawIntersection();
void drawTrafficSignal(float x, float z, SignalState state, float rotation);
void drawPedestrianSignal(float x, float z, bool canWalk);
void drawVehicle(Vehicle& v);
void drawPedestrian(Pedestrian& p);
void drawBuildings();
void drawTrees();
void drawStreetLights();
void drawClouds();
void drawSun();
void drawMoon();
void drawStars();
void drawRoadMarkings();
void drawSidewalk();
void drawBusStop(float x, float z);
void drawParkingArea();
void updateTrafficSignals();
void updateVehicles();
void updatePedestrians();
void spawnVehicle();
void spawnPedestrian();
void checkCollisions();
void displayStats();
void drawText3D(float x, float y, float z, const char* text);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1400, 800);
    glutCreateWindow("🚦 Advanced Traffic Signal Simulation - OpenGL");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, timer, 0);

    std::cout << "\n╔═══════════════════════════════════════════╗\n";
    std::cout << "║   TRAFFIC SIGNAL SIMULATION - CONTROLS    ║\n";
    std::cout << "╠═══════════════════════════════════════════╣\n";
    std::cout << "║ W/A/S/D    - Move camera                  ║\n";
    std::cout << "║ Arrow Keys - Look around                  ║\n";
    std::cout << "║ N          - Cycle Day/Sunset/Night       ║\n";
    std::cout << "║ M          - Toggle Manual/Auto signals   ║\n";
    std::cout << "║ SPACE      - Manual signal change         ║\n";
    std::cout << "║ T          - Change traffic density       ║\n";
    std::cout << "║ P          - Pause/Resume                 ║\n";
    std::cout << "║ R          - Reset camera                 ║\n";
    std::cout << "║ +/-        - Adjust simulation speed      ║\n";
    std::cout << "║ ESC        - Exit                         ║\n";
    std::cout << "╚═══════════════════════════════════════════╝\n\n";

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
    glShadeModel(GL_SMOOTH);

    glClearColor(skyR, skyG, skyB, 1.0f);
    setupLighting();

    // Spawn initial vehicles
    for (int i = 0; i < 5; i++) {
        spawnVehicle();
    }

    // Spawn initial pedestrians
    for (int i = 0; i < 4; i++) {
        spawnPedestrian();
    }
}

void setupLighting() {
    GLfloat ambientLight[4], diffuseLight[4], specularLight[4];

    if (timeOfDay == 0) { // Day
        ambientLight[0] = 0.6f; ambientLight[1] = 0.6f; ambientLight[2] = 0.6f; ambientLight[3] = 1.0f;
        diffuseLight[0] = 1.0f; diffuseLight[1] = 1.0f; diffuseLight[2] = 0.95f; diffuseLight[3] = 1.0f;
    }
    else if (timeOfDay == 1) { // Sunset
        ambientLight[0] = 0.5f; ambientLight[1] = 0.3f; ambientLight[2] = 0.2f; ambientLight[3] = 1.0f;
        diffuseLight[0] = 1.0f; diffuseLight[1] = 0.6f; diffuseLight[2] = 0.3f; diffuseLight[3] = 1.0f;
    }
    else { // Night
        ambientLight[0] = 0.15f; ambientLight[1] = 0.15f; ambientLight[2] = 0.25f; ambientLight[3] = 1.0f;
        diffuseLight[0] = 0.4f; diffuseLight[1] = 0.4f; diffuseLight[2] = 0.6f; diffuseLight[3] = 1.0f;
    }

    specularLight[0] = 0.5f; specularLight[1] = 0.5f; specularLight[2] = 0.5f; specularLight[3] = 1.0f;

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    GLfloat lightPosition[] = { 30.0f, 80.0f, 30.0f, 1.0f };
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

    // === SKY ELEMENTS ===
    if (timeOfDay == 0) drawSun();
    else if (timeOfDay == 2) {
        drawMoon();
        drawStars();
    }
    drawClouds();

    // === GROUND & ROADS ===
    drawGround();
    drawSidewalk();
    drawRoads();
    drawRoadMarkings();
    drawIntersection();

    // === INFRASTRUCTURE ===
    drawBuildings();
    drawTrees();
    drawStreetLights();
    drawBusStop(25, 15);
    drawBusStop(-25, -15);
    drawParkingArea();

    // === TRAFFIC SIGNALS ===
    drawTrafficSignal(12, 12, mainRoadSignal, 0);
    drawTrafficSignal(-12, -12, mainRoadSignal, 180);
    drawTrafficSignal(12, -12, sideRoadSignal, 90);
    drawTrafficSignal(-12, 12, sideRoadSignal, 270);

    // === PEDESTRIAN SIGNALS ===
    drawPedestrianSignal(10, 10, mainRoadSignal == RED);
    drawPedestrianSignal(-10, -10, mainRoadSignal == RED);
    drawPedestrianSignal(10, -10, sideRoadSignal == RED);
    drawPedestrianSignal(-10, 10, sideRoadSignal == RED);

    // === MOVING OBJECTS ===
    for (auto& v : vehicles) {
        drawVehicle(v);
    }

    for (auto& p : pedestrians) {
        drawPedestrian(p);
    }

    // === STATISTICS ===
    displayStats();

    glutSwapBuffers();
}

void drawGround() {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.5f, 0.2f);
    glVertex3f(-100, 0, 100);
    glVertex3f(100, 0, 100);
    glVertex3f(100, 0, -100);
    glVertex3f(-100, 0, -100);
    glEnd();
}

void drawSidewalk() {
    glColor3f(0.7f, 0.7f, 0.7f);

    // Main road sidewalks
    glBegin(GL_QUADS);
    glVertex3f(-100, 0.01f, 12);
    glVertex3f(100, 0.01f, 12);
    glVertex3f(100, 0.01f, 15);
    glVertex3f(-100, 0.01f, 15);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-100, 0.01f, -15);
    glVertex3f(100, 0.01f, -15);
    glVertex3f(100, 0.01f, -12);
    glVertex3f(-100, 0.01f, -12);
    glEnd();

    // Side road sidewalks
    glBegin(GL_QUADS);
    glVertex3f(12, 0.01f, -100);
    glVertex3f(15, 0.01f, -100);
    glVertex3f(15, 0.01f, 100);
    glVertex3f(12, 0.01f, 100);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-15, 0.01f, -100);
    glVertex3f(-12, 0.01f, -100);
    glVertex3f(-12, 0.01f, 100);
    glVertex3f(-15, 0.01f, 100);
    glEnd();
}

void drawRoads() {
    // Main horizontal road
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex3f(-100, 0.02f, 12);
    glVertex3f(100, 0.02f, 12);
    glVertex3f(100, 0.02f, -12);
    glVertex3f(-100, 0.02f, -12);
    glEnd();

    // Side vertical road
    glBegin(GL_QUADS);
    glVertex3f(12, 0.02f, -100);
    glVertex3f(-12, 0.02f, -100);
    glVertex3f(-12, 0.02f, 100);
    glVertex3f(12, 0.02f, 100);
    glEnd();
}

void drawRoadMarkings() {
    glColor3f(1.0f, 1.0f, 0.9f);

    // Main road lane divider
    for (int i = -20; i < 20; i++) {
        if (abs(i * 5) < 13) continue; // Skip intersection
        glBegin(GL_QUADS);
        glVertex3f(i * 5, 0.03f, 0.3f);
        glVertex3f(i * 5 + 3, 0.03f, 0.3f);
        glVertex3f(i * 5 + 3, 0.03f, -0.3f);
        glVertex3f(i * 5, 0.03f, -0.3f);
        glEnd();
    }

    // Side road lane divider
    for (int i = -20; i < 20; i++) {
        if (abs(i * 5) < 13) continue;
        glBegin(GL_QUADS);
        glVertex3f(0.3f, 0.03f, i * 5);
        glVertex3f(-0.3f, 0.03f, i * 5);
        glVertex3f(-0.3f, 0.03f, i * 5 + 3);
        glVertex3f(0.3f, 0.03f, i * 5 + 3);
        glEnd();
    }

    // Zebra crossings
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 8; i++) {
        // North crossing
        glBegin(GL_QUADS);
        glVertex3f(-8 + i * 2, 0.04f, 14);
        glVertex3f(-7 + i * 2, 0.04f, 14);
        glVertex3f(-7 + i * 2, 0.04f, 12);
        glVertex3f(-8 + i * 2, 0.04f, 12);
        glEnd();

        // South crossing
        glBegin(GL_QUADS);
        glVertex3f(-8 + i * 2, 0.04f, -12);
        glVertex3f(-7 + i * 2, 0.04f, -12);
        glVertex3f(-7 + i * 2, 0.04f, -14);
        glVertex3f(-8 + i * 2, 0.04f, -14);
        glEnd();

        // East crossing
        glBegin(GL_QUADS);
        glVertex3f(12, 0.04f, -8 + i * 2);
        glVertex3f(14, 0.04f, -8 + i * 2);
        glVertex3f(14, 0.04f, -7 + i * 2);
        glVertex3f(12, 0.04f, -7 + i * 2);
        glEnd();

        // West crossing
        glBegin(GL_QUADS);
        glVertex3f(-14, 0.04f, -8 + i * 2);
        glVertex3f(-12, 0.04f, -8 + i * 2);
        glVertex3f(-12, 0.04f, -7 + i * 2);
        glVertex3f(-14, 0.04f, -7 + i * 2);
        glEnd();
    }
}

void drawIntersection() {
    // Intersection center - slightly different color
    glBegin(GL_QUADS);
    glColor3f(0.25f, 0.25f, 0.25f);
    glVertex3f(-12, 0.025f, 12);
    glVertex3f(12, 0.025f, 12);
    glVertex3f(12, 0.025f, -12);
    glVertex3f(-12, 0.025f, -12);
    glEnd();
}

void drawTrafficSignal(float x, float z, SignalState state, float rotation) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glRotatef(rotation, 0, 1, 0);

    // Pole
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 4, 0);
    glScalef(0.3f, 8, 0.3f);
    glutSolidCube(1);
    glPopMatrix();

    // Signal box
    glPushMatrix();
    glTranslatef(0, 8, 0);
    glColor3f(0.2f, 0.2f, 0.2f);
    glScalef(0.8f, 2.5f, 0.6f);
    glutSolidCube(1);
    glPopMatrix();

    // Red light
    glPushMatrix();
    glTranslatef(0, 9, 0.35f);
    if (state == RED) {
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else {
        glColor3f(0.3f, 0.0f, 0.0f);
    }
    glutSolidSphere(0.35f, 20, 20);
    glPopMatrix();

    // Yellow light
    glPushMatrix();
    glTranslatef(0, 8, 0.35f);
    if (state == YELLOW) {
        glColor3f(1.0f, 1.0f, 0.0f);
    }
    else {
        glColor3f(0.3f, 0.3f, 0.0f);
    }
    glutSolidSphere(0.35f, 20, 20);
    glPopMatrix();

    // Green light
    glPushMatrix();
    glTranslatef(0, 7, 0.35f);
    if (state == GREEN) {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else {
        glColor3f(0.0f, 0.3f, 0.0f);
    }
    glutSolidSphere(0.35f, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

void drawPedestrianSignal(float x, float z, bool canWalk) {
    glPushMatrix();
    glTranslatef(x, 3, z);

    // Signal box
    glColor3f(0.2f, 0.2f, 0.2f);
    glScalef(0.6f, 1.2f, 0.3f);
    glutSolidCube(1);
    glPopMatrix();

    // Walk/Don't Walk indicator
    glPushMatrix();
    glTranslatef(x, 3, z + 0.2f);
    if (canWalk) {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    else {
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    glutSolidSphere(0.25f, 15, 15);
    glPopMatrix();
}

void drawVehicle(Vehicle& v) {
    glPushMatrix();
    glTranslatef(v.x, 0, v.z);

    // Rotate based on direction
    if (v.direction == 0) glRotatef(0, 0, 1, 0);      // Right
    else if (v.direction == 1) glRotatef(180, 0, 1, 0); // Left
    else if (v.direction == 2) glRotatef(90, 0, 1, 0);  // Up
    else glRotatef(270, 0, 1, 0);                        // Down

    // Body
    glPushMatrix();
    glColor3f(v.r, v.g, v.b);
    glTranslatef(0, 0.8f, 0);
    glScalef(v.length, 1.2f, v.width);
    glutSolidCube(1);
    glPopMatrix();

    // Top/Cabin
    glPushMatrix();
    glColor3f(v.r * 0.8f, v.g * 0.8f, v.b * 0.8f);
    glTranslatef(-0.3f, 1.8f, 0);
    glScalef(v.length * 0.6f, 0.8f, v.width * 0.85f);
    glutSolidCube(1);
    glPopMatrix();

    // Windows
    glColor3f(0.3f, 0.5f, 0.7f);
    glPushMatrix();
    glTranslatef(-0.3f, 1.8f, v.width * 0.45f);
    glScalef(v.length * 0.55f, 0.7f, 0.05f);
    glutSolidCube(1);
    glPopMatrix();

    // Headlights
    if (timeOfDay == 2) {
        glColor3f(1.0f, 1.0f, 0.9f);
    }
    else {
        glColor3f(0.9f, 0.9f, 0.8f);
    }
    glPushMatrix();
    glTranslatef(v.length * 0.52f, 0.6f, v.width * 0.3f);
    glutSolidSphere(0.15f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(v.length * 0.52f, 0.6f, -v.width * 0.3f);
    glutSolidSphere(0.15f, 10, 10);
    glPopMatrix();

    // Wheels
    glColor3f(0.1f, 0.1f, 0.1f);
    float wheelPositions[4][2] = {
        {v.length * 0.35f, v.width * 0.55f},
        {v.length * 0.35f, -v.width * 0.55f},
        {-v.length * 0.35f, v.width * 0.55f},
        {-v.length * 0.35f, -v.width * 0.55f}
    };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(wheelPositions[i][0], 0.4f, wheelPositions[i][1]);
        glRotatef(90, 0, 0, 1);
        glutSolidTorus(0.1, 0.3, 10, 10);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawPedestrian(Pedestrian& p) {
    glPushMatrix();
    glTranslatef(p.x, 0, p.z);

    // Body
    glPushMatrix();
    glColor3f(0.3f, 0.4f, 0.7f);
    glTranslatef(0, 1.2f, 0);
    glScalef(0.5f, 1.0f, 0.3f);
    glutSolidCube(1);
    glPopMatrix();

    // Head
    glPushMatrix();
    glColor3f(0.9f, 0.7f, 0.5f);
    glTranslatef(0, 2.0f, 0);
    glutSolidSphere(0.3f, 15, 15);
    glPopMatrix();

    // Legs (animated)
    float legSwing = sin(p.walkCycle * 8) * 0.25f;

    glColor3f(0.2f, 0.2f, 0.4f);
    glPushMatrix();
    glTranslatef(-0.15f, 0.5f, legSwing);
    glScalef(0.2f, 1.0f, 0.2f);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f, 0.5f, -legSwing);
    glScalef(0.2f, 1.0f, 0.2f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}

void drawBuildings() {
    // Building function
    auto drawBuilding = [](float x, float z, float width, float height, float depth,
        float r, float g, float b) {
            glPushMatrix();
            glColor3f(r, g, b);
            glTranslatef(x, height / 2, z);
            glScalef(width, height, depth);
            glutSolidCube(1);
            glPopMatrix();

            // Windows
            glColor3f(0.4f, 0.6f, 0.8f);
            int floors = (int)(height / 3);
            int windowsPerFloor = (int)(width / 2.5f);

            for (int floor = 0; floor < floors; floor++) {
                for (int win = 0; win < windowsPerFloor; win++) {
                    glPushMatrix();
                    glTranslatef(x - width / 2 + 1 + win * 2.5f, 2 + floor * 3, z + depth / 2 + 0.1f);
                    if (timeOfDay == 2 && (rand() % 3 == 0)) {
                        glColor3f(1.0f, 1.0f, 0.7f);
                    }
                    else {
                        glColor3f(0.4f, 0.6f, 0.8f);
                    }
                    glScalef(1.0f, 1.2f, 0.1f);
                    glutSolidCube(1);
                    glPopMatrix();
                }
            }
        };

    // North buildings
    drawBuilding(-40, 25, 15, 20, 10, 0.8f, 0.85f, 0.9f);
    drawBuilding(-20, 25, 12, 25, 8, 0.85f, 0.8f, 0.85f);
    drawBuilding(0, 25, 18, 18, 12, 0.75f, 0.8f, 0.9f);
    drawBuilding(25, 25, 14, 22, 10, 0.8f, 0.82f, 0.88f);
    drawBuilding(45, 25, 16, 28, 11, 0.78f, 0.84f, 0.9f);

    // South buildings
    drawBuilding(-40, -25, 14, 24, 9, 0.82f, 0.86f, 0.9f);
    drawBuilding(-20, -25, 16, 20, 10, 0.8f, 0.83f, 0.88f);
    drawBuilding(5, -25, 15, 26, 11, 0.85f, 0.82f, 0.87f);
    drawBuilding(28, -25, 18, 22, 13, 0.77f, 0.85f, 0.9f);
    drawBuilding(50, -25, 12, 30, 8, 0.8f, 0.8f, 0.9f);

    // East buildings
    drawBuilding(25, -5, 8, 18, 8, 0.83f, 0.85f, 0.88f);
    drawBuilding(25, 5, 10, 21, 9, 0.8f, 0.84f, 0.9f);

    // West buildings
    drawBuilding(-25, -5, 9, 19, 7, 0.81f, 0.83f, 0.89f);
    drawBuilding(-25, 5, 11, 23, 10, 0.79f, 0.85f, 0.87f);
}

void drawTrees() {
    auto drawTree = [](float x, float z) {
        // Trunk
        glPushMatrix();
        glColor3f(0.4f, 0.25f, 0.15f);
        glTranslatef(x, 2, z);
        glScalef(0.5f, 4, 0.5f);
        glutSolidCube(1);
        glPopMatrix();

        // Leaves
        glPushMatrix();
        glColor3f(0.15f, 0.6f, 0.2f);
        glTranslatef(x, 5, z);
        glutSolidSphere(2, 15, 15);
        glPopMatrix();
        };

    // Trees along roads
    for (int i = 0; i < 8; i++) {
        drawTree(-40 + i * 12, 17);
        drawTree(-40 + i * 12, -17);
    }

    for (int i = 0; i < 6; i++) {
        drawTree(17, -30 + i * 12);
        drawTree(-17, -30 + i * 12);
    }
}

void drawStreetLights() {
    bool lightsOn = (timeOfDay == 2);

    auto drawLight = [lightsOn](float x, float z) {
        // Pole
        glPushMatrix();
        glColor3f(0.25f, 0.25f, 0.25f);
        glTranslatef(x, 4, z);
        glScalef(0.3f, 8, 0.3f);
        glutSolidCube(1);
        glPopMatrix();

        // Light
        glPushMatrix();
        if (lightsOn) {
            glColor3f(1.0f, 1.0f, 0.8f);
        }
        else {
            glColor3f(0.8f, 0.8f, 0.8f);
        }
        glTranslatef(x, 8, z);
        glutSolidSphere(0.8f, 20, 20);
        glPopMatrix();

        // Light beam at night
        if (lightsOn) {
            glPushMatrix();
            glColor4f(1.0f, 1.0f, 0.6f, 0.3f);
            glTranslatef(x, 6, z);
            glRotatef(90, 1, 0, 0);
            glutSolidCone(3, 6, 20, 20);
            glPopMatrix();
        }
        };

    // Street lights at intersection
    drawLight(15, 15);
    drawLight(-15, 15);
    drawLight(15, -15);
    drawLight(-15, -15);

    // Additional street lights
    for (int i = -3; i <= 3; i++) {
        if (i == 0) continue;
        drawLight(i * 20, 15);
        drawLight(i * 20, -15);
        drawLight(15, i * 20);
        drawLight(-15, i * 20);
    }
}

void drawBusStop(float x, float z) {
    // Shelter roof
    glPushMatrix();
    glColor3f(0.6f, 0.6f, 0.7f);
    glTranslatef(x, 3, z);
    glScalef(4, 0.2f, 2);
    glutSolidCube(1);
    glPopMatrix();

    // Support poles
    glColor3f(0.3f, 0.3f, 0.3f);
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(x + (i * 3 - 1.5f), 1.5f, z);
        glScalef(0.2f, 3, 0.2f);
        glutSolidCube(1);
        glPopMatrix();
    }

    // Bench
    glPushMatrix();
    glColor3f(0.5f, 0.3f, 0.2f);
    glTranslatef(x, 0.6f, z);
    glScalef(3, 0.2f, 1);
    glutSolidCube(1);
    glPopMatrix();

    // Sign
    glPushMatrix();
    glColor3f(0.2f, 0.4f, 0.8f);
    glTranslatef(x + 2.5f, 2, z);
    glScalef(0.1f, 1, 0.8f);
    glutSolidCube(1);
    glPopMatrix();
}

void drawParkingArea() {
    // Parking lot
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.35f);
    glVertex3f(30, 0.01f, 15);
    glVertex3f(45, 0.01f, 15);
    glVertex3f(45, 0.01f, 25);
    glVertex3f(30, 0.01f, 25);
    glEnd();

    // Parking lines
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 5; i++) {
        glBegin(GL_LINE_LOOP);
        glVertex3f(31 + i * 3, 0.02f, 16);
        glVertex3f(33 + i * 3, 0.02f, 16);
        glVertex3f(33 + i * 3, 0.02f, 24);
        glVertex3f(31 + i * 3, 0.02f, 24);
        glEnd();
    }
}

void drawSun() {
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.3f);
    glTranslatef(60, 50, -40);
    glutSolidSphere(8, 30, 30);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawMoon() {
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3f(0.9f, 0.9f, 1.0f);
    glTranslatef(-50, 50, -40);
    glutSolidSphere(6, 30, 30);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawStars() {
    glDisable(GL_LIGHTING);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < 100; i++) {
        float x = -80 + (rand() % 160);
        float y = 30 + (rand() % 40);
        float z = -80 + (rand() % 80);
        glVertex3f(x, y, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawClouds() {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Cloud 1
    glPushMatrix();
    glTranslatef(cloudPos, 35, -30);
    glutSolidSphere(3, 15, 15);
    glTranslatef(4, 0, 0);
    glutSolidSphere(3.5, 15, 15);
    glTranslatef(4, 0, 0);
    glutSolidSphere(3, 15, 15);
    glPopMatrix();

    // Cloud 2
    glPushMatrix();
    glTranslatef(cloudPos - 40, 38, -20);
    glutSolidSphere(2.5, 15, 15);
    glTranslatef(3, 0, 0);
    glutSolidSphere(3, 15, 15);
    glTranslatef(3, 0, 0);
    glutSolidSphere(2.5, 15, 15);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void displayStats() {
    // This would display statistics as text overlay
    // For now, we'll just print to console periodically
}

void updateTrafficSignals() {
    if (manualMode || animationPaused) return;

    signalTimer += 0.016f; // 16ms per frame

    if (mainRoadSignal == GREEN) {
        if (signalTimer >= signalDurations[2]) {
            mainRoadSignal = YELLOW;
            signalTimer = 0;
        }
    }
    else if (mainRoadSignal == YELLOW) {
        if (signalTimer >= signalDurations[1]) {
            mainRoadSignal = RED;
            sideRoadSignal = GREEN;
            signalTimer = 0;
            std::cout << "🚦 Signal Changed: Main Road RED | Side Road GREEN\n";
        }
    }
    else { // RED
        if (signalTimer >= signalDurations[0]) {
            sideRoadSignal = YELLOW;
            signalTimer = 0;
        }
    }

    if (sideRoadSignal == YELLOW) {
        if (signalTimer >= signalDurations[1]) {
            sideRoadSignal = RED;
            mainRoadSignal = GREEN;
            signalTimer = 0;
            std::cout << "🚦 Signal Changed: Main Road GREEN | Side Road RED\n";
        }
    }
}

void updateVehicles() {
    if (animationPaused) return;

    for (auto& v : vehicles) {
        v.waiting = false;

        // Check if vehicle needs to stop at signal
        if (v.direction == 0) { // Moving right
            if (v.x > -15 && v.x < -8 && mainRoadSignal != GREEN) {
                v.waiting = true;
            }
        }
        else if (v.direction == 1) { // Moving left
            if (v.x < 15 && v.x > 8 && mainRoadSignal != GREEN) {
                v.waiting = true;
            }
        }
        else if (v.direction == 2) { // Moving up
            if (v.z < 15 && v.z > 8 && sideRoadSignal != GREEN) {
                v.waiting = true;
            }
        }
        else { // Moving down
            if (v.z > -15 && v.z < -8 && sideRoadSignal != GREEN) {
                v.waiting = true;
            }
        }

        // Move vehicle if not waiting
        if (!v.waiting) {
            if (v.direction == 0) v.x += v.speed;
            else if (v.direction == 1) v.x -= v.speed;
            else if (v.direction == 2) v.z += v.speed;
            else v.z -= v.speed;
        }

        // Remove vehicles that are off screen
        if (v.x > 80 || v.x < -80 || v.z > 80 || v.z < -80) {
            v.x = (rand() % 2 == 0) ? -70 : 70;
            v.z = (rand() % 2 == 0) ? -70 : 70;
            v.direction = rand() % 4;

            // Update position based on direction
            if (v.direction == 0) { v.x = -70; v.z = (rand() % 2 == 0) ? 6 : -6; }
            else if (v.direction == 1) { v.x = 70; v.z = (rand() % 2 == 0) ? 6 : -6; }
            else if (v.direction == 2) { v.z = -70; v.x = (rand() % 2 == 0) ? 6 : -6; }
            else { v.z = 70; v.x = (rand() % 2 == 0) ? 6 : -6; }
        }
    }
}

void updatePedestrians() {
    if (animationPaused) return;

    for (auto& p : pedestrians) {
        p.walkCycle += 0.05f;

        // Simple pedestrian movement
        if (!p.crossing) {
            // Check if can cross
            if ((p.crossingPoint < 2 && mainRoadSignal == RED) ||
                (p.crossingPoint >= 2 && sideRoadSignal == RED)) {
                p.crossing = true;
            }
        }
        else {
            // Move across
            if (p.crossingPoint == 0) p.z -= p.speed;
            else if (p.crossingPoint == 1) p.z += p.speed;
            else if (p.crossingPoint == 2) p.x += p.speed;
            else p.x -= p.speed;

            // Check if crossed
            if (abs(p.x) > 15 || abs(p.z) > 15) {
                p.crossing = false;
                // Reset position
                p.crossingPoint = rand() % 4;
                if (p.crossingPoint == 0) { p.x = rand() % 10 - 5; p.z = 20; }
                else if (p.crossingPoint == 1) { p.x = rand() % 10 - 5; p.z = -20; }
                else if (p.crossingPoint == 2) { p.x = -20; p.z = rand() % 10 - 5; }
                else { p.x = 20; p.z = rand() % 10 - 5; }
            }
        }
    }
}

void spawnVehicle() {
    Vehicle v;
    v.length = 3.5f + (rand() % 10) * 0.1f;
    v.width = 2.0f + (rand() % 5) * 0.1f;
    v.speed = 0.15f + (rand() % 10) * 0.01f;
    v.r = (rand() % 100) / 100.0f;
    v.g = (rand() % 100) / 100.0f;
    v.b = (rand() % 100) / 100.0f;
    v.direction = rand() % 4;
    v.waiting = false;

    if (v.direction == 0) { v.x = -70; v.z = (rand() % 2 == 0) ? 6 : -6; }
    else if (v.direction == 1) { v.x = 70; v.z = (rand() % 2 == 0) ? 6 : -6; }
    else if (v.direction == 2) { v.z = -70; v.x = (rand() % 2 == 0) ? 6 : -6; }
    else { v.z = 70; v.x = (rand() % 2 == 0) ? 6 : -6; }

    vehicles.push_back(v);
}

void spawnPedestrian() {
    Pedestrian p;
    p.speed = 0.05f + (rand() % 5) * 0.01f;
    p.walkCycle = 0;
    p.crossing = false;
    p.crossingPoint = rand() % 4;

    if (p.crossingPoint == 0) { p.x = rand() % 10 - 5; p.z = 15; }
    else if (p.crossingPoint == 1) { p.x = rand() % 10 - 5; p.z = -15; }
    else if (p.crossingPoint == 2) { p.x = -15; p.z = rand() % 10 - 5; }
    else { p.x = 15; p.z = rand() % 10 - 5; }

    pedestrians.push_back(p);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 0.1, 300.0);
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
            std::cout << "☀️  DAY Mode\n";
        }
        else if (timeOfDay == 1) {
            glClearColor(0.9f, 0.6f, 0.3f, 1.0f);
            std::cout << "🌅 SUNSET Mode\n";
        }
        else {
            glClearColor(0.05f, 0.05f, 0.2f, 1.0f);
            std::cout << "🌙 NIGHT Mode\n";
        }
        break;
    case 'm': case 'M':
        manualMode = !manualMode;
        std::cout << (manualMode ? "📋 MANUAL Signal Control\n" : "🤖 AUTO Signal Control\n");
        break;
    case ' ':
        if (manualMode) {
            if (mainRoadSignal == GREEN) {
                mainRoadSignal = RED;
                sideRoadSignal = GREEN;
            }
            else {
                mainRoadSignal = GREEN;
                sideRoadSignal = RED;
            }
            std::cout << "🚦 Signal Manually Changed\n";
        }
        break;
    case 't': case 'T':
        trafficDensity = (trafficDensity + 1) % 3;
        std::cout << "🚗 Traffic Density: " << (trafficDensity == 0 ? "LIGHT" : trafficDensity == 1 ? "MEDIUM" : "HEAVY") << "\n";
        break;
    case 'p': case 'P':
        animationPaused = !animationPaused;
        std::cout << (animationPaused ? "⏸️  PAUSED\n" : "▶️  RESUMED\n");
        break;
    case 'r': case 'R':
        camX = 0.0f; camY = 25.0f; camZ = 60.0f;
        camYaw = 0.0f; camPitch = -15.0f;
        std::cout << "🎥 Camera RESET\n";
        break;
    case '+': case '=':
        moveSpeed += 0.1f;
        std::cout << "⚡ Speed increased: " << moveSpeed << "\n";
        break;
    case '-': case '_':
        moveSpeed -= 0.1f;
        if (moveSpeed < 0.1f) moveSpeed = 0.1f;
        std::cout << "🐌 Speed decreased: " << moveSpeed << "\n";
        break;
    case 27: // ESC
        std::cout << "\n👋 Exiting Traffic Simulation...\n";
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
        camY += 1.0f;
        break;
    case GLUT_KEY_PAGE_DOWN:
        camY -= 1.0f;
        if (camY < 2.0f) camY = 2.0f;
        break;
    }
    glutPostRedisplay();
}

void timer(int value) {
    if (!animationPaused) {
        globalTime += 0.016f;
        cloudPos += 0.1f;
        if (cloudPos > 100) cloudPos = -100;

        updateTrafficSignals();
        updateVehicles();
        updatePedestrians();

        // Spawn new vehicles based on traffic density
        if ((int)(globalTime * 60) % (30 - trafficDensity * 10) == 0) {
            if (vehicles.size() < 20) {
                spawnVehicle();
            }
        }

        // Spawn pedestrians occasionally
        if ((int)(globalTime * 60) % 180 == 0) {
            if (pedestrians.size() < 12) {
                spawnPedestrian();
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}