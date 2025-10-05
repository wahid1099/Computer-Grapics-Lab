

#include <GL/glut.h>
#include <cmath>

// window size
const int WIN_W = 800;
const int WIN_H = 600;

void drawHouse()
{
    // Use black for outlines
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);

    // House base (rectangle outline)
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.5f, -0.5f); // bottom-left
    glVertex2f(0.5f, -0.5f); // bottom-right
    glVertex2f(0.5f, 0.1f); // top-right
    glVertex2f(-0.5f, 0.1f); // top-left
    glEnd();

    // Roof (triangle outline)
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.6f, 0.1f);
    glVertex2f(0.0f, 0.6f);
    glVertex2f(0.6f, 0.1f);
    glEnd();

    // Door (centered, rectangle outline)
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.12f, -0.5f);
    glVertex2f(0.12f, -0.5f);
    glVertex2f(0.12f, -0.06f);
    glVertex2f(-0.12f, -0.06f);
    glEnd();

    // Door knob (small circle made of line loop)
    const float cx = 0.08f, cy = -0.28f, r = 0.01f;
    int segments = 24;
    glBegin(GL_LINE_LOOP);
    for (int i = 0;i < segments;i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        glVertex2f(cx + r * cosf(theta), cy + r * sinf(theta));
    }
    glEnd();

    // Left window (outline + panes)
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.42f, -0.12f);
    glVertex2f(-0.18f, -0.12f);
    glVertex2f(-0.18f, 0.02f);
    glVertex2f(-0.42f, 0.02f);
    glEnd();
    // panes
    glBegin(GL_LINES);
    glVertex2f(-0.42f, -0.05f); glVertex2f(-0.18f, -0.05f); // horizontal
    glVertex2f(-0.30f, -0.12f); glVertex2f(-0.30f, 0.02f); // vertical
    glEnd();

    // Right window (outline + panes)
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.18f, -0.12f);
    glVertex2f(0.42f, -0.12f);
    glVertex2f(0.42f, 0.02f);
    glVertex2f(0.18f, 0.02f);
    glEnd();
    // panes
    glBegin(GL_LINES);
    glVertex2f(0.18f, -0.05f); glVertex2f(0.42f, -0.05f); // horizontal
    glVertex2f(0.30f, -0.12f); glVertex2f(0.30f, 0.02f); // vertical
    glEnd();

    // Chimney (outline)
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.32f, 0.25f);
    glVertex2f(0.32f, 0.42f);
    glVertex2f(0.44f, 0.42f);
    glVertex2f(0.44f, 0.18f);
    glEnd();

    // Small smoke lines (curly-ish lines)
    glBegin(GL_LINE_STRIP);
    glVertex2f(0.44f, 0.42f);
    glVertex2f(0.50f, 0.48f);
    glVertex2f(0.48f, 0.54f);
    glVertex2f(0.52f, 0.58f);
    glEnd();

    // Path in front of door (two outlines)
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.12f, -0.5f);
    glVertex2f(-0.18f, -0.8f);
    glVertex2f(0.18f, -0.8f);
    glVertex2f(0.12f, -0.5f);
    glEnd();

    // Ground line
    glBegin(GL_LINES);
    glVertex2f(-1.0f, -0.8f);
    glVertex2f(1.0f, -0.8f);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // draw house centered in the scene
    drawHouse();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Keep aspect ratio and use a simple orthographic projection for 2D
    float aspect = (float)w / (float)h;
    if (aspect >= 1.0f) {
        // wider than tall
        glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
    }
    else {
        // taller than wide
        glOrtho(-1.0, 1.0, -1.0f / aspect, 1.0f / aspect, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Simple Monochrome House");

    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
