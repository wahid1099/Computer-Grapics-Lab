#include <GL/glut.h>
#include <cmath>

// Define the mathematical constant PI if it's not already available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function to draw the hexagon for the tree canopy
void drawHexagon(float cx, float cy, float radius) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 6; i++) {
        // Calculate the angle for each vertex
        float angle = 2.0f * M_PI * float(i) / 6.0f;
        // Calculate the x and y coordinates of the vertex
        float x = cx + radius * cos(angle);
        float y = cy + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// The main display function where all the drawing happens
void display() {
    // Clear the window with white color (the whiteboard)
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the drawing color to black (the marker)
    glColor3f(0.0f, 0.0f, 0.0f);

    // Set the line thickness
    glLineWidth(2.0f);

    // --- Draw the outer frame ---
    glBegin(GL_LINE_LOOP);
    glVertex2f(50.0f, 50.0f);
    glVertex2f(750.0f, 50.0f);
    glVertex2f(750.0f, 550.0f);
    glVertex2f(50.0f, 550.0f);
    glEnd();

    // --- Draw the ground line ---
    glBegin(GL_LINES);
    glVertex2f(50.0f, 150.0f);
    glVertex2f(750.0f, 150.0f);
    glEnd();

    // --- Draw the mountains in the background ---
    // Left mountains
    glBegin(GL_LINE_LOOP);
    glVertex2f(50.0f, 150.0f);
    glVertex2f(125.0f, 250.0f);
    glVertex2f(200.0f, 150.0f);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(160.0f, 150.0f);
    glVertex2f(210.0f, 220.0f);
    glVertex2f(260.0f, 150.0f);
    glEnd();
    // Right mountains
    glBegin(GL_LINE_LOOP);
    glVertex2f(650.0f, 150.0f);
    glVertex2f(690.0f, 210.0f);
    glVertex2f(750.0f, 150.0f);
    glEnd();

    // --- Draw the house ---
    // House body (rectangle)
    glBegin(GL_LINE_LOOP);
    glVertex2f(270.0f, 150.0f);
    glVertex2f(470.0f, 150.0f);
    glVertex2f(470.0f, 300.0f);
    glVertex2f(270.0f, 300.0f);
    glEnd();
    // Roof (triangle)
    glBegin(GL_LINE_LOOP);
    glVertex2f(260.0f, 300.0f);
    glVertex2f(480.0f, 300.0f);
    glVertex2f(370.0f, 400.0f);
    glEnd();
    // Door (rectangle)
    glBegin(GL_LINE_LOOP);
    glVertex2f(350.0f, 150.0f);
    glVertex2f(390.0f, 150.0f);
    glVertex2f(390.0f, 240.0f);
    glVertex2f(350.0f, 240.0f);
    glEnd();
    // Windows (squares)
    glBegin(GL_LINE_LOOP); // Left window
    glVertex2f(290.0f, 210.0f);
    glVertex2f(320.0f, 210.0f);
    glVertex2f(320.0f, 240.0f);
    glVertex2f(290.0f, 240.0f);
    glEnd();
    glBegin(GL_LINE_LOOP); // Right window
    glVertex2f(420.0f, 210.0f);
    glVertex2f(450.0f, 210.0f);
    glVertex2f(450.0f, 240.0f);
    glVertex2f(420.0f, 240.0f);
    glEnd();

    // --- Draw the tree ---
    // Trunk (rectangle)
    glBegin(GL_LINE_LOOP);
    glVertex2f(590.0f, 150.0f);
    glVertex2f(610.0f, 150.0f);
    glVertex2f(610.0f, 280.0f);
    glVertex2f(590.0f, 280.0f);
    glEnd();
    // Base of the trunk
    glBegin(GL_LINES);
    glVertex2f(580.0f, 150.0f);
    glVertex2f(620.0f, 150.0f);
    glEnd();
    // Canopy (hexagon)
    drawHexagon(600.0f, 340.0f, 60.0f);

    // --- Draw the path ---
    glBegin(GL_LINES);
    glVertex2f(180.0f, 50.0f);
    glVertex2f(300.0f, 150.0f);
    glEnd();

    // --- Draw the sky objects (simple birds) ---
    // Left bird
    glBegin(GL_LINE_STRIP);
    glVertex2f(220.0f, 480.0f);
    glVertex2f(235.0f, 490.0f);
    glVertex2f(250.0f, 480.0f);
    glEnd();
    // Right bird
    glBegin(GL_LINE_STRIP);
    glVertex2f(520.0f, 490.0f);
    glVertex2f(535.0f, 500.0f);
    glVertex2f(550.0f, 490.0f);
    glEnd();

    // Send all drawing commands to be executed
    glFlush();
}

// Initialization function
void init() {
    // Set the clear color to white (R, G, B, alpha)
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set up a 2D orthographic viewing region
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}

// The main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);                         // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Set display mode
    glutInitWindowSize(800, 600);                  // Set window size
    glutInitWindowPosition(100, 100);              // Set window position
    glutCreateWindow("Scenery from Whiteboard");   // Create window with a title
    init();                                        // Call initialization function
    glutDisplayFunc(display);                      // Register display callback
    glutMainLoop();                                // Enter the GLUT event processing loop
    return 0;
}