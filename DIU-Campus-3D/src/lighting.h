#ifndef LIGHTING_H
#define LIGHTING_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Initialize lighting system
void initLighting();

// Set day mode lighting
void setDayMode();

// Set night mode lighting
void setNightMode();

// Global variable to track day/night state
extern bool isNightMode;

#endif
