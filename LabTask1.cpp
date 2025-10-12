#include <GL/glut.h>
#include <cmath>

void display() {
    // Clear the window with the background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the drawing color to black for all shapes
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f); // Make lines a bit thicker

    // House body
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();

    // Roof
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.55f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.55f, 0.0f);
    glEnd();

    // Door
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.1f, -0.5f);
    glVertex2f(0.1f, -0.5f);
    glVertex2f(0.1f, -0.1f);
    glVertex2f(-0.1f, -0.1f);
    glEnd();

    // Left window
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.4f, -0.1f);
    glVertex2f(-0.25f, -0.1f);
    glVertex2f(-0.25f, 0.05f);
    glVertex2f(-0.4f, 0.05f);
    glEnd();

    // Right window
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.25f, -0.1f);
    glVertex2f(0.4f, -0.1f);
    glVertex2f(0.4f, 0.05f);
    glVertex2f(0.25f, 0.05f);
    glEnd();

    // Tree trunk
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.8f, -0.5f);
    glVertex2f(-0.75f, -0.5f);
    glVertex2f(-0.75f, -0.3f);
    glVertex2f(-0.8f, -0.3f);
    glEnd();

    // Tree canopy (outline of a circle)
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.1416f / 180.0f;
        glVertex2f(-0.775f + 0.1f * cos(theta), -0.2f + 0.1f * sin(theta));
    }
    glEnd();

    glFlush();
}

void init() {
    // Set a white background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Simple House - Monochrome");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}