#include "utils.h"
#include <cmath>

void drawCube(float size) {
    float half = size / 2.0f;
    
    glBegin(GL_QUADS);
    
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-half, -half, half);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(half, -half, half);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(half, half, half);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-half, half, half);
    
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-half, -half, -half);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-half, half, -half);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(half, half, -half);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(half, -half, -half);
    
    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-half, half, -half);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-half, half, half);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(half, half, half);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(half, half, -half);
    
    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-half, -half, -half);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(half, -half, -half);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(half, -half, half);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-half, -half, half);
    
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(half, -half, -half);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(half, half, -half);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(half, half, half);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(half, -half, half);
    
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-half, -half, -half);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-half, -half, half);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-half, half, half);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-half, half, -half);
    
    glEnd();
}

void drawCylinder(float radius, float height, int slices) {
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);
    
    glPushMatrix();
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadric, radius, radius, height, slices, 1);
    
    // Draw bottom cap
    gluDisk(quadric, 0.0f, radius, slices, 1);
    
    // Draw top cap
    glTranslatef(0.0f, 0.0f, height);
    gluDisk(quadric, 0.0f, radius, slices, 1);
    glPopMatrix();
    
    gluDeleteQuadric(quadric);
}

void drawSphere(float radius, int slices, int stacks) {
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);
    
    gluSphere(quadric, radius, slices, stacks);
    
    gluDeleteQuadric(quadric);
}

void drawText(float x, float y, float z, const char* text) {
    glRasterPos3f(x, y, z);
    
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

void setMaterial(float r, float g, float b, float shininess) {
    GLfloat mat_ambient[] = { r * 0.3f, g * 0.3f, b * 0.3f, 1.0f };
    GLfloat mat_diffuse[] = { r, g, b, 1.0f };
    GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_shininess[] = { shininess };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void drawRectPrism(float width, float height, float depth) {
    float hw = width / 2.0f;
    float hh = height / 2.0f;
    float hd = depth / 2.0f;
    
    glBegin(GL_QUADS);
    
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hw, -hh, hd);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(hw, -hh, hd);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(hw, hh, hd);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-hw, hh, hd);
    
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-hw, -hh, -hd);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-hw, hh, -hd);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(hw, hh, -hd);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(hw, -hh, -hd);
    
    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-hw, hh, -hd);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hw, hh, hd);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(hw, hh, hd);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(hw, hh, -hd);
    
    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-hw, -hh, -hd);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(hw, -hh, -hd);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(hw, -hh, hd);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-hw, -hh, hd);
    
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(hw, -hh, -hd);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(hw, hh, -hd);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(hw, hh, hd);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(hw, -hh, hd);
    
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-hw, -hh, -hd);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-hw, -hh, hd);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-hw, hh, hd);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-hw, hh, -hd);
    
    glEnd();
}
