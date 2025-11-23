#ifndef CAMERA_H
#define CAMERA_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

class Camera {
private:
    float posX, posY, posZ;      // Camera position
    float yaw, pitch;             // Camera rotation angles (in degrees)
    float moveSpeed;              // Movement speed
    float rotateSpeed;            // Rotation speed
    
    // Initial position for reset
    float initialX, initialY, initialZ;
    float initialYaw, initialPitch;

public:
    Camera();
    
    // Movement functions
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
    // Rotation functions
    void rotateLeft();
    void rotateRight();
    void rotateUp();
    void rotateDown();
    
    // Apply camera transformation
    void apply();
    
    // Reset camera to initial position
    void reset();
    
    // Getters
    float getX() const { return posX; }
    float getY() const { return posY; }
    float getZ() const { return posZ; }
};

#endif
