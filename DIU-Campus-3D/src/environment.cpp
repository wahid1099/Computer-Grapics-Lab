#include "environment.h"
#include "utils.h"
#include "textures.h"
#include "lighting.h"
#include <cmath>

void drawGround() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texGrass);
    setMaterial(0.2f, 0.6f, 0.2f);  // Green
    
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, 0.0f, -100.0f);
    glTexCoord2f(20.0f, 0.0f); glVertex3f(100.0f, 0.0f, -100.0f);
    glTexCoord2f(20.0f, 20.0f); glVertex3f(100.0f, 0.0f, 100.0f);
    glTexCoord2f(0.0f, 20.0f); glVertex3f(-100.0f, 0.0f, 100.0f);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawSkybox() {
    glPushMatrix();
    
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texSky);
    
    float size = 200.0f;
    
    // Set sky color based on day/night mode
    if (isNightMode) {
        glColor3f(0.1f, 0.1f, 0.2f);  // Dark blue for night
    } else {
        glColor3f(0.6f, 0.8f, 1.0f);  // Light blue for day
    }
    
    // Top face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
    glEnd();
    
    // Front face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0f, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, 0.0f, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    glEnd();
    
    // Back face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0f, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, 0.0f, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
    glEnd();
    
    // Left face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0f, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, 0.0f, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    glEnd();
    
    // Right face
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, 0.0f, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, 0.0f, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPopMatrix();
}

void drawPlayground() {
    glPushMatrix();
    glTranslatef(15.0f, 0.01f, 20.0f);
    
    // Playground ground (slightly elevated)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texGrass);
    setMaterial(0.15f, 0.5f, 0.15f);  // Darker green
    
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, 0.0f, -15.0f);
    glTexCoord2f(4.0f, 0.0f); glVertex3f(20.0f, 0.0f, -15.0f);
    glTexCoord2f(4.0f, 3.0f); glVertex3f(20.0f, 0.0f, 15.0f);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-20.0f, 0.0f, 15.0f);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    // Boundary lines (white)
    setMaterial(1.0f, 1.0f, 1.0f);
    
    // Top line
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, -15.0f);
    drawRectPrism(40.0f, 0.1f, 0.2f);
    glPopMatrix();
    
    // Bottom line
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 15.0f);
    drawRectPrism(40.0f, 0.1f, 0.2f);
    glPopMatrix();
    
    // Left line
    glPushMatrix();
    glTranslatef(-20.0f, 0.05f, 0.0f);
    drawRectPrism(0.2f, 0.1f, 30.0f);
    glPopMatrix();
    
    // Right line
    glPushMatrix();
    glTranslatef(20.0f, 0.05f, 0.0f);
    drawRectPrism(0.2f, 0.1f, 30.0f);
    glPopMatrix();
    
    // Goal posts (left)
    setMaterial(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-18.0f, 2.5f, 0.0f);
    drawRectPrism(0.3f, 5.0f, 8.0f);  // Crossbar and posts
    glPopMatrix();
    
    // Goal posts (right)
    glPushMatrix();
    glTranslatef(18.0f, 2.5f, 0.0f);
    drawRectPrism(0.3f, 5.0f, 8.0f);
    glPopMatrix();
    
    glPopMatrix();
}

void drawTree(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Trunk
    glPushMatrix();
    glTranslatef(0.0f, 2.5f, 0.0f);
    setMaterial(0.4f, 0.2f, 0.1f);  // Brown
    drawCylinder(0.5f, 5.0f);
    glPopMatrix();
    
    // Foliage (sphere)
    glPushMatrix();
    glTranslatef(0.0f, 6.5f, 0.0f);
    setMaterial(0.1f, 0.6f, 0.1f);  // Dark green
    drawSphere(2.5f);
    glPopMatrix();
    
    glPopMatrix();
}

void drawBench(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    setMaterial(0.5f, 0.3f, 0.1f);  // Brown wood
    
    // Seat
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    drawRectPrism(3.0f, 0.2f, 1.0f);
    glPopMatrix();
    
    // Backrest
    glPushMatrix();
    glTranslatef(0.0f, 1.8f, -0.4f);
    drawRectPrism(3.0f, 1.2f, 0.2f);
    glPopMatrix();
    
    // Legs (4 legs)
    float legPositions[][2] = {{-1.2f, 0.4f}, {1.2f, 0.4f}, {-1.2f, -0.4f}, {1.2f, -0.4f}};
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legPositions[i][0], 0.5f, legPositions[i][1]);
        drawCylinder(0.1f, 1.0f);
        glPopMatrix();
    }
    
    glPopMatrix();
}

void drawStreetLight(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Pole
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f);
    setMaterial(0.4f, 0.4f, 0.4f);  // Gray
    drawCylinder(0.2f, 8.0f);
    glPopMatrix();
    
    // Light bulb
    glPushMatrix();
    glTranslatef(0.0f, 8.5f, 0.0f);
    
    if (isNightMode) {
        setMaterial(1.0f, 1.0f, 0.5f);  // Yellow (lit)
    } else {
        setMaterial(0.9f, 0.9f, 0.9f);  // White (off)
    }
    
    drawSphere(0.5f);
    glPopMatrix();
    
    glPopMatrix();
}

void drawPathways() {
    setMaterial(0.5f, 0.5f, 0.5f);  // Gray
    
    // Main pathway from gate to buildings
    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -25.0f);
    drawRectPrism(8.0f, 0.1f, 50.0f);
    glPopMatrix();
    
    // Left pathway to AB-1
    glPushMatrix();
    glTranslatef(-15.0f, 0.02f, -20.0f);
    drawRectPrism(30.0f, 0.1f, 4.0f);
    glPopMatrix();
    
    // Right pathway to AB-2
    glPushMatrix();
    glTranslatef(15.0f, 0.02f, -20.0f);
    drawRectPrism(30.0f, 0.1f, 4.0f);
    glPopMatrix();
    
    // Pathway to playground
    glPushMatrix();
    glTranslatef(8.0f, 0.02f, 10.0f);
    drawRectPrism(4.0f, 0.1f, 20.0f);
    glPopMatrix();
}

void drawAllEnvironment() {
    drawGround();
    drawSkybox();
    drawPlayground();
    drawPathways();
    
    // Place trees around campus
    drawTree(-45.0f, 0.0f, -10.0f);
    drawTree(-40.0f, 0.0f, -35.0f);
    drawTree(45.0f, 0.0f, -10.0f);
    drawTree(40.0f, 0.0f, -35.0f);
    drawTree(-20.0f, 0.0f, 30.0f);
    drawTree(0.0f, 0.0f, -70.0f);
    drawTree(20.0f, 0.0f, -70.0f);
    drawTree(-10.0f, 0.0f, 15.0f);
    
    // Place benches
    drawBench(-10.0f, 0.0f, 10.0f);
    drawBench(5.0f, 0.0f, -10.0f);
    drawBench(-25.0f, 0.0f, -40.0f);
    drawBench(25.0f, 0.0f, -40.0f);
    
    // Place street lights
    drawStreetLight(-5.0f, 0.0f, 5.0f);
    drawStreetLight(5.0f, 0.0f, 5.0f);
    drawStreetLight(-5.0f, 0.0f, -30.0f);
    drawStreetLight(5.0f, 0.0f, -30.0f);
}
