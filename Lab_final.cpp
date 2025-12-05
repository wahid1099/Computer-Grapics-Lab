#include <GL/glut.h>
#include <cmath>


void drawLine(float x0, float y0, float x1, float y1) {
    glBegin(GL_LINES);
    glVertex2f(x0, y0);
    glVertex2f(x1, y1);
    glEnd();
}


void drawCircle(float cx, float cy, float radius) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float angle = 2.0f * 3.14159f * i / 100;
        float x = cx + radius * cos(angle);
        float y = cy + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}


void drawFilledCircle(float cx, float cy, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 100; i++) {
        float angle = 2.0f * 3.14159f * i / 100;
        float x = cx + radius * cos(angle);
        float y = cy + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}


void drawStraightLineBehindRoof() {
    glColor3f(0.0f, 0.0f, 0.0f);
    drawLine(-1.0f, 0.35f, 1.0f, 0.35f);
}

void drawHouse() {

    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.05f);
    glVertex2f(-0.3f, 0.21f);
    glVertex2f(-0.1f, -0.05f);
    glEnd();

    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.3f, 0.21f);
    glVertex2f(0.225f, 0.21f);
    glVertex2f(0.425f, -0.05f);
    glVertex2f(-0.1f, -0.05f);
    glEnd();

    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-0.465f, -0.05f);
    glVertex2f(-0.1f, -0.05f);
    glVertex2f(-0.1f, -0.35f);
    glVertex2f(-0.465f, -0.35f);
    glEnd();

    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0.39f, -0.05f);
    glVertex2f(-0.11f, -0.05f);
    glVertex2f(-0.11f, -0.35f);
    glVertex2f(0.39f, -0.35f);
    glEnd();

    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-0.465f, -0.35f);
    glVertex2f(0.39f, -0.35f);
    glVertex2f(0.435f, -0.385f);
    glVertex2f(-0.51f, -0.385f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.105f, -0.2f);
    glVertex2f(0.195f, -0.2f);
    glVertex2f(0.195f, -0.35f);
    glVertex2f(0.105f, -0.35f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.105f, -0.2f);
    glVertex2f(0.195f, -0.2f);
    glVertex2f(0.195f, -0.35f);
    glVertex2f(0.105f, -0.35f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    drawLine(0.150f, -0.35f, 0.150f, -0.2f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.5f, -0.05f);
    glVertex2f(-0.3f, 0.21f);
    glVertex2f(-0.1f, -0.05f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.3f, 0.21f);
    glVertex2f(0.225f, 0.21f);
    glVertex2f(0.425f, -0.05f);
    glVertex2f(-0.1f, -0.05f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.465f, -0.05f);
    glVertex2f(-0.11f, -0.05f);
    glVertex2f(-0.11f, -0.35f);
    glVertex2f(-0.465f, -0.35f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.39f, -0.05f);
    glVertex2f(-0.11f, -0.05f);
    glVertex2f(-0.11f, -0.35f);
    glVertex2f(0.39f, -0.35f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.465f, -0.35f);
    glVertex2f(0.39f, -0.35f);
    glVertex2f(0.435f, -0.385f);
    glVertex2f(-0.51f, -0.385f);
    glEnd();

    glFlush();

}


void drawMoon() {

    glColor3f(0.8f, 0.8f, 0.8f);
    drawFilledCircle(0.7f, 0.7f, 0.1f);


    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(0.7f, 0.7f, 0.1f);
}


void drawBird() {
    glColor3f(0.0f, 0.0f, 0.0f);


    drawLine(-0.7f, 0.6f, -0.75f, 0.65f);


    drawLine(-0.7f, 0.6f, -0.65f, 0.65f);



}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawStraightLineBehindRoof();
    drawHouse();
    drawMoon();
    drawBird();
    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLineWidth(2.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("House Scene");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}