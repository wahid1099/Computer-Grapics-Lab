#include "animations.h"
#include "utils.h"
#include <cmath>

// Animation state variables
float studentPosition = 0.0f;
float flagAngle = 0.0f;
float cloudPosition = 0.0f;
bool animationPaused = false;

void drawStudent(float x, float y, float z, float walkPhase) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Head
    glPushMatrix();
    glTranslatef(0.0f, 3.5f, 0.0f);
    setMaterial(0.9f, 0.7f, 0.6f);  // Skin tone
    drawSphere(0.4f);
    glPopMatrix();
    
    // Body
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    setMaterial(0.2f, 0.3f, 0.8f);  // Blue shirt
    drawCylinder(0.3f, 1.5f);
    glPopMatrix();
    
    // Left arm
    glPushMatrix();
    glTranslatef(-0.4f, 2.5f, 0.0f);
    glRotatef(sin(walkPhase) * 30.0f, 1.0f, 0.0f, 0.0f);  // Swing with walking
    setMaterial(0.2f, 0.3f, 0.8f);
    drawCylinder(0.1f, 1.0f);
    glPopMatrix();
    
    // Right arm
    glPushMatrix();
    glTranslatef(0.4f, 2.5f, 0.0f);
    glRotatef(-sin(walkPhase) * 30.0f, 1.0f, 0.0f, 0.0f);  // Swing opposite
    setMaterial(0.2f, 0.3f, 0.8f);
    drawCylinder(0.1f, 1.0f);
    glPopMatrix();
    
    // Left leg
    glPushMatrix();
    glTranslatef(-0.2f, 0.8f, 0.0f);
    glRotatef(-sin(walkPhase) * 40.0f, 1.0f, 0.0f, 0.0f);  // Walking motion
    setMaterial(0.1f, 0.1f, 0.3f);  // Dark blue pants
    drawCylinder(0.15f, 1.2f);
    glPopMatrix();
    
    // Right leg
    glPushMatrix();
    glTranslatef(0.2f, 0.8f, 0.0f);
    glRotatef(sin(walkPhase) * 40.0f, 1.0f, 0.0f, 0.0f);  // Walking motion opposite
    setMaterial(0.1f, 0.1f, 0.3f);  // Dark blue pants
    drawCylinder(0.15f, 1.2f);
    glPopMatrix();
    
    glPopMatrix();
}

void drawFlag(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Flag pole
    glPushMatrix();
    setMaterial(0.7f, 0.7f, 0.7f);  // Gray pole
    drawCylinder(0.1f, 8.0f);
    glPopMatrix();
    
    // Flag (triangle with waving effect)
    glPushMatrix();
    glTranslatef(0.0f, 7.0f, 0.0f);
    
    setMaterial(0.8f, 0.0f, 0.0f);  // Red flag
    
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, 1.0f);
    
    // Apply sine wave for waving effect
    float wave = sin(flagAngle * 3.14159f / 180.0f) * 0.3f;
    
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(2.0f, 0.0f, wave);
    glVertex3f(0.0f, -0.5f, 0.0f);
    glEnd();
    
    glPopMatrix();
    
    glPopMatrix();
}

void drawClouds() {
    setMaterial(1.0f, 1.0f, 1.0f);  // White clouds
    
    // Cloud 1
    glPushMatrix();
    glTranslatef(cloudPosition - 50.0f, 40.0f, -80.0f);
    drawSphere(3.0f);
    glTranslatef(4.0f, 0.0f, 0.0f);
    drawSphere(3.5f);
    glTranslatef(4.0f, 0.0f, 0.0f);
    drawSphere(3.0f);
    glPopMatrix();
    
    // Cloud 2
    glPushMatrix();
    glTranslatef(cloudPosition + 30.0f, 45.0f, -70.0f);
    drawSphere(2.5f);
    glTranslatef(3.5f, 0.0f, 0.0f);
    drawSphere(3.0f);
    glTranslatef(3.5f, 0.0f, 0.0f);
    drawSphere(2.5f);
    glPopMatrix();
    
    // Cloud 3
    glPushMatrix();
    glTranslatef(cloudPosition - 80.0f, 38.0f, -60.0f);
    drawSphere(2.8f);
    glTranslatef(4.0f, 0.0f, 0.0f);
    drawSphere(3.2f);
    glTranslatef(4.0f, 0.0f, 0.0f);
    drawSphere(2.8f);
    glPopMatrix();
}

void updateAllAnimations() {
    if (animationPaused) return;
    
    // Update student walking position
    studentPosition += 0.15f;
    if (studentPosition > 360.0f) {
        studentPosition = 0.0f;
    }
    
    // Update flag waving
    flagAngle += 5.0f;
    if (flagAngle >= 360.0f) {
        flagAngle = 0.0f;
    }
    
    // Update cloud movement
    cloudPosition += 0.08f;
    if (cloudPosition > 200.0f) {
        cloudPosition = -100.0f;
    }
}

void drawAllAnimations() {
    // Draw walking students around playground
    float radius = 25.0f;
    
    // Student 1
    float angle1 = studentPosition * 3.14159f / 180.0f;
    float x1 = 15.0f + radius * cos(angle1);
    float z1 = 20.0f + radius * sin(angle1);
    drawStudent(x1, 0.0f, z1, studentPosition * 3.14159f / 180.0f);
    
    // Student 2 (90 degrees behind)
    float angle2 = (studentPosition - 90.0f) * 3.14159f / 180.0f;
    float x2 = 15.0f + radius * cos(angle2);
    float z2 = 20.0f + radius * sin(angle2);
    drawStudent(x2, 0.0f, z2, (studentPosition - 90.0f) * 3.14159f / 180.0f);
    
    // Student 3 (180 degrees behind)
    float angle3 = (studentPosition - 180.0f) * 3.14159f / 180.0f;
    float x3 = 15.0f + radius * cos(angle3);
    float z3 = 20.0f + radius * sin(angle3);
    drawStudent(x3, 0.0f, z3, (studentPosition - 180.0f) * 3.14159f / 180.0f);
    
    // Draw flag at corner of playground
    drawFlag(35.0f, 0.0f, 5.0f);
    
    // Draw clouds
    drawClouds();
}
