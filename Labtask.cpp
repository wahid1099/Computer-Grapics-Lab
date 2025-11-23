#include <stdio.h>
#include <GL/glut.h>

int x1_, y1_, x2_, y2_;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0); // Green color for line
    glPointSize(5.0);         // Bold points

    int x = x1_;
    int y = y1_;
    int dx = x2_ - x1_;
    int dy = y2_ - y1_;

    int sx = (dx >= 0) ? 1 : -1; // Step direction for x
    int sy = (dy >= 0) ? 1 : -1; // Step direction for y

    dx = abs(dx);
    dy = abs(dy);

    glBegin(GL_POINTS);

    if (dx > dy)
    {
        int p = 2 * dy - dx;
        for (int i = 0; i <= dx; i++)
        {
            glVertex2f(x / 100.0, y / 100.0);
            x += sx;
            if (p < 0)
                p += 2 * dy;
            else
            {
                y += sy;
                p += 2 * (dy - dx);
            }
        }
    }
    else
    {
        int p = 2 * dx - dy;
        for (int i = 0; i <= dy; i++)
        {
            glVertex2f(x / 100.0, y / 100.0);
            y += sy;
            if (p < 0)
                p += 2 * dx;
            else
            {
                x += sx;
                p += 2 * (dx - dy);
            }
        }
    }

    glEnd();
    glFlush();
}

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0); // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); // 2D orthographic view
}

int main(int argc, char** argv)
{
    printf("Enter X1: ");
    scanf("%d", &x1_);
    printf("Enter Y1: ");
    scanf("%d", &y1_);
    printf("Enter X2: ");
    scanf("%d", &x2_);
    printf("Enter Y2: ");
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
