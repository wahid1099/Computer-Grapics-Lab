// main.cpp
// DIU Mini Campus - OpenGL (legacy) single-file demo
// Controls:
//  W/A/S/D  - move forward/left/back/right
//  ↑/↓/←/→  - look up/down/left/right (rotate camera yaw/pitch)
//  PageUp/PageDown - move camera up/down (zoom-ish)
//  N - toggle day/night
//  P - pause/resume student animation
//  R - reset camera
//  ESC - exit

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <string>
#include <cstdio>

// ---------- Camera ----------
float camX = 0.0f, camY = 3.5f, camZ = 18.0f;
float camYaw = 180.0f, camPitch = -10.0f; // degrees
float moveSpeed = 0.35f;
float lookSpeed = 3.0f;

// day/night
bool isNight = false;

// animation control
bool animateStudents = true;
float timeSeconds = 0.0f;

// window size
int winW = 1024, winH = 720;

// utility degrees->radians
inline float toRad(float deg) { return deg * 3.14159265f / 180.0f; }

// ---------- Simple student struct ----------
struct Student {
    float x, z;       // current position on ground (y is fixed)
    float angle;      // orientation (degrees)
    float speed;      // movement speed along path
    std::vector<std::pair<float, float>> path; // loop path points (x,z)
    int targetIndex;  // current target index in path
    bool forward;
};

std::vector<Student> students;

// create looped rectangular paths for students
void initStudents() {
    students.clear();
    // student 1
    Student s1;
    s1.path = { {-6.0f, -2.0f}, {-3.0f, -2.0f}, {-3.0f, -5.0f}, {-6.0f, -5.0f} };
    s1.x = s1.path[0].first; s1.z = s1.path[0].second;
    s1.targetIndex = 1; s1.forward = true; s1.speed = 0.8f; s1.angle = 0;
    students.push_back(s1);
    // student 2
    Student s2;
    s2.path = { {2.0f, -3.5f}, {5.0f, -3.5f}, {5.0f, -6.5f}, {2.0f, -6.5f} };
    s2.x = s2.path[0].first; s2.z = s2.path[0].second;
    s2.targetIndex = 1; s2.forward = true; s2.speed = 0.7f; s2.angle = 0;
    students.push_back(s2);
    // student 3
    Student s3;
    s3.path = { {-1.0f, 2.0f}, {3.0f, 2.0f}, {3.0f, -0.5f}, {-1.0f, -0.5f} };
    s3.x = s3.path[0].first; s3.z = s3.path[0].second;
    s3.targetIndex = 1; s3.forward = true; s3.speed = 1.0f; s3.angle = 0;
    students.push_back(s3);
}

// ---------- Drawing helpers ----------
void setMaterial(float r, float g, float b, float shine = 32.0f) {
    GLfloat ambient[] = { r * 0.15f, g * 0.15f, b * 0.15f, 1.0f };
    GLfloat diffuse[] = { r, g, b, 1.0f };
    GLfloat spec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat shininess = shine;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void drawCube(float sx = 1.0f, float sy = 1.0f, float sz = 1.0f) {
    glPushMatrix();
    glScalef(sx, sy, sz);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawBox(float w, float h, float d) {
    drawCube(w, h, d);
}

// draw a simple upright cylinder (using glut)
void drawCylinder(double base, double top, double height, int slices = 16, int stacks = 4) {
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, base, top, height, slices, stacks);
    gluDeleteQuadric(q);
}

// ---------- Scene objects ----------
void drawGround() {
    // big plane as ground (grass)
    setMaterial(0.2f, 0.6f, 0.2f, 8.0f);
    glPushMatrix();
    glTranslatef(0.0f, -0.005f, 0.0f);
    glBegin(GL_QUADS);
    // slightly darker stripes to suggest turf
    for (float x = -50.0f; x < 50.0f; x += 10.0f) {}
    glNormal3f(0, 1, 0);
    glVertex3f(-50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, -50.0f);
    glVertex3f(50.0f, 0.0f, 50.0f);
    glVertex3f(-50.0f, 0.0f, 50.0f);
    glEnd();
    glPopMatrix();
}

void drawMainGate() {
    // two pillars and a top slab
    glPushMatrix();
    glTranslatef(-10.0f, 0.0f, 0.0f);
    // left pillar
    glPushMatrix();
    glTranslatef(-2.0f, 2.5f, 0.0f);
    setMaterial(0.1f, 0.6f, 0.2f);
    drawBox(1.0f, 5.0f, 0.6f);
    glPopMatrix();
    // right pillar
    glPushMatrix();
    glTranslatef(2.0f, 2.5f, 0.0f);
    setMaterial(0.1f, 0.6f, 0.2f);
    drawBox(1.0f, 5.0f, 0.6f);
    glPopMatrix();
    // top beam
    glPushMatrix();
    glTranslatef(0.0f, 5.1f, 0.0f);
    setMaterial(0.9f, 0.9f, 0.9f);
    drawBox(6.0f, 0.6f, 1.0f);
    glPopMatrix();
    // decorative board with text (DIU)
    glPushMatrix();
    glTranslatef(0.0f, 5.8f, 0.0f);
    setMaterial(0.0f, 0.3f, 0.6f);
    drawBox(4.2f, 0.6f, 0.2f);
    // DIU label will be painted using bitmap strings below in world coordinates
    glPopMatrix();
    glPopMatrix();

    // DIU text near gate (world coords)
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(-10.0f, 5.8f, 0.12f);
    const char* txt = "DAFFODIL INTERNATIONAL UNIVERSITY";
    for (const char* c = txt; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    glPopMatrix();
}

void drawBuilding(float centerX, float centerZ, float width, float height, float depth, const char* label) {
    glPushMatrix();
    glTranslatef(centerX, height * 0.5f, centerZ);
    setMaterial(0.85f, 0.85f, 0.85f);
    drawBox(width, height, depth);

    // windows as darker quads repeated grid
    setMaterial(0.05f, 0.2f, 0.4f);
    int rows = int(height / 1.0f);
    int cols = int(width / 1.0f);
    float startX = -width / 2 + 0.6f, startY = -height / 2 + 0.6f;
    for (int r = 0;r < rows - 1;r++) {
        for (int c = 0;c < cols - 1;c++) {
            float wx = startX + c * 1.0f;
            float wy = startY + r * 1.0f;
            // front face windows (z positive)
            glPushMatrix();
            glTranslatef(wx, wy, depth / 2 + 0.01f);
            glScalef(0.6f, 0.6f, 0.02f);
            glutSolidCube(1.0f);
            glPopMatrix();
            // back face
            glPushMatrix();
            glTranslatef(wx, wy, -depth / 2 - 0.01f);
            glScalef(0.6f, 0.6f, 0.02f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }

    // building label
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos3f(centerX - width / 2 + 0.2f, height + 0.2f, centerZ + depth / 2 + 0.1f);
    for (const char* c = label; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    glPopMatrix();
}

void drawPlayground() {
    // positioned near center-right
    glPushMatrix();
    glTranslatef(6.0f, 0.0f, -4.0f);
    // green plane
    setMaterial(0.15f, 0.55f, 0.15f);
    glPushMatrix();
    glTranslatef(0.0f, 0.001f, 0.0f);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-6.0f, 0.0f, -6.0f);
    glVertex3f(6.0f, 0.0f, -6.0f);
    glVertex3f(6.0f, 0.0f, 6.0f);
    glVertex3f(-6.0f, 0.0f, 6.0f);
    glEnd();
    glPopMatrix();

    // goal posts (two)
    setMaterial(0.95f, 0.95f, 0.95f);
    glPushMatrix();
    glTranslatef(-4.2f, 1.0f, -4.8f);
    drawBox(0.2f, 2.0f, 0.2f);
    glTranslatef(4.4f, 0.0f, 0.0f);
    drawBox(0.2f, 2.0f, 0.2f);
    glTranslatef(-2.2f, 0.9f, 0.0f);
    drawBox(4.4f, 0.2f, 0.2f); // crossbar
    glPopMatrix();

    // flag pole
    glPushMatrix();
    glTranslatef(5.2f, 0.0f, 5.0f);
    setMaterial(0.5f, 0.5f, 0.5f);
    drawBox(0.08f, 3.5f, 0.08f);
    // flag (simple quad)
    setMaterial(0.0f, 0.5f, 0.8f);
    glPushMatrix();
    glTranslatef(0.3f, 2.2f, 0.0f);
    glRotatef(sinf(timeSeconds * 3.0f) * 10.0f, 0, 0, 1); // waving
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(0.0f, 0.3f, 0.01f);
    glVertex3f(1.0f, 0.15f, 0.01f);
    glVertex3f(0.0f, -0.15f, 0.01f);
    glVertex3f(-0.02f, 0.0f, 0.01f);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

// draw a simple stick-figure student (sphere head + cylinder body + legs)
void drawStudentModel() {
    // head
    setMaterial(1.0f, 0.9f, 0.7f);
    glPushMatrix();
    glTranslatef(0.0f, 0.55f, 0.0f);
    glutSolidSphere(0.15f, 12, 12);
    glPopMatrix();
    // body (cube)
    setMaterial(0.1f, 0.2f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 0.0f);
    glScalef(0.25f, 0.4f, 0.12f);
    glutSolidCube(1.0f);
    glPopMatrix();
    // legs
    setMaterial(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(-0.06f, 0.0f, 0.0f);
    glScalef(0.08f, 0.4f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.06f, 0.0f, 0.0f);
    glScalef(0.08f, 0.4f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// draw all students in world
void drawStudents() {
    for (auto& s : students) {
        glPushMatrix();
        glTranslatef(s.x, 0.0f, s.z);
        glRotatef(s.angle, 0, 1, 0);
        drawStudentModel();
        glPopMatrix();
    }
}

// simple trees (cylinder trunk + sphere foliage)
void drawTree(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    // trunk
    setMaterial(0.45f, 0.25f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f);
    glScalef(0.3f, 1.6f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();
    // foliage
    setMaterial(0.05f, 0.5f, 0.05f);
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glutSolidSphere(0.8f, 12, 12);
    glPopMatrix();
    glPopMatrix();
}

// ---------- Lighting ----------
void setupLights() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // ambient scene lighting changes for day/night
    GLfloat ambientDay[] = { 0.45f, 0.45f, 0.45f, 1.0f };
    GLfloat ambientNight[] = { 0.08f, 0.08f, 0.12f, 1.0f };
    GLfloat ambientUse[4];
    if (isNight) {
        for (int i = 0;i < 4;i++) ambientUse[i] = ambientNight[i];
    }
    else {
        for (int i = 0;i < 4;i++) ambientUse[i] = ambientDay[i];
    }
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientUse);

    // directional sun light (GL_LIGHT0)
    GLfloat sunDir[] = { -0.2f, -1.0f, -0.3f, 0.0f }; // directional
    GLfloat sunDiff[] = { 0.9f, 0.85f, 0.8f, 1.0f };
    GLfloat sunSpec[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    if (isNight) {
        GLfloat moonDiff[] = { 0.25f, 0.3f, 0.45f, 1.0f };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, moonDiff);
        GLfloat moonSpec[] = { 0.2f,0.25f,0.3f,1.0f };
        glLightfv(GL_LIGHT0, GL_SPECULAR, moonSpec);
        GLfloat moonDir[] = { 0.2f, -0.8f, 0.4f, 0.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, moonDir);
    }
    else {
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiff);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpec);
        glLightfv(GL_LIGHT0, GL_POSITION, sunDir);
    }
}

// ---------- Update animation ----------
void updateStudents(float dt) {
    if (!animateStudents) return;
    for (auto& s : students) {
        if (s.path.empty()) continue;
        // target point
        auto [tx, tz] = s.path[s.targetIndex];
        float dx = tx - s.x;
        float dz = tz - s.z;
        float dist = sqrtf(dx * dx + dz * dz);
        if (dist < 0.05f) {
            // advance index
            s.targetIndex = (s.targetIndex + 1) % s.path.size();
            auto [nx, nz] = s.path[s.targetIndex];
            dx = nx - s.x;
            dz = nz - s.z;
            dist = sqrtf(dx * dx + dz * dz);
            if (dist == 0) continue;
        }
        float vx = (dx / dist) * s.speed * dt;
        float vz = (dz / dist) * s.speed * dt;
        s.x += vx;
        s.z += vz;
        // update facing angle
        s.angle = atan2f(-vz, vx) * 180.0f / 3.14159265f; // adjust orientation
    }
}

// ---------- Rendering ----------
void drawScene() {
    // background (sky)
    if (isNight) {
        glClearColor(0.02f, 0.03f, 0.08f, 1.0f);
    }
    else {
        glClearColor(0.53f, 0.80f, 0.92f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // compute camera direction vector
    float yawR = toRad(camYaw), pitchR = toRad(camPitch);
    float cx = cosf(pitchR) * cosf(yawR);
    float cy = sinf(pitchR);
    float cz = cosf(pitchR) * sinf(yawR);
    // gluLookAt parameter: eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ
    gluLookAt(camX, camY, camZ, camX + cx, camY + cy, camZ + cz, 0.0f, 1.0f, 0.0f);

    // lighting
    setupLights();

    // ground
    drawGround();

    // main gate
    drawMainGate();

    // academic buildings
    drawBuilding(-3.0f, -8.0f, 6.0f, 4.5f, 4.0f, "AB-1");
    drawBuilding(3.5f, -6.0f, 5.0f, 6.0f, 4.5f, "AB-2");
    drawBuilding(0.0f, 6.0f, 9.0f, 3.8f, 5.0f, "AB-3");

    // playground
    drawPlayground();

    // trees
    drawTree(-8.0f, 4.0f);
    drawTree(-6.0f, 6.0f);
    drawTree(7.0f, -1.0f);
    drawTree(4.0f, 8.0f);
    drawTree(-1.0f, -10.0f);

    // students
    drawStudents();

    // DIU nameboard near entrance (a board)
    glPushMatrix();
    glTranslatef(-12.0f, 1.1f, -2.5f);
    setMaterial(0.05f, 0.3f, 0.6f);
    drawBox(4.0f, 0.8f, 0.2f);
    // board text
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(-14.0f, 1.5f, -2.2f);
    const char* t = "DAFFODIL INTERNATIONAL UNIVERSITY - MINI CAMPUS (Demo)";
    for (const char* c = t; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    glPopMatrix();

    glutSwapBuffers();
}

// ---------- GLUT callbacks ----------
void display() {
    drawScene();
}

void idleFunc() {
    // update time
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int now = glutGet(GLUT_ELAPSED_TIME);
    int diff = now - lastTime;
    lastTime = now;
    float dt = diff / 1000.0f;
    timeSeconds += dt;
    updateStudents(dt);
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    winW = w; winH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC
        exit(0); break;
    case 'w': case 'W':
        // move forward
        camX += cosf(toRad(camYaw)) * moveSpeed;
        camZ += sinf(toRad(camYaw)) * moveSpeed;
        break;
    case 's': case 'S':
        camX -= cosf(toRad(camYaw)) * moveSpeed;
        camZ -= sinf(toRad(camYaw)) * moveSpeed;
        break;
    case 'a': case 'A':
        camX += sinf(toRad(camYaw)) * moveSpeed;
        camZ -= cosf(toRad(camYaw)) * moveSpeed;
        break;
    case 'd': case 'D':
        camX -= sinf(toRad(camYaw)) * moveSpeed;
        camZ += cosf(toRad(camYaw)) * moveSpeed;
        break;
    case 'n': case 'N':
        isNight = !isNight;
        break;
    case 'p': case 'P':
        animateStudents = !animateStudents;
        break;
    case 'r': case 'R':
        // reset camera
        camX = 0.0f; camY = 3.5f; camZ = 18.0f;
        camYaw = 180.0f; camPitch = -10.0f;
        break;
    case '+':
    case '=':
        camY += 0.5f;
        break;
    case '-':
        camY -= 0.5f;
        break;
    }
    glutPostRedisplay();
}

void specialKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:    camPitch += lookSpeed; if (camPitch > 89) camPitch = 89; break;
    case GLUT_KEY_DOWN:  camPitch -= lookSpeed; if (camPitch < -89) camPitch = -89; break;
    case GLUT_KEY_LEFT:  camYaw -= lookSpeed; break;
    case GLUT_KEY_RIGHT: camYaw += lookSpeed; break;
    case GLUT_KEY_PAGE_UP:   camY += 0.7f; break;
    case GLUT_KEY_PAGE_DOWN: camY -= 0.7f; break;
    }
    glutPostRedisplay();
}

// ---------- Init GL ----------
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    // material color tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // default lighting set from draw loop
    glClearColor(0.53f, 0.80f, 0.92f, 1.0f);
}

// ---------- Main ----------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("DIU Mini Campus - OpenGL Demo");

    initGL();
    initStudents();

    glutDisplayFunc(display);
    glutIdleFunc(idleFunc);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);

    printf("Controls:\n W/A/S/D - Move  Arrow Keys - Look  PageUp/PageDown - Up/Down\n N - Toggle Day/Night  P - Pause Students  R - Reset  ESC - Exit\n");

    glutMainLoop();
    return 0;
}
