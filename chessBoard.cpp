#include <GL/glut.h>
#include <cmath>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    int rows = 8, cols = 8;

    float xstart = -0.8f;
    float ystart = -0.8f;
    float squareSize = 0.2f;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            if ((i + j) % 2 == 0)
                glColor3f(1.0f, 1.0f, 1.0f);
            else
                glColor3f(0.0f, 0.0f, 0.0f);

            float x = xstart + j * squareSize;
            float y = ystart + i * squareSize;

            glBegin(GL_POLYGON);
            glVertex2f(x, y);
            glVertex2f(x + squareSize, y);
            glVertex2f(x + squareSize, y + squareSize);
            glVertex2f(x, y + squareSize);
            glEnd();
        }
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);


    glEnd();

    glFlush();
}

void init() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("8x8 Chess Board");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}