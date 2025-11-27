#include <GL/glut.h>
#include <math.h>

float tx = 0, ty = 0;
float sx = 1, sy = 1;
float angle = 0;
int reflectX = 1, reflectY = 1;

// ---------------------------------------------------
// PLOT A POINT (with color support)
// ---------------------------------------------------
void plot(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// ---------------------------------------------------
// BRESENHAM LINE
// ---------------------------------------------------
void bresLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        plot(x1, y1);
        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

// ---------------------------------------------------
// MIDPOINT CIRCLE
// ---------------------------------------------------
void circle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;

    while (x <= y) {
        plot(xc + x, yc + y);
        plot(xc - x, yc + y);
        plot(xc + x, yc - y);
        plot(xc - x, yc - y);
        plot(xc + y, yc + x);
        plot(xc - y, yc + x);
        plot(xc + y, yc - x);
        plot(xc - y, yc - x);

        if (d < 0)
            d += 2 * x + 3;
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// ---------------------------------------------------
// TRANSFORMATION
// ---------------------------------------------------
void transform(int* x, int* y) {
    float X = *x, Y = *y;

    float rad = angle * 3.1416 / 180;

    float xr = (X * cos(rad) - Y * sin(rad));
    float yr = (X * sin(rad) + Y * cos(rad));

    xr = xr * sx * reflectX;
    yr = yr * sy * reflectY;

    *x = xr + tx;
    *y = yr + ty;
}

void drawLine(int x1, int y1, int x2, int y2) {
    transform(&x1, &y1);
    transform(&x2, &y2);
    bresLine(x1, y1, x2, y2);
}

void drawCircle(int xc, int yc, int r) {
    transform(&xc, &yc);
    circle(xc, yc, r);
}

// ---------------------------------------------------
// FILL POLYGONS WITH COLOR
// ---------------------------------------------------
void fillRect(int x1, int y1, int x2, int y2, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    int xA = x1, yA = y1, xB = x2, yB = y2;
    transform(&xA, &yA);
    transform(&xB, &yB);
    glVertex2i(xA, yA);
    glVertex2i(xB, yA);
    glVertex2i(xB, yB);
    glVertex2i(xA, yB);
    glEnd();
}

// ---------------------------------------------------
// ADVANCED COLORED CAR DRAWING
// ---------------------------------------------------
void drawCar() {

    // SHADOW
    fillRect(-160, -10, 160, -5, 0.1, 0.1, 0.1);

    // MAIN BODY (gradient style)
    fillRect(-150, 0, 150, 80, 0.8, 0.1, 0.2);  // red body

    // ROOF COLOR
    glColor3f(0.6, 0.1, 0.1);
    glBegin(GL_POLYGON);
    int x1 = -80, y1 = 80, x2 = -50, y2 = 140, x3 = 50, y3 = 140, x4 = 80, y4 = 80;
    transform(&x1, &y1); transform(&x2, &y2);
    transform(&x3, &y3); transform(&x4, &y4);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glVertex2i(x3, y3);
    glVertex2i(x4, y4);
    glEnd();

    // WINDOWS (sky blue)
    fillRect(-45, 90, 45, 130, 0.5, 0.7, 1);

    // HEADLIGHTS
    fillRect(135, 20, 150, 60, 1, 1, 0.2);

    // SPOILER
    fillRect(-160, 80, -140, 130, 0.3, 0.3, 0.3);

    // WHEELS FILL
    glColor3f(0.1, 0.1, 0.1);
    int lx = -80, ly = 0, rx = 80, ry = 0;
    transform(&lx, &ly); transform(&rx, &ry);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float rad = i * 3.1416 / 180;
        glVertex2i(lx + cos(rad) * 40, ly + sin(rad) * 40);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float rad = i * 3.1416 / 180;
        glVertex2i(rx + cos(rad) * 40, ry + sin(rad) * 40);
    }
    glEnd();

    // RIMS
    glColor3f(0.8, 0.8, 0.8);
    drawCircle(-80, 0, 20);
    drawCircle(80, 0, 20);

    // OUTLINES (algorithm)
    glColor3f(1, 1, 1);
    drawLine(-150, 0, 150, 0);
    drawLine(-150, 0, -150, 80);
    drawLine(150, 0, 150, 80);
    drawLine(-150, 80, 150, 80);

    drawLine(-80, 80, -50, 140);
    drawLine(80, 80, 50, 140);
    drawLine(-50, 140, 50, 140);

    drawCircle(-80, 0, 40);
    drawCircle(80, 0, 40);
}

// ---------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawCar();
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': ty += 10; break;
    case 's': ty -= 10; break;
    case 'a': tx -= 10; break;
    case 'd': tx += 10; break;

    case '+': sx += 0.1; sy += 0.1; break;
    case '-': sx -= 0.1; sy -= 0.1; break;

    case 'r': angle += 5; break;
    case 'f': angle -= 5; break;

    case 'x': reflectX *= -1; break;
    case 'y': reflectY *= -1; break;
    }

    glutPostRedisplay();
}

void init() {
    gluOrtho2D(-300, 300, -150, 300);
    glClearColor(0, 0, 0, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Advanced Colored Car (Algorithm Based)");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
