#include "buildings.h"
#include "utils.h"
#include "textures.h"
#include <cmath>

void drawWindow(float x, float y, float z, float width, float height) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texWindow);
    setMaterial(0.6f, 0.8f, 1.0f);
    
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width/2, -height/2, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width/2, -height/2, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width/2, height/2, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width/2, height/2, 0.0f);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawMainGate() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    
    // Left pillar
    glPushMatrix();
    glTranslatef(-8.0f, 5.0f, 0.0f);
    setMaterial(0.0f, 0.5f, 0.0f);  // Dark green
    drawRectPrism(2.0f, 10.0f, 2.0f);
    glPopMatrix();
    
    // Right pillar
    glPushMatrix();
    glTranslatef(8.0f, 5.0f, 0.0f);
    setMaterial(0.0f, 0.5f, 0.0f);  // Dark green
    drawRectPrism(2.0f, 10.0f, 2.0f);
    glPopMatrix();
    
    // Top arch/beam
    glPushMatrix();
    glTranslatef(0.0f, 10.0f, 0.0f);
    setMaterial(1.0f, 1.0f, 1.0f);  // White
    drawRectPrism(18.0f, 1.5f, 2.0f);
    glPopMatrix();
    
    // DIU Logo board on top
    glPushMatrix();
    glTranslatef(0.0f, 11.5f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texDIULogo);
    setMaterial(1.0f, 1.0f, 1.0f);
    drawRectPrism(6.0f, 2.0f, 0.5f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // Gate text
    glColor3f(0.0f, 0.4f, 0.0f);
    drawText(-3.0f, 12.0f, 1.0f, "DIU GATE");
    
    glPopMatrix();
}

void drawAcademicBuilding1() {
    glPushMatrix();
    glTranslatef(-30.0f, 0.0f, -20.0f);
    
    // Main building structure
    glPushMatrix();
    glTranslatef(0.0f, 10.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texWall);
    setMaterial(0.85f, 0.85f, 0.75f);  // Light beige
    drawRectPrism(15.0f, 20.0f, 10.0f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // Windows - front face
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            float wx = -5.0f + col * 5.0f;
            float wy = 5.0f + row * 4.0f;
            drawWindow(wx, wy, 5.1f, 2.0f, 2.5f);
        }
    }
    
    // Door
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 5.1f);
    setMaterial(0.3f, 0.2f, 0.1f);  // Dark brown
    drawRectPrism(2.5f, 3.0f, 0.2f);
    glPopMatrix();
    
    // Building label
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(-2.0f, 22.0f, 5.5f, "AB-1");
    
    glPopMatrix();
}

void drawAcademicBuilding2() {
    glPushMatrix();
    glTranslatef(30.0f, 0.0f, -20.0f);
    
    // Main building structure (taller)
    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texWall);
    setMaterial(0.75f, 0.75f, 0.8f);  // Light gray-blue
    drawRectPrism(12.0f, 30.0f, 10.0f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // Windows - front face (more floors)
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 2; col++) {
            float wx = -3.0f + col * 6.0f;
            float wy = 5.0f + row * 4.5f;
            drawWindow(wx, wy, 5.1f, 2.5f, 3.0f);
        }
    }
    
    // Door
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 5.1f);
    setMaterial(0.3f, 0.2f, 0.1f);  // Dark brown
    drawRectPrism(2.5f, 3.0f, 0.2f);
    glPopMatrix();
    
    // Building label
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(-2.0f, 32.0f, 5.5f, "AB-2");
    
    glPopMatrix();
}

void drawAcademicBuilding3() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -50.0f);
    
    // Main building structure (wide)
    glPushMatrix();
    glTranslatef(0.0f, 7.5f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texWall);
    setMaterial(0.8f, 0.75f, 0.7f);  // Light tan
    drawRectPrism(25.0f, 15.0f, 12.0f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // Windows - front face
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 5; col++) {
            float wx = -10.0f + col * 5.0f;
            float wy = 4.0f + row * 4.0f;
            drawWindow(wx, wy, 6.1f, 2.0f, 2.5f);
        }
    }
    
    // Main entrance door
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 6.1f);
    setMaterial(0.3f, 0.2f, 0.1f);  // Dark brown
    drawRectPrism(3.5f, 4.0f, 0.2f);
    glPopMatrix();
    
    // Stairs (3 steps)
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(0.0f, i * 0.3f, 6.5f + i * 0.5f);
        setMaterial(0.5f, 0.5f, 0.5f);  // Gray
        drawRectPrism(8.0f, 0.3f, 0.5f);
        glPopMatrix();
    }
    
    // Building label
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(-2.0f, 17.0f, 6.5f, "AB-3");
    
    glPopMatrix();
}

void drawNameBoard() {
    glPushMatrix();
    glTranslatef(-15.0f, 3.0f, 5.0f);
    
    // Board support poles
    glPushMatrix();
    glTranslatef(-8.0f, -1.5f, 0.0f);
    setMaterial(0.3f, 0.3f, 0.3f);  // Dark gray
    drawCylinder(0.2f, 3.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(8.0f, -1.5f, 0.0f);
    setMaterial(0.3f, 0.3f, 0.3f);  // Dark gray
    drawCylinder(0.2f, 3.0f);
    glPopMatrix();
    
    // Board
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texDIULogo);
    setMaterial(1.0f, 1.0f, 1.0f);  // White background
    drawRectPrism(16.0f, 3.0f, 0.3f);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // Text on board
    glColor3f(0.0f, 0.5f, 0.0f);  // Green text
    drawText(-7.5f, 0.5f, 0.3f, "Daffodil International");
    drawText(-5.0f, -0.5f, 0.3f, "University");
    
    glPopMatrix();
}
