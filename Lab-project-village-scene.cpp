#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <cstdlib>
#include <ctime>

// --- Global Variables ---
bool isDay = true;
float cloudX = 0.0f;
float cowX = 650.0f;
float cowHeadAngle = 0.0f;
bool grazingDown = true;

float doorAngle = 0.0f;
float treeScale = 1.0f;
bool treeGrowing = true;

// People
float person1ArmAngle = 0.0f;
bool person1WavingUp = true;
float person2X = 100.0f;
float person2LegAngle = 0.0f;
bool person2MovingRight = true;

// New features
float windmillAngle = 0.0f;
float birdFlap = 0.0f;
float butterflyX = 820.0f, butterflyY = 250.0f;
float butterflyWing = 0.0f;

// Sheep animation
float sheep1X = 300.0f;
float sheep2X = 380.0f;
bool sheep1MovingRight = true;

// Smoke & Stars
std::vector<float> smokeY;
struct Star { float x, y, phase; };
std::vector<Star> stars;

// --- Your Original Midpoint Circle ---
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
        else d = d + 4 * x + 6;
    }
}

void drawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1); glVertex2f(x2, y1);
    glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
}

void drawCloudCluster(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(1.0f, 1.0f, 1.0f);
    if (!isDay) glColor3f(0.6f, 0.6f, 0.7f);
    drawFilledCircle(0, 0, 30);
    drawFilledCircle(35, 10, 35);
    drawFilledCircle(70, 5, 30);
    drawFilledCircle(35, -15, 25);
    glPopMatrix();
}

void drawPond() {
    glColor3f(0.2f, 0.5f, 0.9f);
    drawFilledCircle(150, 120, 60);
    drawFilledCircle(210, 120, 45);
    drawFilledCircle(180, 100, 55);

    glColor3f(0.8f, 0.9f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 10) {
        float rad = i * 3.14f / 180;
        glVertex2f(180 + cos(rad) * 70, 120 + sin(rad) * 30);
    }
    glEnd();

    glColor3f(1.0f, 0.5f, 0.1f);
    drawFilledCircle(170, 115, 6);
    glBegin(GL_TRIANGLES);
    glVertex2f(162, 115); glVertex2f(155, 120); glVertex2f(155, 110);
    glEnd();
}

void drawChickenCoop() {
    glColor3f(0.75f, 0.55f, 0.35f);
    drawRect(50, 220, 150, 270);

    glColor3f(0.6f, 0.2f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(40, 270);
    glVertex2f(160, 270);
    glVertex2f(100, 310);
    glEnd();

    glColor3f(0.3f, 0.15f, 0.05f);
    drawRect(85, 220, 115, 255);

    for (int x = 60; x <= 130; x += 35) {
        glColor3f(1.0f, 1.0f, 0.8f);
        drawFilledCircle(x, 210, 8);
        drawFilledCircle(x, 220, 5);

        glColor3f(1.0f, 0.3f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x + 5, 220);
        glVertex2f(x + 10, 218);
        glVertex2f(x + 5, 222);
        glEnd();
    }
}

void drawBarn() {
    glColor3f(0.85f, 0.2f, 0.2f);
    drawRect(800, 200, 950, 350);

    glColor3f(0.55f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(780, 350);
    glVertex2f(970, 350);
    glVertex2f(875, 420);
    glEnd();

    glColor3f(0.4f, 0.2f, 0.1f);
    drawRect(840, 200, 910, 290);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(820, 315, 845, 340);
    drawRect(905, 315, 930, 340);
}

void drawMountains() {
    float baseR = isDay ? 0.5f : 0.2f;
    float baseG = isDay ? 0.45f : 0.18f;
    float baseB = isDay ? 0.4f : 0.16f;
    float snowR = isDay ? 1.0f : 0.7f;

    glColor3f(baseR, baseG, baseB);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 200); glVertex2f(200, 500); glVertex2f(400, 200);
    glVertex2f(300, 200); glVertex2f(500, 450); glVertex2f(700, 200);
    glVertex2f(600, 200); glVertex2f(800, 380); glVertex2f(1000, 200);
    glEnd();

    glColor3f(snowR, snowR, snowR);
    glBegin(GL_TRIANGLES);
    glVertex2f(150, 450); glVertex2f(200, 500); glVertex2f(250, 450);
    glVertex2f(450, 410); glVertex2f(500, 450); glVertex2f(550, 410);
    glVertex2f(760, 350); glVertex2f(800, 380); glVertex2f(840, 350);
    glEnd();
}

// Enhanced walking person with better leg animation
void drawPerson(float x, float y, float armAngle, float legAngle) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Legs with walking animation
    glColor3f(0.2f, 0.3f, 0.5f);
    glPushMatrix();
    glTranslatef(-5, 30, 0);
    glRotatef(legAngle, 0, 0, 1);
    glTranslatef(5, -30, 0);
    drawRect(-8, 0, -2, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5, 30, 0);
    glRotatef(-legAngle, 0, 0, 1);
    glTranslatef(-5, -30, 0);
    drawRect(2, 0, 8, 30);
    glPopMatrix();

    // Body
    glColor3f(0.9f, 0.2f, 0.2f);
    drawRect(-12, 30, 12, 55);

    // Arms
    glPushMatrix();
    glTranslatef(-12, 50, 0);
    glRotatef(armAngle, 0, 0, 1);
    glTranslatef(12, -50, 0);
    glColor3f(0.9f, 0.2f, 0.2f);
    drawRect(-18, 35, -12, 50);
    glColor3f(0.95f, 0.76f, 0.65f);
    drawFilledCircle(-15, 33, 4);
    glPopMatrix();

    glColor3f(0.9f, 0.2f, 0.2f);
    drawRect(12, 35, 18, 50);
    glColor3f(0.95f, 0.76f, 0.65f);
    drawFilledCircle(15, 33, 4);

    // Head
    glColor3f(0.95f, 0.76f, 0.65f);
    drawFilledCircle(0, 65, 12);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(-4, 67, 2);
    drawFilledCircle(4, 67, 2);

    glBegin(GL_LINE_STRIP);
    glVertex2f(-4, 60);
    glVertex2f(0, 58);
    glVertex2f(4, 60);
    glEnd();

    glPopMatrix();
}

void drawCow(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    glColor3f(0.95f, 0.95f, 0.95f);
    drawRect(0, 30, 80, 70);
    glColor3f(0.1f, 0.1f, 0.1f);
    drawFilledCircle(20, 50, 10);
    drawFilledCircle(55, 60, 8);

    glColor3f(0.9f, 0.9f, 0.9f);
    drawRect(5, 0, 15, 30);
    drawRect(65, 0, 75, 30);

    glPushMatrix();
    glTranslatef(0, 60, 0);
    glRotatef(cowHeadAngle, 0, 0, 1);
    glTranslatef(0, -60, 0);
    glColor3f(0.95f, 0.95f, 0.95f);
    drawRect(-25, 50, 5, 75);
    glColor3f(1.0f, 0.7f, 0.8f);
    drawRect(-25, 50, -15, 60);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(-15, 68, 2);
    glPopMatrix();

    glPopMatrix();
}

// NEW: Vegetable field with rows
void drawVegetableField(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Soil rows
    glColor3f(0.4f, 0.25f, 0.15f);
    for (int i = 0; i < 5; i++) {
        drawRect(0, i * 20, 150, i * 20 + 12);
    }

    // Vegetables in rows (carrots, cabbages alternating)
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 6; col++) {
            float vx = col * 25 + 10;
            float vy = row * 20 + 6;

            if (row % 2 == 0) {
                // Carrots (orange circles with green tops)
                glColor3f(1.0f, 0.5f, 0.0f);
                drawFilledCircle(vx, vy, 4);
                glColor3f(0.0f, 0.7f, 0.0f);
                glBegin(GL_LINES);
                glVertex2f(vx, vy + 4);
                glVertex2f(vx, vy + 10);
                glEnd();
            }
            else {
                // Cabbages (green circles)
                glColor3f(0.2f, 0.7f, 0.2f);
                drawFilledCircle(vx, vy, 5);
            }
        }
    }

    glPopMatrix();
}

// NEW: Sheep/Goat
void drawSheep(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Body (fluffy white circle)
    glColor3f(0.95f, 0.95f, 0.95f);
    drawFilledCircle(0, 20, 18);

    // Head (smaller gray circle)
    glColor3f(0.3f, 0.3f, 0.3f);
    drawFilledCircle(-20, 22, 10);

    // Ears (small triangles)
    glBegin(GL_TRIANGLES);
    glVertex2f(-25, 30);
    glVertex2f(-22, 35);
    glVertex2f(-28, 32);

    glVertex2f(-15, 30);
    glVertex2f(-12, 35);
    glVertex2f(-18, 32);
    glEnd();

    // Legs (lines)
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(-10, 5); glVertex2f(-10, 0);
    glVertex2f(-3, 5); glVertex2f(-3, 0);
    glVertex2f(3, 5); glVertex2f(3, 0);
    glVertex2f(10, 5); glVertex2f(10, 0);
    glEnd();
    glLineWidth(1.0f);

    // Eye
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(-18, 24, 2);

    glPopMatrix();
}

// NEW: Flying birds
void drawSimpleBird(float x, float y, float flapAngle) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Body (small circle)
    glColor3f(0.2f, 0.2f, 0.2f);
    drawFilledCircle(0, 0, 4);

    // Wings (lines that flap)
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(-10, sin(flapAngle) * 8);

    glVertex2f(0, 0);
    glVertex2f(10, sin(flapAngle) * 8);
    glEnd();
    glLineWidth(1.0f);

    glPopMatrix();
}

void drawGarden(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(0, 0, 200, 5);
    drawRect(0, 25, 200, 30);

    for (int i = 0; i <= 200; i += 20) {
        drawRect(i, 0, i + 10, 50);
        glBegin(GL_TRIANGLES);
        glVertex2f(i, 50);
        glVertex2f(i + 5, 60);
        glVertex2f(i + 10, 50);
        glEnd();
    }
    for (int i = 15; i < 200; i += 35) {
        glColor3f(0.0f, 0.6f, 0.0f);
        drawRect(i, 0, i + 2, 35);
        glColor3f((i / 35) % 2 ? 1.0f : 1.0f, (i / 35) % 2 ? 0.2f : 0.8f, (i / 35) % 2 ? 0.8f : 0.0f);
        drawFilledCircle(i + 1, 35, 8);
        glColor3f(1.0f, 1.0f, 0.4f);
        drawFilledCircle(i + 1, 35, 3);
    }
    glPopMatrix();
}

void drawUI() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    drawRect(0, 0, 1000, 60);
    glDisable(GL_BLEND);

    glColor3f(1.0f, 1.0f, 1.0f);
    const char* txt1 = "CONTROLS: [D] Day  [N] Night  [O] Open Door  [C] Close Door";
    const char* txt2 = isDay ? "STATUS: Day Time" : "STATUS: Night Time";

    glRasterPos2f(20, 20);
    for (int i = 0; i < strlen(txt1); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, txt1[i]);

    glRasterPos2f(750, 20);
    for (int i = 0; i < strlen(txt2); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, txt2[i]);
}

void drawWindmill() {
    glPushMatrix();
    glTranslatef(880, 220, 0);
    glColor3f(0.7f, 0.7f, 0.8f);
    drawRect(-12, 0, 12, 160);
    glTranslatef(0, 160, 0);
    glRotatef(windmillAngle, 0, 0, 1);
    glColor3f(0.95f, 0.95f, 0.95f);
    for (int i = 0; i < 4; i++) {
        glRotatef(90, 0, 0, 1);
        drawRect(0, -10, 90, 10);
    }
    glPopMatrix();
}

void drawChimneySmoke() {
    glColor3f(0.7f, 0.7f, 0.7f);
    for (float sy : smokeY) {
        drawFilledCircle(620, sy, 8);
    }
}

void drawWell() {
    glColor3f(0.5f, 0.3f, 0.2f);
    drawFilledCircle(950, 150, 30);
    glColor3f(0.2f, 0.2f, 0.2f);
    drawFilledCircle(950, 150, 20);
    glColor3f(0.4f, 0.25f, 0.15f);
    drawRect(935, 150, 945, 200);
    drawRect(955, 150, 965, 200);
    drawRect(930, 195, 970, 200);
}

void drawSimpleFence() {
    glColor3f(0.6f, 0.4f, 0.2f);
    for (int x = 250; x < 750; x += 40) {
        drawRect(x, 180, x + 5, 220);
    }
    drawRect(250, 195, 750, 200);
    drawRect(250, 210, 750, 215);
}

void drawSunMoon() {
    if (isDay) {
        glColor3f(1.0f, 0.95f, 0.0f);
        drawFilledCircle(900, 650, 50);
    }
    else {
        glColor3f(0.9f, 0.9f, 0.95f);
        drawFilledCircle(900, 650, 45);
        glColor3f(0.05f, 0.08f, 0.25f);
        drawFilledCircle(915, 655, 40);

        for (auto& star : stars) {
            star.phase += 0.05f;
            float brightness = 0.5f + 0.5f * sin(star.phase);
            glColor3f(brightness, brightness, brightness);
            drawFilledCircle(star.x, star.y, 2);
        }
    }
}

void drawHouse() {
    glColor3f(0.92f, 0.90f, 0.85f);
    drawRect(300, 200, 700, 450);
    glColor3f(0.8f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(280, 450);
    glVertex2f(720, 450);
    glVertex2f(650, 600);
    glVertex2f(350, 600);
    glEnd();

    glColor3f(0.7f, 0.4f, 0.3f);
    drawRect(600, 480, 640, 580);

    glColor3f(0.4f, 0.7f, 0.9f);
    drawRect(360, 330, 440, 410);
    drawRect(560, 330, 640, 410);

    glPushMatrix();
    glTranslatef(500, 200, 0);
    glRotatef(doorAngle, 0, 1, 0);
    glTranslatef(-500, -200, 0);
    glColor3f(0.45f, 0.25f, 0.15f);
    drawRect(460, 200, 540, 350);
    glColor3f(0.9f, 0.8f, 0.2f);
    drawFilledCircle(525, 275, 4);
    glPopMatrix();
}

void drawButterfly() {
    glPushMatrix();
    glTranslatef(butterflyX, butterflyY, 0);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawRect(-2, -8, 2, 8);

    float wing = sin(butterflyWing) * 15;
    glColor3f(1.0f, 0.5f, 0.8f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-2, 0); glVertex2f(-15 - wing, 10); glVertex2f(-15 - wing, -10);
    glVertex2f(2, 0); glVertex2f(15 + wing, 10); glVertex2f(15 + wing, -10);
    glEnd();

    glPopMatrix();
}

void display() {
    glClearColor(isDay ? 0.53f : 0.05f, isDay ? 0.81f : 0.08f, isDay ? 0.98f : 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(isDay ? 0.4f : 0.1f, isDay ? 0.8f : 0.3f, isDay ? 0.3f : 0.1f);
    glVertex2f(0, 200);
    glVertex2f(1000, 200);
    glColor3f(isDay ? 0.3f : 0.05f, isDay ? 0.7f : 0.2f, isDay ? 0.2f : 0.05f);
    glVertex2f(1000, 0);
    glVertex2f(0, 0);
    glEnd();

    drawMountains();
    drawSunMoon();

    glPushMatrix();
    glTranslatef(cloudX, 0, 0);
    drawCloudCluster(200, 600);
    drawCloudCluster(600, 650);
    drawCloudCluster(900, 580);
    glPopMatrix();

    drawSimpleFence();
    drawHouse();
    drawChimneySmoke();
    drawGarden(750, 150);
    drawCow(cowX, 150);
    drawWindmill();
    drawWell();
    drawPond();
    drawChickenCoop();
    drawBarn();

    // NEW: Vegetable field
    drawVegetableField(550, 80);

    // NEW: Sheep grazing
    drawSheep(sheep1X, 100);
    drawSheep(sheep2X, 95);

    // People
    drawPerson(420, 150, person1ArmAngle, 0);
    drawPerson(person2X, 120, 0, person2LegAngle);

    // Tree
    glPushMatrix();
    glTranslatef(150, 200, 0);
    glColor3f(0.55f, 0.27f, 0.07f);
    drawRect(-10, 0, 10, 120);
    glTranslatef(0, 120, 0);
    glScalef(treeScale, 1.0f, 1.0f);
    glColor3f(isDay ? 0.13f : 0.05f, isDay ? 0.55f : 0.25f, isDay ? 0.13f : 0.05f);
    drawFilledCircle(0, 0, 50);
    glPopMatrix();

    // NEW: Flying birds
    drawSimpleBird(250, 600, birdFlap);
    drawSimpleBird(400, 630, birdFlap + 1.0f);
    drawSimpleBird(600, 580, birdFlap + 2.0f);
    drawSimpleBird(800, 610, birdFlap + 0.5f);

    drawButterfly();
    drawUI();

    glutSwapBuffers();
}

void timer(int) {
    cloudX += 0.7f;
    if (cloudX > 1100) cloudX = -400;

    windmillAngle += 3.0f;
    birdFlap += 0.15f;

    butterflyWing += 0.15f;
    butterflyX += cos(butterflyWing) * 2.5f;
    butterflyY += sin(butterflyWing * 0.8f) * 1.8f;
    if (butterflyX > 1000) butterflyX = -50;

    if (rand() % 10 == 0) smokeY.push_back(500.0f);
    for (auto it = smokeY.begin(); it != smokeY.end();) {
        *it += 1.1f;
        if (*it > 750) it = smokeY.erase(it);
        else ++it;
    }

    // Cow grazing animation
    if (grazingDown) {
        cowHeadAngle -= 0.5f;
        if (cowHeadAngle <= -15.0f) grazingDown = false;
    }
    else {
        cowHeadAngle += 0.5f;
        if (cowHeadAngle >= 0.0f) grazingDown = true;
    }

    // Person 1 waving
    if (person1WavingUp) {
        person1ArmAngle += 2.0f;
        if (person1ArmAngle >= 45.0f) person1WavingUp = false;
    }
    else {
        person1ArmAngle -= 2.0f;
        if (person1ArmAngle <= -10.0f) person1WavingUp = true;
    }

    // Person 2 walking with realistic leg animation
    if (person2MovingRight) {
        person2X += 0.8f;
        person2LegAngle = sin(person2X * 0.1f) * 20.0f;
        if (person2X >= 250.0f) person2MovingRight = false;
    }
    else {
        person2X -= 0.8f;
        person2LegAngle = sin(person2X * 0.1f) * 20.0f;
        if (person2X <= 100.0f) person2MovingRight = true;
    }

    // NEW: Sheep wandering
    if (sheep1MovingRight) {
        sheep1X += 0.3f;
        if (sheep1X >= 450.0f) sheep1MovingRight = false;
    }
    else {
        sheep1X -= 0.3f;
        if (sheep1X <= 280.0f) sheep1MovingRight = true;
    }

    sheep2X += sin(birdFlap) * 0.2f;

    // Tree breathing
    if (treeGrowing) {
        treeScale += 0.002f;
        if (treeScale >= 1.15f) treeGrowing = false;
    }
    else {
        treeScale -= 0.002f;
        if (treeScale <= 0.95f) treeGrowing = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int, int) {
    if (key == 'd' || key == 'D') isDay = true;
    if (key == 'n' || key == 'N') isDay = false;
    if (key == 'o' || key == 'O') doorAngle = -85.0f;
    if (key == 'c' || key == 'C') doorAngle = 0.0f;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(1000, 750);
    glutCreateWindow("Beautiful Animated Farmhouse - Fixed & Enhanced");
    gluOrtho2D(0, 1000, 0, 750);

    // Initialize stars
    for (int i = 0; i < 100; i++) {
        stars.push_back({ (float)(rand() % 1000), 450.0f + rand() % 300, (float)(rand() % 100) });
    }

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}