#include "camera.h"

#define PI 3.14159265359

Camera::Camera() {
    // Start camera at a good viewing position
    posX = 0.0f;
    posY = 15.0f;  // Elevated to see the campus
    posZ = 60.0f;  // Back from the gate
    
    yaw = 180.0f;   // Looking forward (towards gate)
    pitch = -10.0f; // Slightly looking down
    
    moveSpeed = 0.8f;
    rotateSpeed = 2.0f;
    
    // Save initial position
    initialX = posX;
    initialY = posY;
    initialZ = posZ;
    initialYaw = yaw;
    initialPitch = pitch;
}

void Camera::moveForward() {
    float radYaw = yaw * PI / 180.0f;
    posX += sin(radYaw) * moveSpeed;
    posZ -= cos(radYaw) * moveSpeed;
}

void Camera::moveBackward() {
    float radYaw = yaw * PI / 180.0f;
    posX -= sin(radYaw) * moveSpeed;
    posZ += cos(radYaw) * moveSpeed;
}

void Camera::moveLeft() {
    float radYaw = yaw * PI / 180.0f;
    posX -= cos(radYaw) * moveSpeed;
    posZ -= sin(radYaw) * moveSpeed;
}

void Camera::moveRight() {
    float radYaw = yaw * PI / 180.0f;
    posX += cos(radYaw) * moveSpeed;
    posZ += sin(radYaw) * moveSpeed;
}

void Camera::moveUp() {
    posY += moveSpeed;
}

void Camera::moveDown() {
    if (posY > 2.0f) {  // Don't go below ground level
        posY -= moveSpeed;
    }
}

void Camera::rotateLeft() {
    yaw -= rotateSpeed;
    if (yaw < 0.0f) yaw += 360.0f;
}

void Camera::rotateRight() {
    yaw += rotateSpeed;
    if (yaw >= 360.0f) yaw -= 360.0f;
}

void Camera::rotateUp() {
    pitch += rotateSpeed;
    if (pitch > 89.0f) pitch = 89.0f;  // Limit looking up
}

void Camera::rotateDown() {
    pitch -= rotateSpeed;
    if (pitch < -89.0f) pitch = -89.0f;  // Limit looking down
}

void Camera::apply() {
    glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-posX, -posY, -posZ);
}

void Camera::reset() {
    posX = initialX;
    posY = initialY;
    posZ = initialZ;
    yaw = initialYaw;
    pitch = initialPitch;
}
