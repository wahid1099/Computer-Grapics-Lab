#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Helper function to draw a textured cube
void drawCube(float size);

// Helper function to draw a cylinder
void drawCylinder(float radius, float height, int slices = 20);

// Helper function to draw a sphere
void drawSphere(float radius, int slices = 20, int stacks = 20);

// Helper function to render 3D text
void drawText(float x, float y, float z, const char* text);

// Helper function to set material properties
void setMaterial(float r, float g, float b, float shininess = 50.0f);

// Helper function to draw a rectangular prism
void drawRectPrism(float width, float height, float depth);

#endif
