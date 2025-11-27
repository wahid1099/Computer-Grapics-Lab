#include <GL/glut.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} Point;

Point Original[4] = { {0,0}, {2,0}, {2,2}, {0,2} };
Point Transformed[4];

// Transformation parameters
float angle = 0.0f;
float sx = 1.0f, sy = 1.0f;
float tx = 0.0f, ty = 0.0f;

int animate = 0;  // Animation toggle

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
    float rad = angle * M_PI / 180.0;
    Point z;
    z.x = p.x * cos(rad) - p.y * sin(rad);
    z.y = p.x * sin(rad) + p.y * cos(rad);
    return z;
}

void ComputeTransformedPoints() {
    for (int i = 0; i < 4; i++) {
        Point p = Original[i];
        p = scale(p, sx, sy);
        p = rotate(p, angle);
        p = translate(p, tx, ty);
        Transformed[i] = p;
    }
}

void drawShape(Point arr[], float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; i++)
        glVertex2f(arr[i].x, arr[i].y);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    ComputeTransformedPoints();

    // Draw original shape (white)
    drawShape(Original, 1, 1, 1);

    // Draw transformed shape (red)
    drawShape(Transformed, 1, 0, 0);

    glFlush();
}

void timer(int value) {
    if (animate) {
        angle += 2; // rotate per frame
        if (angle >= 360) angle -= 360;
        glutPostRedisplay();
    }
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {

    case 'r': angle += 5; break;
    case 'R': angle -= 5; break;

    case 's': sx += 0.1; sy += 0.1; break;
    case 'S': sx -= 0.1; sy -= 0.1; break;

    case 't': tx += 0.2; break;
    case 'T': tx -= 0.2; break;

    case 'y': ty += 0.2; break;
    case 'Y': ty -= 0.2; break;

    case 'a': animate = !animate; break;

    case '0':  // Reset everything
        angle = 0;
        sx = sy = 1.0;
        tx = ty = 0;
        animate = 0;
        break;

    case 27: exit(0); // ESC key
    }

    glutPostRedisplay();
}

void init() {
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 20, -10, 20);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("2D Transformations with Animation");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
