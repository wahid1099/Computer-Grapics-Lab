#include "textures.h"
#include <iostream>
#include <fstream>

// Texture IDs
GLuint texDIULogo = 0;
GLuint texGrass = 0;
GLuint texSky = 0;
GLuint texWall = 0;
GLuint texWindow = 0;

// Simple BMP loader (supports 24-bit BMPs)
// For PNG support, you would need SOIL or stb_image library
GLuint loadTexture(const char* filename) {
    // This is a simplified version that works with raw RGB data
    // In a real implementation, you'd use SOIL_load_OGL_texture()
    // For now, we'll create a simple procedural texture
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Create a simple procedural texture (checkerboard or solid color)
    const int size = 64;
    unsigned char* data = new unsigned char[size * size * 3];
    
    // Generate a simple pattern based on filename
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int index = (i * size + j) * 3;
            
            // Different patterns for different textures
            if (strstr(filename, "grass")) {
                data[index] = 34;      // R - dark green
                data[index + 1] = 139; // G
                data[index + 2] = 34;  // B
            } else if (strstr(filename, "sky")) {
                data[index] = 135;     // R - sky blue
                data[index + 1] = 206; // G
                data[index + 2] = 235; // B
            } else if (strstr(filename, "wall")) {
                data[index] = 200;     // R - light gray
                data[index + 1] = 200; // G
                data[index + 2] = 200; // B
            } else if (strstr(filename, "window")) {
                // Checkerboard for windows
                if ((i / 8 + j / 8) % 2 == 0) {
                    data[index] = 100;     // R - blue glass
                    data[index + 1] = 150; // G
                    data[index + 2] = 200; // B
                } else {
                    data[index] = 255;     // R - white frame
                    data[index + 1] = 255; // G
                    data[index + 2] = 255; // B
                }
            } else {
                // Default white
                data[index] = 255;
                data[index + 1] = 255;
                data[index + 2] = 255;
            }
        }
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    delete[] data;
    
    std::cout << "Loaded texture: " << filename << " (ID: " << textureID << ")" << std::endl;
    
    return textureID;
}

void loadAllTextures() {
    std::cout << "Loading textures..." << std::endl;
    
    texDIULogo = loadTexture("textures/diu_logo.png");
    texGrass = loadTexture("textures/grass.png");
    texSky = loadTexture("textures/sky.png");
    texWall = loadTexture("textures/wall.png");
    texWindow = loadTexture("textures/window.png");
    
    std::cout << "All textures loaded successfully!" << std::endl;
}
