#include <GL/glut.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} Point;

Point translate(Point p, float tx, float ty) {
    Point z;
    z.x = p.x + tx;
    z.y = p.y + ty;
    return z;
}

Point scale(Point p, float sx, float sy) {
    Point z;
    z.x = p.x * sx;
    z.y = p.y * sy;
    return z;
}

Point rotate(Point p, float angle) {
    Point z;
    float rad = angle * M_PI / 180.0;
    z.x = p.x * cos(rad) - p.y * sin(rad);
    z.y = p.x * sin(rad) + p.y * cos(rad);
    return z;
}



Point shear(Point p, float shx, float shy) {
    Point r;
    r.x = p.x + shx * p.y;
    r.y = p.y + shy * p.x;
    return r;
}

Point reflect(Point p) {
    Point z;
    z.x = -p.x;
    z.y = -p.y;
    return z;
}



Point Original[4] = { {0,0}, {2,0}, {2,2}, {0,2} };
Point Transformed[4];

void ComputeTransformedPoints() {
    float tx = 1, ty = 1;
    float sx = 1.5, sy = 1.5;
    float angle = 30;

    for (int i = 0; i < 4; i++) {
        Point p = Original[i];
        p = scale(p, sx, sy);
        p = rotate(p, angle);
        p = translate(p, tx, ty);
        p = reflect(p);
        p = shear(p, 0.5, 0.5);
        Transformed[i] = p;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; i++) {
        glVertex2f(Original[i].x, Original[i].y);
    }
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; i++) {
        glVertex2f(Transformed[i].x, Transformed[i].y);
    }
    glEnd();

    glFlush();
}

void init() {
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-15, 15, -15, 15);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    ComputeTransformedPoints();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("2D Transformations");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
