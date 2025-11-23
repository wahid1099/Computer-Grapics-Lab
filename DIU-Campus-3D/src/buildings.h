#ifndef BUILDINGS_H
#define BUILDINGS_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Draw the main gate
void drawMainGate();

// Draw academic buildings
void drawAcademicBuilding1();  // AB-1
void drawAcademicBuilding2();  // AB-2
void drawAcademicBuilding3();  // AB-3

// Draw name board
void drawNameBoard();

// Helper function to draw a window
void drawWindow(float x, float y, float z, float width, float height);

#endif
