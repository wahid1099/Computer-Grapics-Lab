#include <GL/glut.h>
#include <cmath>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    // glBegin(GL_TRIANGLES);
    // glBegin(GL_QUADS);
    glBegin(GL_POLYGON);
    //For trainagles
     // glVertex2f(0.0f, 0.5f);
     // glVertex2f(-0.5f, -0.5f);
     // glVertex2f(0.5f, -0.5f);

     //For quads
    // glVertex2f(0.0f, 0.5f);
    // glVertex2f(0.5f, 0.5f);
    // glVertex2f(0.5f, 0.0f);
    // glVertex2f(0.0f, 0.0f);


     //For quads
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.25f, 0.75f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, 0.0f);
    glVertex2f(0.0f, 0.0f);

    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Triangle using GL_TRIANGLES");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}