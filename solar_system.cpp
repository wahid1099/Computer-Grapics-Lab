// solar.cpp
// Simple Solar System animation (legacy OpenGL + freeGLUT)
// Features:
//  - Sun with glow (additive blended concentric spheres)
//  - Planets with rotation (spin) and revolution (orbit)
//  - Orbit paths
//  - Keyboard controls: Space pause, +/- speed, arrow keys rotate camera, W/S zoom
//  - Uses GLUT (freeGLUT) and legacy fixed-function pipeline

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>
#include <vector>
#include <cstdio>

// ---------- Config ----------
const float PI = 3.14159265358979323846f;

// Window
int winW = 1000, winH = 700;

// Time and animation control
float globalSpeed = 1.0f;   // multiplier for orbital/rotation speeds
bool paused = false;
float elapsedSeconds = 0.0f;
int lastTick = 0;

// Camera (orbit-style camera around origin)
float camAngleY = 20.0f;   // pitch
float camAngleX = -30.0f;  // yaw
float camDistance = 40.0f;

// Planet struct
struct Planet {
    const char* name;
    float radius;           // visual radius
    float distance;         // from sun center
    float orbitPeriod;      // seconds for a full revolution (smaller => faster)
    float rotationPeriod;   // seconds for a full spin on its axis
    float orbitAngle;       // current angle around sun (degrees)
    float rotationAngle;    // current spin angle (degrees)
    float color[3];
    bool showOrbit;
};

// Planets list
std::vector<Planet> planets;

// forward declarations
void initPlanets();
void display();
void reshape(int w, int h);
void idleFunc();
void keyboard(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void drawOrbit(float radius);
void drawSun();
void drawPlanet(const Planet& p);
void drawTextOnScreen(float x, float y, const char* str);

// ---------- Initialize ----------
void initPlanets() {
    planets.clear();
    // The numbers are tuned to look nice, not astronomically accurate.
    planets.push_back({ "Mercury", 0.6f, 6.0f, 8.0f, 10.0f, 0, 0, {0.6f, 0.6f, 0.63f}, true });
    planets.push_back({ "Venus",   1.0f, 9.0f, 12.0f, -20.0f, 0, 0, {0.9f, 0.7f, 0.4f}, true });
    planets.push_back({ "Earth",   1.1f, 12.0f, 16.0f, 30.0f, 0, 0, {0.2f, 0.4f, 0.9f}, true });
    planets.push_back({ "Mars",    0.8f, 15.0f, 20.0f, 25.0f, 0, 0, {0.9f, 0.35f, 0.2f}, true });
    planets.push_back({ "Jupiter", 2.4f, 20.0f, 40.0f, 15.0f, 0, 0, {0.9f, 0.7f, 0.5f}, true });
    planets.push_back({ "Saturn",  2.1f, 26.0f, 70.0f, -18.0f, 0, 0, {0.95f, 0.85f, 0.6f}, true });
    planets.push_back({ "Uranus",  1.6f, 31.0f, 100.0f, 12.0f, 0, 0, {0.6f, 0.85f, 0.9f}, true });
    planets.push_back({ "Neptune", 1.6f, 36.0f, 140.0f, 12.0f, 0, 0, {0.25f, 0.45f, 0.9f}, true });
}

// ---------- Helpers ----------
void setMaterial(const float* diffuse, float shininess = 32.0f) {
    GLfloat am[4] = { diffuse[0] * 0.1f, diffuse[1] * 0.1f, diffuse[2] * 0.1f, 1.0f };
    GLfloat diff[4] = { diffuse[0], diffuse[1], diffuse[2], 1.0f };
    GLfloat spec[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, am);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

// draw a thick circular orbit line (XZ-plane)
void drawOrbit(float r) {
    const int slices = 180;
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < slices;i++) {
        float a = 2.0f * PI * i / slices;
        float x = cosf(a) * r;
        float z = sinf(a) * r;
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

// draw the sun with glow using additive blended translucent spheres
void drawSun() {
    // Sun base (emissive)
    glPushMatrix();
    GLfloat sunCol[3] = { 1.0f, 0.85f, 0.2f };
    // emissive core
    GLfloat emission[] = { sunCol[0] * 1.0f, sunCol[1] * 1.0f, sunCol[2] * 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);

    // Draw core bright sphere
    glDisable(GL_LIGHTING); // draw bright core as unlit color to simulate glow
    glColor3f(sunCol[0], sunCol[1], sunCol[2]);
    glutSolidSphere(3.0f, 40, 40);
    glEnable(GL_LIGHTING);

    // clear emission for other objects
    GLfloat noEmission[] = { 0,0,0,1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    glPopMatrix();

    // Glow: draw several translucent spheres with additive blending
    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // additive
    glDepthMask(GL_FALSE); // don't write depth so glow accumulates
    // draw concentric spheres (larger and more transparent)
    int layers = 5;
    for (int i = 0;i < layers;i++) {
        float t = (float)i / (float)(layers - 1);
        float radius = 3.0f + t * 6.0f;
        float alpha = 0.18f * (1.0f - t);
        float r = 1.0f, g = 0.85f, b = 0.2f;
        glColor4f(r, g, b, alpha);
        glutSolidSphere(radius, 30, 30);
    }
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPopAttrib();
}

// draw a planet at its orbit position with rotation
void drawPlanet(const Planet& p) {
    // compute position in XZ-plane
    float ax = p.orbitAngle * (PI / 180.0f);
    float px = cosf(ax) * p.distance;
    float pz = sinf(ax) * p.distance;

    glPushMatrix();
    // translate to orbit position
    glTranslatef(px, 0.0f, pz);

    // material color
    setMaterial(p.color, 16.0f);

    // rotate planet on its axis (tilt can be added)
    glRotatef(p.rotationAngle, 0.0f, 1.0f, 0.0f);

    // sphere for planet body
    glColor3fv(p.color);
    glutSolidSphere(p.radius, 32, 32);

    // small moon for Earth (if Earth)
    if (strcmp(p.name, "Earth") == 0) {
        glPushMatrix();
        // moon orbit around earth
        float moonAngle = p.rotationAngle * 3.0f;
        float mx = cosf(toRadians(moonAngle)) * 2.2f;
        float mz = sinf(toRadians(moonAngle)) * 2.2f;
        glTranslatef(mx, 0.0f, mz);
        setMaterial((float[]) { 0.7f, 0.7f, 0.7f }, 8.0f);
        glutSolidSphere(0.28f, 16, 16);
        glPopMatrix();
    }

    glPopMatrix();
}

// helper to convert deg->rad inline
inline float toRadians(float deg) { return deg * PI / 180.0f; }

// draw HUD text at top-left
void drawTextOnScreen(float x, float y, const char* str) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);
    for (const char* c = str; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ---------- Rendering ----------
void display() {
    glClearColor(0.02f, 0.02f, 0.04f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set viewport & camera
    glViewport(0, 0, winW, winH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)winW / winH, 0.1, 400.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // camera position: using spherical coords based on camAngleX/Y and camDistance
    float ay = camAngleY * PI / 180.0f;
    float ax = camAngleX * PI / 180.0f;

    float cx = camDistance * cosf(ay) * cosf(ax);
    float cy = camDistance * sinf(ay);
    float cz = camDistance * cosf(ay) * sinf(ax);

    gluLookAt(cx, cy, cz,  // eye
        0.0, 0.0, 0.0, // center
        0.0, 1.0, 0.0); // up

    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // set light as weak ambient + directional from camera (for subtle shading)
    GLfloat lightPos[] = { 0.0f, 30.0f, 0.0f, 1.0f };
    GLfloat lightDiff[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat lightAmb[] = { 0.08f, 0.08f, 0.1f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);

    // Draw orbits first (no lighting)
    glDisable(GL_LIGHTING);
    glColor3f(0.35f, 0.35f, 0.35f);
    for (const auto& p : planets) if (p.showOrbit) drawOrbit(p.distance);
    glEnable(GL_LIGHTING);

    // Draw Sun at origin
    glPushMatrix();
    drawSun();
    glPopMatrix();

    // Draw planets
    for (const auto& p : planets) {
        drawPlanet(p);
    }

    // HUD
    char buf[128];
    snprintf(buf, sizeof(buf), "Space: Pause/Resume   +/-: Speed (%.2fx)   Arrows: rotate camera   W/S: zoom", globalSpeed);
    drawTextOnScreen(10, winH - 20, buf);

    // small legend of planet names placed near each planet (screen-space approx)
    // (for simplicity draw labels by projecting planet XY to screen - but here we just skip projection)
    glutSwapBuffers();
}

// ---------- Update animation (idle) ----------
void idleFunc() {
    int now = glutGet(GLUT_ELAPSED_TIME);
    int diff = now - lastTick;
    if (lastTick == 0) diff = 0;
    lastTick = now;
    float dt = diff / 1000.0f;
    if (!paused) elapsedSeconds += dt * globalSpeed;

    // Update planet angles
    for (auto& p : planets) {
        if (p.orbitPeriod != 0.0f) {
            // angle per second = 360 / orbitPeriod
            float degPerSec = 360.0f / p.orbitPeriod;
            p.orbitAngle += degPerSec * dt * globalSpeed;
            if (p.orbitAngle >= 360.0f) p.orbitAngle -= 360.0f;
        }
        if (p.rotationPeriod != 0.0f) {
            float degPerSec = 360.0f / p.rotationPeriod;
            p.rotationAngle += degPerSec * dt * globalSpeed;
            if (p.rotationAngle >= 360.0f) p.rotationAngle -= 360.0f;
        }
    }

    glutPostRedisplay();
}

// ---------- Input ----------
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC
        exit(0);
        break;
    case ' ':
        paused = !paused;
        break;
    case '+':
    case '=':
        globalSpeed *= 1.2f;
        if (globalSpeed > 10.0f) globalSpeed = 10.0f;
        break;
    case '-':
    case '_':
        globalSpeed /= 1.2f;
        if (globalSpeed < 0.01f) globalSpeed = 0.01f;
        break;
    case 'w':
    case 'W':
        camDistance -= 2.0f;
        if (camDistance < 5.0f) camDistance = 5.0f;
        break;
    case 's':
    case 'S':
        camDistance += 2.0f;
        break;
    case 'o':
    case 'O':
        // toggle orbit visibility for all
        for (auto& p : planets) p.showOrbit = !p.showOrbit;
        break;
    }
}

void specialKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:  camAngleX -= 6.0f; break;
    case GLUT_KEY_RIGHT: camAngleX += 6.0f; break;
    case GLUT_KEY_UP:    camAngleY += 4.0f; if (camAngleY > 89.0f) camAngleY = 89.0f; break;
    case GLUT_KEY_DOWN:  camAngleY -= 4.0f; if (camAngleY < -89.0f) camAngleY = -89.0f; break;
    case GLUT_KEY_PAGE_UP: camDistance -= 2.0f; if (camDistance < 5.0f) camDistance = 5.0f; break;
    case GLUT_KEY_PAGE_DOWN: camDistance += 2.0f; break;
    }
}

// ---------- Reshape ----------
void reshape(int w, int h) {
    if (h == 0) h = 1;
    winW = w; winH = h;
    glViewport(0, 0, w, h);
}

// ---------- Init GL ----------
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // lighting general settings
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // nice perspective hints
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

// ---------- Main ----------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
