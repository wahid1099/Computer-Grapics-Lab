#include<windows.h> 
#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 

#include <stdlib.h> 
#include <math.h> 

float p = -10.0;   // starting position 
float speed = 0.05; // movement speed 
int paused = 0;     // pause flag 

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(p, 3);
    glVertex2f(p + 3, 3);
    glVertex2f(p + 3, -3);
    glVertex2f(p, -3);
    glEnd();

    glFlush();
}

void timer(int value)
{
    if (!paused) {      // move only when not paused 
        p += speed;

        if (p > 10)     // wrap around 
            p = -13;
        if (p < -13)
            p = 10;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS 
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'p':   // pause/unpause 
        paused = !paused;
        break;

    case 'r':   // reset position 
        p = -10;
        break;

    case 'a':   // move left 
        p -= 0.3;
        break;

    case 'd':   // move right 
        p += 0.3;
        break;

    case '+':   // increase speed 
        speed += 0.01;
        break;

    case '-':   // decrease speed 
        speed -= 0.01;
        if (speed < 0) speed = 0;
        break;

    default:
        break;
    }
}

void init(void)
{
    glClearColor(1.0, 0.0, 0.0, 0.0);
    glOrtho(-10, 10, -10, 10, -10, 10);
}

// ... other code ...

int main(int argc, char** argv) // IMPORTANT: argc and argv are required here
{
    // ----------------------------------------------------
    // 💡 FIX: Initialize GLUT before calling any other GLUT function.
    glutInit(&argc, argv);
    // ----------------------------------------------------

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Moving Object");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}