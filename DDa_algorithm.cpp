#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

float x1_, y1_, x2_, y2_; // original inputs

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0); // Green
    glPointSize(5.0);

    float x = x1_;
    float y = y1_;
    float dx = x2_ - x1_;
    float dy = y2_ - y1_;
    float steps;

    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);

    float xInc = dx / steps;
    float yInc = dy / steps;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2f(x / 100, y / 100);  // scale down for OpenGL coordinate system
        x += xInc;
        y += yInc;
        printf("%f %f\n", x, y);
    }
    glEnd();

    glFlush();
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0); // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); // simple orthographic view
}

int main(int argc, char** argv) {
    printf("Enter value of X1: ");
    scanf("%f", &x1_);
    printf("Enter value of Y1: ");
    scanf("%f", &y1_);
    printf("Enter value of X2: ");
    scanf("%f", &x2_);
    printf("Enter value of Y2: ");
    scanf("%f", &y2_);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA Line Drawing");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
