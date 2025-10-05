#include <GL/glut.h>
#include <cmath>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);

    // House body
    glColor3f(0.88f, 0.78f, 0.64f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();

    // Roof
    glColor3f(0.45f, 0.20f, 0.10f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.55f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.55f, 0.0f);
    glEnd();

    // Door
    glColor3f(0.35f, 0.20f, 0.12f);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.5f);
    glVertex2f(0.1f, -0.5f);
    glVertex2f(0.1f, -0.1f);
    glVertex2f(-0.1f, -0.1f);
    glEnd();

    // Left window
    glColor3f(0.65f, 0.85f, 0.95f);
    glBegin(GL_QUADS);
    glVertex2f(-0.4f, -0.1f);
    glVertex2f(-0.25f, -0.1f);
    glVertex2f(-0.25f, 0.05f);
    glVertex2f(-0.4f, 0.05f);
    glEnd();

    // Right window
    glBegin(GL_QUADS);
    glVertex2f(0.25f, -0.1f);
    glVertex2f(0.4f, -0.1f);
    glVertex2f(0.4f, 0.05f);
    glVertex2f(0.25f, 0.05f);
    glEnd();

    // Tree
    glColor3f(0.45f, 0.28f, 0.12f);
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, -0.5f);
    glVertex2f(-0.75f, -0.5f);
    glVertex2f(-0.75f, -0.3f);
    glVertex2f(-0.8f, -0.3f);
    glEnd();

    glColor3f(0.14f, 0.55f, 0.12f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.1416f / 180.0f;
        glVertex2f(-0.775f + 0.1f * cos(theta), -0.2f + 0.1f * sin(theta));
    }
    glEnd();

    glFlush();
}

void init() {
    glClearColor(0.95f, 0.95f, 0.98f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Simple House - FreeGLUT");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
