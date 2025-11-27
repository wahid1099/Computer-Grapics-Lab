#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <iostream>

// --- Global Variables ---
bool isDay = true;
float cloudX = 0.0f;
float cowX = 650.0f;        // Cow position
float cowHeadAngle = 0.0f;  // For grazing animation
bool grazingDown = true;
float doorAngle = 0.0f;
float treeScale = 1.0f;
bool treeGrowing = true;

// Animation variables
float person1ArmAngle = 0.0f;     // Person 1 arm wave animation
bool person1WavingUp = true;      // Animation direction flag for person 1
float person2X = 100.0f;          // Person 2 position for walking
float person2LegAngle = 0.0f;     // Person 2 walking animation
bool person2MovingRight = true;   // Walking direction flag for person 2

// --- Colors ---
void setSkyColor() {
    if (isDay) glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // Soft Day Blue
    else glClearColor(0.12f, 0.12f, 0.25f, 1.0f);      // Deep Midnight Blue
}

// --- ALGORITHM: Midpoint Circle Algorithm ---
void drawFilledCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        glBegin(GL_LINES);
        glVertex2i(xc - x, yc + y); glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc - y); glVertex2i(xc + x, yc - y);
        glVertex2i(xc - y, yc + x); glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc - x); glVertex2i(xc + y, yc - x);
        glEnd();
        x++;
        if (d > 0) { y--; d = d + 4 * (x - y) + 10; }
        else { d = d + 4 * x + 6; }
    }
}

// --- Drawing Helper: Rectangle ---
void drawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1); glVertex2f(x2, y1);
    glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
}

// --- UI Overlay ---
void drawUI() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    drawRect(0, 0, 1000, 60);
    glDisable(GL_BLEND);

    glColor3f(1.0f, 1.0f, 1.0f);
    const char* controls = "CONTROLS: [D] Day  [N] Night  [O] Open Door  [C] Close Door";
    const char* status = isDay ? "STATUS: Day Time" : "STATUS: Night Time";

    glRasterPos2f(20, 20);
    for (int i = 0; i < strlen(controls); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, controls[i]);

    glRasterPos2f(800, 20);
    for (int i = 0; i < strlen(status); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, status[i]);
}

// --- Objects ---

void drawCloudCluster(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    glColor3f(1.0f, 1.0f, 1.0f); // White
    if (!isDay) glColor3f(0.6f, 0.6f, 0.7f); // Grey at night

    // Draw a cluster of circles
    drawFilledCircle(0, 0, 30);
    drawFilledCircle(35, 10, 35);
    drawFilledCircle(70, 5, 30);
    drawFilledCircle(35, -15, 25);

    glPopMatrix();
}

void drawMountains() {
    // Colors change based on day/night
    float baseR = isDay ? 0.5f : 0.2f;
    float baseG = isDay ? 0.45f : 0.18f;
    float baseB = isDay ? 0.4f : 0.16f;
    float snowR = isDay ? 1.0f : 0.7f;

    // Mountain 1
    glColor3f(baseR, baseG, baseB);
    glBegin(GL_TRIANGLES); glVertex2f(0, 200); glVertex2f(200, 500); glVertex2f(400, 200); glEnd();
    glColor3f(snowR, snowR, snowR);
    glBegin(GL_TRIANGLES); glVertex2f(150, 450); glVertex2f(200, 500); glVertex2f(250, 450); glEnd();

    // Mountain 2
    glColor3f(baseR, baseG, baseB);
    glBegin(GL_TRIANGLES); glVertex2f(300, 200); glVertex2f(500, 450); glVertex2f(700, 200); glEnd();
    glColor3f(snowR, snowR, snowR);
    glBegin(GL_TRIANGLES); glVertex2f(450, 410); glVertex2f(500, 450); glVertex2f(550, 410); glEnd();

    // Mountain 3
    glColor3f(baseR, baseG, baseB);
    glBegin(GL_TRIANGLES); glVertex2f(600, 200); glVertex2f(800, 380); glVertex2f(1000, 200); glEnd();
    glColor3f(snowR, snowR, snowR);
    glBegin(GL_TRIANGLES); glVertex2f(760, 350); glVertex2f(800, 380); glVertex2f(840, 350); glEnd();
}

void drawPerson(float x, float y, float armAngle, float legAngle) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Legs
    glColor3f(0.2f, 0.3f, 0.5f); // Blue pants
    // Left Leg (rotates)
    glPushMatrix();
    glTranslatef(-5, 30, 0);
    glRotatef(legAngle, 0, 0, 1);
    glTranslatef(5, -30, 0);
    drawRect(-8, 0, -2, 30);
    glPopMatrix();

    // Right Leg (rotates opposite)
    glPushMatrix();
    glTranslatef(5, 30, 0);
    glRotatef(-legAngle, 0, 0, 1);
    glTranslatef(-5, -30, 0);
    drawRect(2, 0, 8, 30);
    glPopMatrix();

    // Body
    glColor3f(0.9f, 0.2f, 0.2f); // Red shirt
    drawRect(-12, 30, 12, 55);

    // Arms
    // Left arm (Waving arm)
    glPushMatrix();
    glTranslatef(-12, 50, 0);
    glRotatef(armAngle, 0, 0, 1); // Wave
    glTranslatef(12, -50, 0);
    glColor3f(0.9f, 0.2f, 0.2f);
    drawRect(-18, 35, -12, 50);
    glColor3f(0.95f, 0.76f, 0.65f); // Skin
    drawFilledCircle(-15, 33, 4);   // Hand
    glPopMatrix();

    // Right arm (Static)
    glColor3f(0.9f, 0.2f, 0.2f);
    drawRect(12, 35, 18, 50);
    glColor3f(0.95f, 0.76f, 0.65f);
    drawFilledCircle(15, 33, 4);

    // Head
    glColor3f(0.95f, 0.76f, 0.65f);
    drawFilledCircle(0, 65, 12);

    // Eyes
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(-4, 67, 2);
    drawFilledCircle(4, 67, 2);

    // Smile
    glBegin(GL_LINE_STRIP);
    glVertex2f(-4, 60); glVertex2f(0, 58); glVertex2f(4, 60);
    glEnd();

    glPopMatrix();
}

void drawCow(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    glColor3f(0.95f, 0.95f, 0.95f); // Body
    drawRect(0, 30, 80, 70);

    glColor3f(0.1f, 0.1f, 0.1f); // Spots
    drawFilledCircle(20, 50, 10);
    drawFilledCircle(55, 60, 8);

    glColor3f(0.9f, 0.9f, 0.9f); // Legs
    drawRect(5, 0, 15, 30); drawRect(65, 0, 75, 30);
    glColor3f(0.7f, 0.7f, 0.7f);
    drawRect(20, 5, 30, 30); drawRect(50, 5, 60, 30);

    // Head Animation
    glPushMatrix();
    glTranslatef(0, 60, 0);
    glRotatef(cowHeadAngle, 0, 0, 1);
    glTranslatef(0, -60, 0);

    glColor3f(0.95f, 0.95f, 0.95f);
    drawRect(-25, 50, 5, 75); // Head
    glColor3f(1.0f, 0.7f, 0.8f); // Nose
    drawRect(-25, 50, -15, 60);
    glColor3f(0.0f, 0.0f, 0.0f); // Eye
    drawFilledCircle(-15, 68, 2);

    glPopMatrix();
    glPopMatrix();
}

void drawGarden(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(0, 0, 200, 5); drawRect(0, 25, 200, 30);

    for (int i = 0; i <= 200; i += 20) {
        drawRect(i, 0, i + 10, 50);
        glBegin(GL_TRIANGLES); glVertex2f(i, 50); glVertex2f(i + 5, 60); glVertex2f(i + 10, 50); glEnd();
    }
    // Flowers
    for (int i = 15; i < 200; i += 35) {
        glColor3f(0.0f, 0.6f, 0.0f); drawRect(i, 0, i + 2, 35); // Stem
        if (i % 2 == 0) glColor3f(1.0f, 0.2f, 0.2f); else glColor3f(1.0f, 0.8f, 0.0f);
        drawFilledCircle(i + 1, 35, 8); // Petals
        glColor3f(1.0f, 1.0f, 1.0f); drawFilledCircle(i + 1, 35, 3); // Center
    }
    glPopMatrix();
}

void drawHouse() {
    glColor3f(0.92f, 0.90f, 0.85f); drawRect(300, 200, 700, 450); // Wall
    glColor3f(0.8f, 0.3f, 0.3f); // Roof
    glBegin(GL_QUADS); glVertex2f(280, 450); glVertex2f(720, 450); glVertex2f(650, 600); glVertex2f(350, 600); glEnd();

    // Windows
    glColor3f(0.4f, 0.2f, 0.1f); drawRect(340, 290, 460, 410); drawRect(540, 290, 660, 410);
    glColor3f(0.7f, 0.9f, 1.0f); if (!isDay) glColor3f(1.0f, 1.0f, 0.6f);
    drawRect(350, 300, 450, 400); drawRect(550, 300, 650, 400);

    // Door
    glPushMatrix();
    glTranslatef(500, 200, 0); glRotatef(doorAngle, 0, 1, 0); glTranslatef(-500, -200, 0);
    glColor3f(0.45f, 0.25f, 0.15f); drawRect(460, 200, 540, 350);
    glColor3f(0.9f, 0.8f, 0.2f); drawFilledCircle(525, 275, 4);
    glPopMatrix();
}

void drawSunMoon() {
    if (isDay) {
        glColor4f(1.0f, 1.0f, 0.0f, 0.3f); drawFilledCircle(100, 650, 55);
        glColor3f(1.0f, 0.9f, 0.0f); drawFilledCircle(100, 650, 40);
    }
    else {
        glColor3f(0.9f, 0.9f, 0.95f); drawFilledCircle(100, 650, 40);
        glColor3f(0.12f, 0.12f, 0.25f); drawFilledCircle(115, 665, 35);
    }
}

// --- Display Function ---
void display() {
    setSkyColor();
    glClear(GL_COLOR_BUFFER_BIT);

    // Ground
    glBegin(GL_QUADS);
    if (isDay) glColor3f(0.4f, 0.8f, 0.3f); else glColor3f(0.1f, 0.3f, 0.1f);
    glVertex2f(0, 200); glVertex2f(1000, 200);
    if (isDay) glColor3f(0.3f, 0.7f, 0.2f); else glColor3f(0.05f, 0.2f, 0.05f);
    glVertex2f(1000, 0); glVertex2f(0, 0);
    glEnd();

    drawMountains();
    drawSunMoon();

    // --- CLOUDS (Multiple clusters) ---
    glPushMatrix();
    glTranslatef(cloudX, 0, 0);
    drawCloudCluster(200, 600); // Low cloud
    drawCloudCluster(600, 650); // High cloud
    drawCloudCluster(900, 580); // Another low cloud
    glPopMatrix();

    drawHouse();
    drawGarden(750, 150);
    drawCow(cowX, 150);

    // --- PERSONS (ADDED HERE) ---
    // 1. Waving person standing by the house
    drawPerson(420, 150, person1ArmAngle, 0);

    // 2. Walking person moving left/right
    drawPerson(person2X, 120, 0, person2LegAngle);

    // Tree
    glPushMatrix();
    glTranslatef(150, 200, 0);
    glColor3f(0.55f, 0.27f, 0.07f); drawRect(-10, 0, 10, 120);
    glTranslatef(0, 120, 0); glScalef(treeScale, 1.0f, 1.0f);
    glColor3f(0.13f, 0.55f, 0.13f); if (!isDay) glColor3f(0.05f, 0.25f, 0.05f);
    drawFilledCircle(0, 0, 50);
    glPopMatrix();

    drawUI();

    glFlush();
    glutSwapBuffers();
}

// --- Logic ---
void timer(int value) {
    // Cloud Move
    cloudX += 0.8f;
    if (cloudX > 1100) cloudX = -300;

    // Tree Sway
    if (treeGrowing) { treeScale += 0.003f; if (treeScale >= 1.05f) treeGrowing = false; }
    else { treeScale -= 0.003f; if (treeScale <= 0.95f) treeGrowing = true; }

    // Cow Head
    if (grazingDown) { cowHeadAngle -= 0.5f; if (cowHeadAngle < -20.0f) grazingDown = false; }
    else { cowHeadAngle += 0.5f; if (cowHeadAngle > 0.0f) grazingDown = true; }

    // Person 1 Wave
    if (person1WavingUp) { person1ArmAngle += 2.0f; if (person1ArmAngle >= 45.0f) person1WavingUp = false; }
    else { person1ArmAngle -= 2.0f; if (person1ArmAngle <= -10.0f) person1WavingUp = true; }

    // Person 2 Walking
    if (person2MovingRight) {
        person2X += 1.0f;
        person2LegAngle += 2.0f; // Leg swing
        if (person2LegAngle > 20) person2LegAngle = -20; // Reset fake cycle
        if (person2X >= 250.0f) person2MovingRight = false;
    }
    else {
        person2X -= 1.0f;
        person2LegAngle -= 2.0f;
        if (person2LegAngle < -20) person2LegAngle = 20;
        if (person2X <= 50.0f) person2MovingRight = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'd': case 'D': isDay = true; break;
    case 'n': case 'N': isDay = false; break;
    case 'o': case 'O': doorAngle = -85.0f; break;
    case 'c': case 'C': doorAngle = 0.0f; break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(1000, 750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Modern Farmhouse with People - OpenGL");
    gluOrtho2D(0, 1000, 0, 750);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}