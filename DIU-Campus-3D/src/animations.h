#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Animation state variables
extern float studentPosition;
extern float flagAngle;
extern float cloudPosition;
extern bool animationPaused;

// Draw a student (stick figure) at position
void drawStudent(float x, float y, float z, float walkPhase);

// Draw the flag with waving animation
void drawFlag(float x, float y, float z);

// Draw clouds
void drawClouds();

// Update all animations
void updateAllAnimations();

// Draw all animated objects
void drawAllAnimations();

#endif
