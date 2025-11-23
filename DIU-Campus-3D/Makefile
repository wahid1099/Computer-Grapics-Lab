# Makefile for DIU Campus 3D OpenGL Project
# For Windows with MinGW

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
INCLUDES = -Isrc
LIBS = -lfreeglut -lopengl32 -lglu32 -lgdi32

# Source files
SOURCES = src/main.cpp \
          src/camera.cpp \
          src/lighting.cpp \
          src/textures.cpp \
          src/buildings.cpp \
          src/environment.cpp \
          src/animations.cpp \
          src/utils.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Output executable
TARGET = DIU_Campus.exe

# Default target
all: $(TARGET)
	@echo "Build complete! Run ./$(TARGET) to start the program."

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	del /Q src\*.o $(TARGET) 2>nul || echo "Clean complete"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean run rebuild
