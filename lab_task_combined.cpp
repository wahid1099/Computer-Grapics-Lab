#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int WIN_W = 800;
const int WIN_H = 600;

// Function to draw a circle using Midpoint Circle Algorithm
void drawCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;

    glBegin(GL_POINTS);
    while (x <= y) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc - y, yc - x);

        if (d < 0) {
            d += 2 * x + 3;
        }
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    glEnd();
}

// Draw the house
void drawHouse() {
    // House body
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(300, 200);
    glVertex2i(500, 200);
    glVertex2i(500, 350);
    glVertex2i(300, 350);
    glEnd();

    // Roof (triangle)
    glBegin(GL_LINE_LOOP);
    glVertex2i(280, 350);
    glVertex2i(520, 350);
    glVertex2i(400, 450);
    glEnd();

    // Door
    glBegin(GL_LINE_LOOP);
    glVertex2i(370, 200);
    glVertex2i(430, 200);
    glVertex2i(430, 280);
    glVertex2i(370, 280);
    glEnd();

    // Windows
    glBegin(GL_LINE_LOOP);
    glVertex2i(310, 300);
    glVertex2i(350, 300);
    glVertex2i(350, 340);
    glVertex2i(310, 340);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2i(450, 300);
    glVertex2i(490, 300);
    glVertex2i(490, 340);
    glVertex2i(450, 340);
    glEnd();
}

// Draw a tree using rectangles and circles
void drawTree() {
    // Trunk
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(600, 200);
    glVertex2i(620, 200);
    glVertex2i(620, 300);
    glVertex2i(600, 300);
    glEnd();

    // Canopy (circle)
    drawCircle(610, 330, 40);
}

// Draw sun in the sky
void drawSun() {
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(150, 450, 50);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw ground line
    glBegin(GL_LINES);
    glVertex2i(0, 180);
    glVertex2i(WIN_W, 180);
    glEnd();

    drawHouse();
    drawTree();
    drawSun();

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H); // 2D orthographic projection
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("House, Tree, and Sun - Circle Algorithm");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
