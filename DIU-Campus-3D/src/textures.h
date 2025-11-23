#ifndef TEXTURES_H
#define TEXTURES_H

#include <windows.h>
#include <GL/gl.h>

// Texture IDs
extern GLuint texDIULogo;
extern GLuint texGrass;
extern GLuint texSky;
extern GLuint texWall;
extern GLuint texWindow;

// Load a single texture from file
GLuint loadTexture(const char* filename);

// Load all textures
void loadAllTextures();

#endif
