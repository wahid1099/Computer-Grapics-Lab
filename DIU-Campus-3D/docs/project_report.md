# 3D DIU Mini Campus - Project Report

**Course**: Computer Graphics Lab  
**Project Title**: 3D Simulation of Daffodil International University Campus  
**Technology**: C++, OpenGL, FreeGLUT  
**Date**: November 2025

---

## Table of Contents

1. [Abstract](#abstract)
2. [Introduction](#introduction)
3. [Objectives](#objectives)
4. [Literature Review](#literature-review)
5. [System Design](#system-design)
6. [Implementation](#implementation)
7. [Features](#features)
8. [Testing and Results](#testing-and-results)
9. [Challenges and Solutions](#challenges-and-solutions)
10. [Conclusion](#conclusion)
11. [Future Work](#future-work)
12. [References](#references)

---

## 1. Abstract

This project presents an interactive 3D simulation of Daffodil International University (DIU) campus developed using C++ and OpenGL. The application features realistic campus structures including the main gate, three academic buildings (AB-1, AB-2, AB-3), a football playground, and various environment objects. The simulation includes animated elements such as walking students, a waving flag, and moving clouds. Users can explore the campus using a first-person camera with full movement and rotation controls. The project demonstrates core computer graphics concepts including 3D transformations, lighting, texturing, and animation.

**Keywords**: OpenGL, 3D Graphics, Computer Graphics, Campus Simulation, C++, FreeGLUT

---

## 2. Introduction

### 2.1 Background

Computer graphics has become an essential tool for creating realistic simulations and visualizations. Educational institutions can benefit from 3D campus simulations for virtual tours, orientation programs, and promotional materials. This project aims to create an interactive 3D representation of Daffodil International University campus.

### 2.2 Motivation

The motivation for this project stems from:
- Demonstrating practical application of computer graphics concepts
- Creating a virtual tour platform for prospective students
- Showcasing DIU campus in an interactive format
- Learning advanced OpenGL programming techniques

### 2.3 Scope

The project covers:
- 3D modeling of campus buildings and structures
- Implementation of lighting and texturing
- Animation of dynamic objects
- Interactive camera controls
- Day/night mode simulation

---

## 3. Objectives

### Primary Objectives
1. Create a realistic 3D model of DIU campus
2. Implement interactive camera navigation
3. Add realistic lighting and textures
4. Animate dynamic elements (students, flag, clouds)
5. Provide day/night mode functionality

### Secondary Objectives
1. Optimize rendering performance
2. Create modular, maintainable code
3. Implement user-friendly controls
4. Generate comprehensive documentation

---

## 4. Literature Review

### 4.1 OpenGL Graphics Pipeline

OpenGL (Open Graphics Library) is a cross-platform API for rendering 2D and 3D graphics. The graphics pipeline consists of:

1. **Vertex Processing**: Transform vertices using modelview and projection matrices
2. **Primitive Assembly**: Assemble vertices into geometric primitives
3. **Rasterization**: Convert primitives to fragments
4. **Fragment Processing**: Apply textures, lighting, and colors
5. **Per-Fragment Operations**: Depth testing, blending, and writing to framebuffer

### 4.2 3D Transformations

Three fundamental transformations are used:
- **Translation**: Moving objects in 3D space using `glTranslatef()`
- **Rotation**: Rotating objects around axes using `glRotatef()`
- **Scaling**: Resizing objects using `glScalef()`

These transformations are applied using transformation matrices managed by OpenGL's matrix stack (`glPushMatrix()` and `glPopMatrix()`).

### 4.3 Lighting Models

The Phong lighting model is used, consisting of:
- **Ambient Light**: Uniform background illumination
- **Diffuse Light**: Directional light scattered by rough surfaces
- **Specular Light**: Reflective highlights on shiny surfaces

### 4.4 Texture Mapping

Texture mapping applies 2D images onto 3D surfaces to add detail without increasing geometric complexity. Texture coordinates (u, v) map image pixels to surface points.

---

## 5. System Design

### 5.1 Architecture

The system follows a modular architecture with separated concerns:

```
┌─────────────────────────────────────┐
│         Main Program (main.cpp)      │
│  - OpenGL Initialization             │
│  - Event Loop                        │
│  - Callback Registration             │
└──────────────┬──────────────────────┘
               │
       ┌───────┴───────┐
       │               │
┌──────▼──────┐  ┌────▼─────────┐
│   Camera    │  │   Lighting   │
│   System    │  │   System     │
└─────────────┘  └──────────────┘
       │               │
       └───────┬───────┘
               │
    ┌──────────┼──────────┐
    │          │          │
┌───▼────┐ ┌──▼─────┐ ┌─▼────────┐
│Buildings│ │Environ-│ │Animation │
│         │ │ment    │ │          │
└─────────┘ └────────┘ └──────────┘
```

### 5.2 Component Diagram

**Core Components**:

1. **Camera System** (`camera.h/cpp`)
   - First-person camera implementation
   - Movement: forward, backward, left, right, up, down
   - Rotation: yaw and pitch control
   - Reset functionality

2. **Lighting System** (`lighting.h/cpp`)
   - Day mode: bright ambient and diffuse lighting
   - Night mode: dim lighting with building lights
   - Smooth transitions between modes

3. **Texture System** (`textures.h/cpp`)
   - Texture loading and management
   - Procedural fallback textures
   - Texture binding for objects

4. **Buildings Module** (`buildings.h/cpp`)
   - Main gate with DIU logo
   - Academic buildings (AB-1, AB-2, AB-3)
   - Windows, doors, and architectural details
   - Name board

5. **Environment Module** (`environment.h/cpp`)
   - Ground with grass texture
   - Skybox
   - Playground with goal posts
   - Trees, benches, street lights
   - Pathways

6. **Animation Module** (`animations.h/cpp`)
   - Walking students with limb movement
   - Waving flag
   - Moving clouds
   - Animation state management

7. **Utilities** (`utils.h/cpp`)
   - Helper functions for drawing primitives
   - Material property setting
   - Text rendering

### 5.3 Data Flow

```
User Input (Keyboard)
        ↓
Event Handlers (keyboard, specialKeys)
        ↓
Camera Update / Mode Toggle / Animation Control
        ↓
Timer Callback (60 FPS)
        ↓
Update Animations
        ↓
Display Function
        ↓
Render Scene (Skybox → Environment → Buildings → Animations)
        ↓
Swap Buffers (Display to Screen)
```

---

## 6. Implementation

### 6.1 Camera Implementation

The camera uses a first-person perspective with yaw and pitch angles:

```cpp
void Camera::apply() {
    glRotatef(-pitch, 1.0f, 0.0f, 0.0f);  // Pitch rotation
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);    // Yaw rotation
    glTranslatef(-posX, -posY, -posZ);     // Position
}
```

Movement calculations use trigonometry:
- Forward: `x += sin(yaw) * speed`, `z -= cos(yaw) * speed`
- Strafe: `x += cos(yaw) * speed`, `z += sin(yaw) * speed`

### 6.2 Lighting Implementation

Day mode lighting:
```cpp
GLfloat ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat diffuse[] = { 0.9f, 0.9f, 0.8f, 1.0f };
glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
```

Night mode reduces ambient and diffuse values for darker atmosphere.

### 6.3 Building Construction

Buildings are constructed using rectangular prisms with windows:

```cpp
void drawAcademicBuilding1() {
    // Main structure
    drawRectPrism(15.0f, 20.0f, 10.0f);
    
    // Windows in grid pattern
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            drawWindow(x, y, z, width, height);
        }
    }
}
```

### 6.4 Animation Implementation

Student walking animation uses circular path:
```cpp
float angle = studentPosition * PI / 180.0f;
float x = centerX + radius * cos(angle);
float z = centerZ + radius * sin(angle);
drawStudent(x, 0.0f, z, angle);
```

Limb movement uses sine waves:
```cpp
glRotatef(sin(walkPhase) * 40.0f, 1.0f, 0.0f, 0.0f);  // Leg swing
```

Flag waving:
```cpp
float wave = sin(flagAngle * PI / 180.0f) * 0.3f;
glVertex3f(2.0f, 0.0f, wave);  // Waving effect
```

### 6.5 Texture Mapping

Textures are applied with coordinates:
```cpp
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, texGrass);
glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y1, z1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, y2, z2);
    // ...
glEnd();
```

---

## 7. Features

### 7.1 Campus Structures

#### Main Gate
- Two green pillars (2×10×2 units)
- White arch beam (18×1.5×2 units)
- DIU logo board on top
- Green and white color scheme

#### Academic Buildings

**AB-1**:
- Dimensions: 15×20×10 units
- 4 floors with 3 windows per floor
- Front entrance door
- Light beige color

**AB-2**:
- Dimensions: 12×30×10 units (tallest)
- 6 floors with 2 windows per floor
- Gray-blue color
- Modern appearance

**AB-3**:
- Dimensions: 25×15×12 units (widest)
- 3 floors with 5 windows per floor
- Entrance stairs (3 steps)
- Light tan color

#### Name Board
- 16×3 units board
- Support poles
- "Daffodil International University" text
- DIU logo

### 7.2 Environment Objects

#### Playground
- 40×30 units grass field
- White boundary lines
- Two goal posts (H-shaped structure)
- Flag in corner

#### Trees
- Brown cylinder trunk (height: 5 units, radius: 0.5)
- Green sphere foliage (radius: 2.5 units)
- 8 trees placed around campus

#### Benches
- Wooden seat and backrest
- 4 legs
- 4 benches near buildings

#### Street Lights
- Gray pole (height: 8 units)
- Yellow bulb (lights up at night)
- 4 lights along pathways

#### Pathways
- Gray concrete paths
- Connect all major buildings
- Main pathway from gate to buildings

### 7.3 Animations

#### Walking Students
- 3 students walking in circular path
- Realistic limb movement (arms and legs swing)
- Stick figure design with spheres and cylinders
- Continuous loop around playground

#### Waving Flag
- Red triangular flag
- Sine wave deformation
- Smooth waving motion
- Positioned at playground corner

#### Moving Clouds
- 3 cloud groups (white spheres)
- Slow horizontal movement
- High altitude positioning
- Continuous loop across sky

### 7.4 Interactive Controls

#### Camera Movement
- **W/S**: Forward/Backward
- **A/D**: Left/Right strafe
- **Q/E**: Up/Down vertical movement
- **Arrow Keys**: Look around (yaw and pitch)

#### Mode Controls
- **N**: Toggle day/night mode
- **P**: Pause/resume animations
- **R**: Reset camera to initial position
- **ESC**: Exit application

### 7.5 Day/Night Mode

**Day Mode**:
- Bright blue sky (RGB: 0.53, 0.81, 0.92)
- Strong ambient lighting (0.4, 0.4, 0.4)
- Bright diffuse lighting (0.9, 0.9, 0.8)
- Street lights off

**Night Mode**:
- Dark blue sky (RGB: 0.05, 0.05, 0.15)
- Dim ambient lighting (0.1, 0.1, 0.15)
- Weak diffuse lighting (0.2, 0.2, 0.3)
- Street lights on (yellow glow)

---

## 8. Testing and Results

### 8.1 Test Cases

| Test Case | Description | Expected Result | Status |
|-----------|-------------|-----------------|--------|
| TC-01 | Compile project with Makefile | Clean compilation, no errors | ✅ Pass |
| TC-02 | Launch application | Window opens, displays campus | ✅ Pass |
| TC-03 | Camera forward movement (W) | Camera moves forward smoothly | ✅ Pass |
| TC-04 | Camera rotation (Arrow keys) | View rotates correctly | ✅ Pass |
| TC-05 | Toggle day/night (N) | Lighting changes, sky darkens | ✅ Pass |
| TC-06 | Pause animations (P) | Students, flag, clouds stop | ✅ Pass |
| TC-07 | Reset camera (R) | Returns to initial position | ✅ Pass |
| TC-08 | Student walking | Students walk in circle | ✅ Pass |
| TC-09 | Flag waving | Flag waves smoothly | ✅ Pass |
| TC-10 | Cloud movement | Clouds drift across sky | ✅ Pass |
| TC-11 | Texture loading | Textures display correctly | ✅ Pass |
| TC-12 | Building rendering | All buildings visible | ✅ Pass |

### 8.2 Performance Metrics

- **Frame Rate**: 60 FPS (stable)
- **Window Resolution**: 1280×720 pixels
- **Polygon Count**: ~5,000 polygons
- **Texture Memory**: ~10 MB
- **Startup Time**: <1 second

### 8.3 Visual Results

The application successfully renders:
- ✅ Main gate with DIU logo
- ✅ Three academic buildings with windows
- ✅ Playground with goal posts and flag
- ✅ 8 trees with foliage
- ✅ 4 benches
- ✅ 4 street lights
- ✅ Pathways connecting buildings
- ✅ 3 walking students
- ✅ Waving flag
- ✅ Moving clouds
- ✅ Day and night modes

---

## 9. Challenges and Solutions

### Challenge 1: Camera Movement Precision
**Problem**: Camera movement felt jerky and imprecise.  
**Solution**: Implemented continuous key state tracking instead of single key press events. Used timer callback to process keys at 60 FPS.

### Challenge 2: Texture Loading
**Problem**: SOIL library dependency complicated compilation.  
**Solution**: Implemented procedural texture generation as fallback. Textures are created programmatically if image files aren't available.

### Challenge 3: Animation Smoothness
**Problem**: Animations appeared choppy.  
**Solution**: Used sine and cosine functions for smooth interpolation. Implemented proper timing with `glutTimerFunc()` at 16ms intervals (60 FPS).

### Challenge 4: Lighting in Night Mode
**Problem**: Scene was too dark in night mode.  
**Solution**: Balanced ambient and diffuse lighting values. Added glowing street lights for visibility.

### Challenge 5: Student Limb Movement
**Problem**: Walking students looked static.  
**Solution**: Applied rotation transformations to arms and legs using sine waves synchronized with walking position.

### Challenge 6: Z-Fighting on Ground
**Problem**: Playground and ground textures flickered.  
**Solution**: Slightly elevated playground (y = 0.01) and pathways (y = 0.02) above ground level.

---

## 10. Conclusion

This project successfully demonstrates the application of computer graphics concepts to create an interactive 3D simulation of DIU campus. The implementation showcases:

1. **3D Modeling**: Realistic representation of campus structures
2. **Lighting**: Dynamic day/night modes with proper illumination
3. **Texturing**: Effective use of textures for visual detail
4. **Animation**: Smooth animations of students, flag, and clouds
5. **Interaction**: Intuitive camera controls for exploration

The modular code architecture ensures maintainability and extensibility. The project meets all primary and secondary objectives, providing both educational value and practical demonstration of OpenGL programming.

### Key Achievements
- ✅ Fully functional 3D campus simulation
- ✅ Interactive first-person camera
- ✅ Realistic lighting and texturing
- ✅ Smooth animations
- ✅ Day/night mode toggle
- ✅ Clean, modular code
- ✅ Comprehensive documentation

---

## 11. Future Work

Potential enhancements for future versions:

1. **Advanced Graphics**
   - Implement shadow mapping for realistic shadows
   - Add fog effects for atmospheric depth
   - Use shaders (GLSL) for advanced lighting

2. **Additional Features**
   - Interior views of buildings
   - More detailed campus objects (cars, buses, fountains)
   - Weather effects (rain, snow)
   - Collision detection for realistic movement

3. **Optimization**
   - Implement frustum culling
   - Use display lists or VBOs for better performance
   - Level of detail (LOD) system

4. **Interactivity**
   - Mouse-based camera rotation
   - Clickable buildings for information
   - Mini-map for navigation
   - Multiple camera presets

5. **Content Expansion**
   - Add more DIU buildings
   - Include cafeteria, library, auditorium
   - Expand playground area
   - Add more animated characters

---

## 12. References

1. **OpenGL Programming Guide** (Red Book), 9th Edition, Addison-Wesley, 2017

2. **FreeGLUT Documentation**  
   http://freeglut.sourceforge.net/docs/api.php

3. **OpenGL Tutorial for Beginners**  
   https://www.opengl-tutorial.org/

4. **Computer Graphics: Principles and Practice**, 3rd Edition  
   John F. Hughes, et al., Addison-Wesley, 2013

5. **Real-Time Rendering**, 4th Edition  
   Tomas Akenine-Möller, et al., A K Peters/CRC Press, 2018

6. **OpenGL SuperBible**, 7th Edition  
   Graham Sellers, et al., Addison-Wesley, 2015

7. **Daffodil International University Official Website**  
   https://daffodilvarsity.edu.bd/

8. **OpenGL Reference Pages**  
   https://www.khronos.org/registry/OpenGL-Refpages/

---

## Appendix A: System Requirements

**Minimum Requirements**:
- OS: Windows 7 or later
- Processor: Intel Core i3 or equivalent
- RAM: 2 GB
- Graphics: OpenGL 2.1 compatible GPU
- Storage: 50 MB

**Recommended Requirements**:
- OS: Windows 10 or later
- Processor: Intel Core i5 or equivalent
- RAM: 4 GB
- Graphics: Dedicated GPU with OpenGL 3.0+
- Storage: 100 MB

---

## Appendix B: Installation Guide

See [README.md](../README.md) for detailed installation and compilation instructions.

---

## Appendix C: Code Statistics

- **Total Lines of Code**: ~1,800 lines
- **Source Files**: 8 files (.cpp)
- **Header Files**: 7 files (.h)
- **Functions**: 45+ functions
- **Classes**: 1 (Camera)

**Code Distribution**:
- main.cpp: 200 lines
- camera.cpp: 100 lines
- lighting.cpp: 60 lines
- textures.cpp: 120 lines
- buildings.cpp: 350 lines
- environment.cpp: 400 lines
- animations.cpp: 250 lines
- utils.cpp: 320 lines

---

**End of Report**

---

*Daffodil International University*  
*Computer Graphics Lab Project*  
*November 2025*
