# Design Document

## Overview

This design document outlines the technical approach for enhancing the existing OpenGL village scene with mountains, animated people, gate functionality, and dynamic wall colors. The enhancements will integrate seamlessly with the existing codebase structure, following established patterns for drawing composite objects, animation, and user interaction.

The implementation will leverage OpenGL immediate mode rendering with GLUT, using geometric primitives (triangles, quads, circles) to create visual elements. All new features will respect the existing day/night cycle system and maintain the modern flat design aesthetic.

## Architecture

The application follows a simple procedural architecture with clear separation of concerns:

### Rendering Pipeline

1. **Background Layer**: Sky, mountains (new), sun/moon
2. **Middle Layer**: Clouds, ground
3. **Object Layer**: House (with dynamic wall color), garden (with gate), tree, cow, people (new)
4. **UI Layer**: Control overlay

### Component Organization

- **Global State**: Variables for animation states, positions, and mode flags
- **Drawing Functions**: Modular functions for each scene element
- **Animation System**: Timer-based updates at 60 FPS (16ms intervals)
- **Input Handling**: Keyboard callback for user interactions

## Components and Interfaces

### 1. Mountain Rendering Component

**Function Signature:**

```cpp
void drawMountains();
```

**Responsibilities:**

- Render 2-3 mountain peaks using triangular shapes
- Apply day/night color variations
- Position mountains in background (drawn early in display order)

**Implementation Details:**

- Use `GL_TRIANGLES` for mountain peaks
- Layer multiple triangles for depth effect
- Add white triangular snow caps on peaks
- Color scheme:
  - Day: Gray-brown base (0.5f, 0.45f, 0.4f), white snow (1.0f, 1.0f, 1.0f)
  - Night: Dark gray base (0.2f, 0.18f, 0.16f), light gray snow (0.7f, 0.7f, 0.75f)

### 2. Person Rendering Component

**Function Signature:**

```cpp
void drawPerson(float x, float y, float armAngle, float legAngle);
```

**Parameters:**

- `x, y`: Base position coordinates
- `armAngle`: Rotation angle for arm animation (waving)
- `legAngle`: Offset for leg animation (walking)

**Responsibilities:**

- Render a person using geometric primitives
- Support animation through angle parameters
- Maintain consistent proportions and colors

**Implementation Details:**

- Head: Circle using `drawFilledCircle()` (radius ~15)
- Body: Rectangle (torso)
- Arms: Rectangles with rotation transforms
- Legs: Rectangles with position offsets
- Color scheme: Skin tone for head, clothing colors for body/limbs

### 3. Gate Animation Component

**Global Variables:**

```cpp
float gateAngle = 0.0f;  // Current gate rotation angle
```

**Function Modification:**

```cpp
void drawGarden(float x, float y);  // Modified to include gate
```

**Responsibilities:**

- Render gate as part of garden fence
- Apply rotation transform based on `gateAngle`
- Maintain hinge point at left edge of gate

**Implementation Details:**

- Gate is a section of the fence (first 40 pixels)
- Use `glPushMatrix()`, `glTranslatef()`, `glRotatef()` for rotation
- Hinge point: (x, y) position of garden
- Open angle: -90 degrees (swings inward)
- Closed angle: 0 degrees

### 4. Dynamic Wall Color Component

**Function Modification:**

```cpp
void drawHouse();  // Modified to use dynamic wall color
```

**Implementation Details:**

- Replace hardcoded wall color with conditional logic
- Day color: Warm cream (0.92f, 0.90f, 0.85f) - existing
- Night color: Cool blue-gray (0.65f, 0.68f, 0.75f)
- Apply color before drawing main house body rectangle

## Data Models

### Animation State Structure

```cpp
// Existing global variables (reference)
bool isDay;
float doorAngle;
float treeScale;
bool treeGrowing;

// New global variables
float gateAngle;           // Gate rotation: 0 (closed) to -90 (open)
float person1ArmAngle;     // Person 1 arm wave animation
float person2LegOffset;    // Person 2 walking animation
bool person1WavingUp;      // Animation direction flag
float person2X;            // Person 2 position for walking
bool person2MovingRight;   // Walking direction flag
```

### Color Palette Extension

```cpp
// Mountain colors
struct MountainColors {
    float baseR, baseG, baseB;    // Mountain body
    float snowR, snowG, snowB;    // Snow cap
};

// Wall colors
struct WallColors {
    float dayR, dayG, dayB;       // Day mode
    float nightR, nightG, nightB; // Night mode
};
```

## Correctness Properties

_A property is a characteristic or behavior that should hold true across all valid executions of a system-essentially, a formal statement about what the system should do. Properties serve as the bridge between human-readable specifications and machine-verifiable correctness guarantees._

### Testable Properties

Based on the prework analysis, most requirements relate to visual rendering and code organization, which are difficult to test programmatically in an OpenGL immediate mode application. However, we can identify a few testable properties for the interactive and animation logic:

Property 1: Animation state progression
_For any_ initial animation state, calling the timer function should update at least one person animation variable (armAngle or legOffset)
**Validates: Requirements 2.3**

Property 2: Gate opening keyboard response
_For any_ initial gate state, when the keyboard handler receives 'G' or 'g', the gateAngle should be set to -90 degrees
**Validates: Requirements 3.1**

Property 3: Gate closing keyboard response
_For any_ initial gate state, when the keyboard handler receives 'H' or 'h', the gateAngle should be set to 0 degrees
**Validates: Requirements 3.2**

Note: Requirements 4.1 and 4.2 (wall color changes) are implementation examples rather than properties, as they test specific color values for specific modes. The actual implementation will be verified through visual inspection and code review.

Most other requirements (1.1-1.5, 2.1-2.2, 2.4-2.5, 3.3-3.5, 4.3-4.5, 5.1-5.5) relate to visual appearance, rendering order, code structure, and design decisions that are best verified through code review and manual testing rather than automated property-based testing.

## Error Handling

### Input Validation

- Keyboard input is handled by GLUT's keyboard callback, which provides validated key codes
- No additional input validation is required for this feature set

### Rendering Robustness

- All drawing functions use existing helper functions (`drawRect`, `drawFilledCircle`)
- Matrix operations use proper push/pop pairs to prevent transform stack corruption
- Color values are clamped to valid OpenGL range [0.0, 1.0]

### Animation Bounds

- Animation variables (angles, positions) are bounded by conditional logic in timer function
- Gate angle is set to discrete values (0 or -90) rather than animated, preventing accumulation errors
- Person animation angles/offsets use similar bounded oscillation as existing cow and tree animations

## Testing Strategy

### Manual Testing Approach

Given the visual and interactive nature of OpenGL rendering, the primary testing approach will be manual verification:

1. **Visual Inspection**

   - Verify mountains appear in background with correct layering
   - Confirm two people are visible and distinguishable
   - Check gate rotation visual correctness
   - Validate wall color changes between day/night modes

2. **Interactive Testing**

   - Test all keyboard controls (D, N, O, C, G, H)
   - Verify gate opens and closes correctly
   - Confirm day/night toggle affects all elements appropriately

3. **Animation Testing**
   - Observe person animations over time
   - Verify animations are smooth and bounded
   - Check that all existing animations still work

### Unit Testing (Limited Scope)

While full rendering cannot be easily unit tested, we can test isolated logic:

1. **State Management Tests**

   - Test keyboard handler updates global variables correctly
   - Verify timer function updates animation variables within bounds

2. **Color Selection Tests**
   - Test that correct color values are selected based on isDay flag
   - Verify color values are within valid range [0.0, 1.0]

### Integration Testing

- Compile and run the complete application
- Verify no OpenGL errors occur during rendering
- Check that new features don't break existing functionality
- Confirm performance remains acceptable (60 FPS target)

### Testing Framework

For the limited unit tests, we will use:

- **Framework**: Standard C++ with manual test functions (no external framework needed for this simple case)
- **Approach**: Create a separate test file that includes the main source and tests individual functions
- **Validation**: Manual verification of test output

Note: Property-based testing is not practical for this OpenGL application as most behavior is visual and depends on the OpenGL rendering context. The three identified properties (animation progression, gate keyboard responses) can be tested with simple unit tests rather than full property-based testing frameworks.

## Implementation Notes

### Coordinate System

- Origin (0, 0) at bottom-left
- Window size: 1000x750 pixels
- Ground level: y = 200
- Sky region: y > 200

### Z-Order (Drawing Order)

1. Mountains (y = 200-500 range)
2. Sky/Sun/Moon
3. Clouds
4. Ground
5. House (y = 200-600)
6. Garden/Gate (y = 150-210)
7. Tree (y = 200-370)
8. Cow (y = 150-230)
9. People (y = 150-230)
10. UI Overlay

### Animation Timing

- Timer callback: 16ms (approximately 60 FPS)
- Person 1 arm wave: ±30 degrees, 2-second cycle
- Person 2 walk: 100 pixel range, 5-second cycle
- Gate: Instant state change (no animation)

### Keyboard Mapping Extension

- Existing: D/N (day/night), O/C (door open/close)
- New: G (gate open), H (gate close)
- Update UI text to include new controls

## Dependencies

- OpenGL (via GL/glut.h)
- GLUT library for windowing and input
- Standard C++ libraries (math.h, iostream, string.h)
- Existing codebase functions: `drawRect()`, `drawFilledCircle()`, `setSkyColor()`

## Performance Considerations

- All new drawing operations use immediate mode (consistent with existing code)
- Mountain rendering: ~6-9 triangles (minimal overhead)
- Person rendering: ~8-10 primitives per person (20 total)
- Gate rotation: Single transform operation
- Expected performance impact: < 5% (well within 60 FPS target)

## Future Enhancements

Potential improvements not included in this iteration:

- Smooth gate animation (interpolated rotation)
- More complex person animations (walking with leg movement)
- Additional mountain details (trees, rocks)
- Dynamic shadows based on sun/moon position
- Weather effects (rain, snow on mountains)
