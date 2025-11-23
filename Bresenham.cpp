#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>

int x1_, y1_, x2_, y2_;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);
    glPointSize(3.0);

    int x = x1_;
    int y = y1_;
    int dx = abs(x2_ - x1_);
    int dy = abs(y2_ - y1_);
    int sx = (x2_ > x1_) ? 1 : -1;
    int sy = (y2_ > y1_) ? 1 : -1;
    int p;

    glBegin(GL_POINTS);

    if (dx > dy)
    {
        p = 2 * dy - dx;
        for (int i = 0; i <= dx; i++)
        {
            glVertex2f(x / 100.0, y / 100.0);
            if (p >= 0)
            {
                y += sy;
                p += 2 * (dy - dx);
            }
            else
            {
                p += 2 * dy;
            }
            x += sx;
        }
    }
    else
    {
        p = 2 * dx - dy;
        for (int i = 0; i <= dy; i++)
        {
            glVertex2f(x / 100.0, y / 100.0);
            if (p >= 0)
            {
                x += sx;
                p += 2 * (dx - dy);
            }
            else
            {
                p += 2 * dx;
            }
            y += sy;
        }
    }

    glEnd();
    glFlush();
}

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv)
{
    printf("Enter x1: ");
    scanf("%d", &x1_);
    printf("Enter y1: ");
    scanf("%d", &y1_);
    printf("Enter x2: ");
    scanf("%d", &x2_);
    printf("Enter y2: ");
    scanf("%d", &y2_);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
