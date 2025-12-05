#include <GL/glut.h>
#include <iostream>
using namespace std;

// --- Cube Corner Points (Predefined for convenience) ---
float X1 = -2.0, Y1 = -2.0, Z1 = -2.0;
float X2 = 2.0, Y2 = 2.0, Z2 = 2.0;
// This creates a 4x4x4 cube centered at (0, 0, 0)

// --- Translation Variables ---
float Tx = 1.0;          // Translation step size for X
float Ty = 1.0;          // Translation step size for Y
float Tz = 1.0;          // Translation step size for Z (FIXED: Declared properly)
float moveX = 0, moveY = 0, moveZ = 0; // Accumulated Translation

// --- Rotation Variables ---
float rotX = 0.0, rotY = 0.0, rotZ = 0.0; // Accumulated Rotation Angles
float rotStep = 5.0;     // Rotation step in degrees

// --- Scaling Variables ---
float scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0; // Accumulated Scaling Factors
float scaleStep = 0.1;   // Scaling step factor

void drawCubeFromPoints(float x1, float y1, float z1, float x2, float y2, float z2)
{
    glBegin(GL_QUADS);

    // Front face (Red)
    glColor3f(1, 0, 0);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);

    // Back face (Green)
    glColor3f(0, 1, 0);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);

    // Left face (Blue)
    glColor3f(0, 0, 1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);

    // Right face (Yellow)
    glColor3f(1, 1, 0);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);

    // Top face (Cyan)
    glColor3f(0, 1, 1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);

    // Bottom face (Magenta)
    glColor3f(1, 0, 1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 1. Camera/View setup
    gluLookAt(15, 15, 25, 0, 0, 0, 0, 1, 0);

    // 2. Apply Accumulated Transformations (Order: Translate * Rotate * Scale)

    // Translation
    glTranslatef(moveX, moveY, moveZ);

    // Rotation
    glRotatef(rotX, 1.0, 0.0, 0.0); // Rotate around X-axis
    glRotatef(rotY, 0.0, 1.0, 0.0); // Rotate around Y-axis
    glRotatef(rotZ, 0.0, 0.0, 1.0); // Rotate around Z-axis

    // Scaling
    glScalef(scaleX, scaleY, scaleZ);

    // 3. Draw the object
    drawCubeFromPoints(X1, Y1, Z1, X2, Y2, Z2);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        // --- Translation ---
    case 'X': moveX += Tx; break;   // +X
    case 'x': moveX -= Tx; break;   // -X
    case 'Y': moveY += Ty; break;   // +Y
    case 'y': moveY -= Ty; break;   // -Y
    case 'Z': moveZ += Tz; break;   // +Z
    case 'z': moveZ -= Tz; break;   // -Z

        // --- Rotation ---
    case 'r': rotX += rotStep; break;   // Rotate +X (Pitch)
    case 'R': rotX -= rotStep; break;   // Rotate -X
    case 'a': rotY += rotStep; break;   // Rotate +Y (Yaw)
    case 'A': rotY -= rotStep; break;   // Rotate -Y
    case 'e': rotZ += rotStep; break;   // Rotate +Z (Roll)
    case 'E': rotZ -= rotStep; break;   // Rotate -Z

        // --- Scaling ---
    case '+':
        scaleX += scaleStep;
        scaleY += scaleStep;
        scaleZ += scaleStep;
        break; // Uniform Scale Up
    case '-':
        scaleX -= scaleStep;
        scaleY -= scaleStep;
        scaleZ -= scaleStep;
        break; // Uniform Scale Down
    case 's': scaleX += scaleStep; break; // Scale +X
    case 'S': scaleX -= scaleStep; break; // Scale -X
    case 'd': scaleY += scaleStep; break; // Scale +Y
    case 'D': scaleY -= scaleStep; break; // Scale -Y
    case 'f': scaleZ += scaleStep; break; // Scale +Z
    case 'F': scaleZ -= scaleStep; break; // Scale -Z
    }

    // Safety checks for scaling (avoid negative/zero dimensions)
    if (scaleX < 0.1) scaleX = 0.1;
    if (scaleY < 0.1) scaleY = 0.1;
    if (scaleZ < 0.1) scaleZ = 0.1;

    // Keep rotation angles within 0-360 degrees
    if (rotX >= 360.0) rotX -= 360.0;
    if (rotY >= 360.0) rotY -= 360.0;
    if (rotZ >= 360.0) rotZ -= 360.0;
    if (rotX < 0.0) rotX += 360.0;
    if (rotY < 0.0) rotY += 360.0;
    if (rotZ < 0.0) rotZ += 360.0;

    glutPostRedisplay();
}


void init()
{
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    cout << "--- 3D Transformations Controls ---" << endl;
    cout << "Translation: (X/x, Y/y, Z/z)" << endl;
    cout << "Rotation: (r/R for X, a/A for Y, e/E for Z)" << endl;
    cout << "Scaling: (+/- for uniform, s/S, d/D, f/F for axes)" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("3D Scale, Rotation, Translation");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}