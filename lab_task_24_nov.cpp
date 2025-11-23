#include <GL/glut.h>
#include <cmath>
#include <stdio.h>

// Window dimensions
const int WIN_W = 800;
const int WIN_H = 600;

// --- 1. DDA ALGORITHM FUNCTION (From your DDA Code) ---
// We modified this to take parameters instead of using scanf
void drawLineDDA(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps;

    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1;
    float y = y1;

    glBegin(GL_POINTS); // We are plotting points manually!
    for (int i = 0; i <= steps; i++) {
        glVertex2f(x, y);
        x += xInc;
        y += yInc;
    }
    glEnd();
}

// --- 2. STANDARD PRIMITIVES (For the House) ---
void drawHouse() {
    glLineWidth(2.0f);

    // House Body (GL_QUADS from your primitives code)
    glColor3f(0.6f, 0.3f, 0.1f); // Brown
    glBegin(GL_QUADS);
    glVertex2f(300, 100);
    glVertex2f(500, 100);
    glVertex2f(500, 300);
    glVertex2f(300, 300);
    glEnd();

    // Roof (GL_TRIANGLES)
    glColor3f(0.8f, 0.0f, 0.0f); // Red
    glBegin(GL_TRIANGLES);
    glVertex2f(280, 300);
    glVertex2f(520, 300);
    glVertex2f(400, 450);
    glEnd();

    // Door (GL_POLYGON)
    glColor3f(0.4f, 0.2f, 0.0f); // Dark Brown
    glBegin(GL_POLYGON);
    glVertex2f(380, 100);
    glVertex2f(420, 100);
    glVertex2f(420, 200);
    glVertex2f(380, 200);
    glEnd();
}

// --- 3. LOOPS & MATH (For the Sun Body and Grass) ---
void drawSunAndGrass() {
    // Sun Body (Circle logic from your tree/house code)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glBegin(GL_POLYGON);
    float cx = 100.0f;
    float cy = 500.0f;
    float r = 40.0f;
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.1416f / 180.0f;
        glVertex2f(cx + r * cos(theta), cy + r * sin(theta));
    }
    glEnd();

    // Grass (Loop logic from your Chessboard code)
    // We draw small green quads along the bottom
    glColor3f(0.0f, 0.8f, 0.0f); // Green
    float grassSize = 20.0f;
    for (int i = 0; i < WIN_W; i += grassSize) {
        glBegin(GL_QUADS);
        glVertex2f(i, 0);
        glVertex2f(i + grassSize, 0);
        glVertex2f(i + grassSize, 50); // height of grass
        glVertex2f(i, 50);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Draw the Scenery using Standard Primitives
    drawSunAndGrass();
    drawHouse();

    // 2. Draw Sun Rays using DDA ALGORITHM
    // This combines the "Line Algo" task with the "Scenery" task.
    glColor3f(1.0f, 0.8f, 0.0f); // Orange-ish
    glPointSize(2.0f); // Make the DDA dots visible

    // Center of sun is (100, 500). We draw lines radiating out.
    drawLineDDA(100, 500, 200, 500); // Right ray
    drawLineDDA(100, 500, 200, 400); // Bottom-Right ray
    drawLineDDA(100, 500, 100, 350); // Bottom ray
    drawLineDDA(100, 500, 0, 350);   // Bottom-Left ray
    drawLineDDA(100, 500, 250, 550); // Far Right ray

    glFlush();
}

void init() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Sky Blue Background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Using 0 to 800 coords to make DDA calculations easy to understand
    gluOrtho2D(0, WIN_W, 0, WIN_H);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Lab Task: The Algorithmic Sunrise");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}