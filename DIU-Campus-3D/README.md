# 3D DIU Mini Campus - OpenGL Project

An interactive 3D simulation of Daffodil International University (DIU) campus built with C++ and OpenGL.

![DIU Campus 3D](textures/diu_logo.png)

## 🎯 Features

### Campus Structures
- **DIU Main Gate** - Green and white themed entrance with DIU logo
- **Academic Buildings**
  - AB-1: Rectangular building with grid windows
  - AB-2: Tall building with multiple floors
  - AB-3: Wide building with entrance stairs
- **Name Board** - "Daffodil International University" sign

### Environment
- Realistic grass ground with texture
- Skybox with day/night modes
- Football playground with:
  - Boundary lines
  - Goal posts
  - Waving flag
- Trees scattered around campus
- Benches for seating
- Street lights (illuminate at night)
- Pathways connecting buildings

### Animations
- **Walking Students** - 3 students walking around the playground with realistic limb movement
- **Waving Flag** - Sine-wave animation for realistic flag motion
- **Moving Clouds** - Clouds drifting across the sky

### Interactive Features
- **First-Person Camera** - Explore the entire campus freely
- **Day/Night Mode** - Toggle between bright day and atmospheric night
- **Pause Animations** - Freeze all moving objects
- **Camera Reset** - Return to initial viewing position

## 🎮 Controls

| Key | Action |
|-----|--------|
| **W** | Move camera forward |
| **S** | Move camera backward |
| **A** | Move camera left |
| **D** | Move camera right |
| **Q** | Move camera up |
| **E** | Move camera down |
| **↑** | Look up |
| **↓** | Look down |
| **←** | Look left |
| **→** | Look right |
| **N** | Toggle day/night mode |
| **P** | Pause/resume animations |
| **R** | Reset camera position |
| **ESC** | Exit program |

## 📋 Prerequisites

### Required Software
- **C++ Compiler**: MinGW (Windows), GCC (Linux), or Xcode (Mac)
- **FreeGLUT**: OpenGL Utility Toolkit
- **OpenGL**: Graphics library (usually pre-installed)
- **GLU**: OpenGL Utility Library

### Windows Installation (MinGW)

1. **Install MinGW**
   - Download from: https://sourceforge.net/projects/mingw/
   - Add MinGW\bin to system PATH

2. **Install FreeGLUT**
   - Download from: https://www.transmissionzero.co.uk/software/freeglut-devel/
   - Extract and copy:
     - `freeglut.dll` to `C:\Windows\System32`
     - `include\GL\*` to `MinGW\include\GL\`
     - `lib\*` to `MinGW\lib\`

## 🔨 Compilation

### Using Makefile (Recommended)

```bash
# Navigate to project directory
cd DIU-Campus-3D

# Compile the project
mingw32-make

# Run the program
./DIU_Campus.exe
```

### Manual Compilation

```bash
g++ -o DIU_Campus.exe src/*.cpp -lfreeglut -lopengl32 -lglu32 -lgdi32 -std=c++11
```

### Clean Build Files

```bash
mingw32-make clean
```

## 📁 Project Structure

```
DIU-Campus-3D/
├── src/
│   ├── main.cpp           # Main program entry point
│   ├── camera.h/cpp       # Camera system
│   ├── lighting.h/cpp     # Lighting and day/night modes
│   ├── textures.h/cpp     # Texture loading
│   ├── buildings.h/cpp    # Campus buildings
│   ├── environment.h/cpp  # Environment objects
│   ├── animations.h/cpp   # Animated objects
│   └── utils.h/cpp        # Helper functions
├── textures/
│   ├── diu_logo.png       # DIU logo
│   ├── grass.png          # Grass texture
│   ├── sky.png            # Sky texture
│   ├── wall.png           # Building wall texture
│   └── window.png         # Window texture
├── docs/
│   ├── project_report.pdf # Project documentation
│   └── presentation.pptx  # Presentation slides
├── Makefile               # Build configuration
└── README.md              # This file
```

## 🎨 Technical Details

### Graphics Techniques Used
- **3D Transformations**: Translation, rotation, scaling using OpenGL matrix stack
- **Lighting**: Ambient, diffuse, and specular lighting with GL_LIGHT0
- **Texturing**: 2D texture mapping on buildings and ground
- **Materials**: Material properties for realistic surface appearance
- **Depth Testing**: Proper occlusion handling
- **Backface Culling**: Performance optimization
- **Double Buffering**: Smooth rendering without flicker

### Code Architecture
- **Modular Design**: Separated concerns (camera, lighting, buildings, etc.)
- **Object-Oriented**: Camera class for encapsulation
- **Event-Driven**: Keyboard and timer callbacks
- **Procedural Generation**: Fallback textures if image files not found

## 🚀 Running the Program

1. Compile the project using the Makefile
2. Run `DIU_Campus.exe`
3. Use keyboard controls to explore the campus
4. Press **N** to see the night mode
5. Press **P** to pause and observe the animations
6. Press **ESC** to exit

## 📸 Screenshots

The program displays:
- Main gate with DIU logo at the entrance
- Three academic buildings (AB-1, AB-2, AB-3) with windows
- A football playground with students walking around
- Trees, benches, and street lights
- A waving flag
- Moving clouds in the sky

## 🐛 Troubleshooting

### "freeglut.dll not found"
- Copy `freeglut.dll` to the same directory as the executable
- Or install FreeGLUT system-wide

### "Cannot open include file 'GL/glut.h'"
- Ensure FreeGLUT headers are in your compiler's include path
- Check MinGW\include\GL\ directory

### Black screen or no textures
- The program uses procedural textures as fallback
- Textures are generated programmatically if PNG files aren't loaded

### Low frame rate
- Reduce window size in main.cpp (WINDOW_WIDTH, WINDOW_HEIGHT)
- Disable some animations or reduce object count

## 📝 Assignment Submission

This project includes:
1. ✅ Complete source code (all .cpp and .h files)
2. ✅ Makefile for easy compilation
3. ✅ README with instructions
4. ✅ Texture assets
5. ✅ Project report (PDF) - in docs/ folder
6. ✅ Presentation slides (PPTX) - in docs/ folder

## 👨‍💻 Development

**Project Type**: Computer Graphics Lab Project  
**Technology**: C++, OpenGL, FreeGLUT  
**Platform**: Windows (adaptable to Linux/Mac)  
**Graphics API**: OpenGL 2.1 (Legacy/Fixed Pipeline)

## 📚 References

- OpenGL Programming Guide (Red Book)
- FreeGLUT Documentation: http://freeglut.sourceforge.net/
- OpenGL Tutorial: https://www.opengl-tutorial.org/
- DIU Official Website: https://daffodilvarsity.edu.bd/

## 📄 License

This project is created for educational purposes as part of a Computer Graphics course assignment.

---

**Daffodil International University**  
*Computer Graphics Lab Project*
