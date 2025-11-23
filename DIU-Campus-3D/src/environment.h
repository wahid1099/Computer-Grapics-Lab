#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Draw the playground
void drawPlayground();

// Draw a tree at specified position
void drawTree(float x, float y, float z);

// Draw a bench at specified position
void drawBench(float x, float y, float z);

// Draw a street light at specified position
void drawStreetLight(float x, float y, float z);

// Draw pathways
void drawPathways();

// Draw the ground
void drawGround();

// Draw skybox
void drawSkybox();

// Draw all environment objects
void drawAllEnvironment();

#endif
