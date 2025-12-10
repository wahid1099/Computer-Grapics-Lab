# Animated Farmhouse Village Scene

## Overview

This is an animated farmhouse village scene using OpenGL/GLUT. It features day/night cycles, interactive elements, and multiple animated characters and objects.

## Global Variables

| Variable             | Type           | Purpose                                 |
| :------------------- | :------------- | :-------------------------------------- |
| `isDay`              | `bool`         | Toggles between day and night mode      |
| `cloudX`             | `float`        | Cloud horizontal position for scrolling |
| `cowX`               | `float`        | Cow horizontal position                 |
| `cowHeadAngle`       | `float`        | Cow head grazing animation angle        |
| `grazingDown`        | `bool`         | Direction of cow head grazing           |
| `doorAngle`          | `float`        | House door rotation angle               |
| `treeScale`          | `float`        | Tree size for breathing animation       |
| `treeGrowing`        | `bool`         | Tree scale direction                    |
| `person1ArmAngle`    | `float`        | First person's arm waving angle         |
| `person1WavingUp`    | `bool`         | Direction of arm wave                   |
| `person2X`           | `float`        | Second person position                  |
| `person2LegAngle`    | `float`        | Second person walking legs              |
| `person2MovingRight` | `bool`         | Walking direction                       |
| `windmillAngle`      | `float`        | Windmill blade rotation                 |
| `birdFlap`           | `float`        | Flying bird wing animation              |
| `butterflyX`         | `float`        | Butterfly X position                    |
| `butterflyY`         | `float`        | Butterfly Y position                    |
| `butterflyWing`      | `float`        | Butterfly wing flap phase               |
| `sheep1X`            | `float`        | Sheep 1 position                        |
| `sheep2X`            | `float`        | Sheep 2 position                        |
| `sheep1MovingRight`  | `bool`         | Sheep 1 movement direction              |
| `smokeY`             | `vector`       | Chimney smoke particle positions        |
| `stars`              | `vector<Star>` | Night sky stars with twinkling phase    |

## Core Drawing Functions

### Primitive Shapes

- **`drawFilledCircle(int xc, int yc, int r)`**
  - **Algorithm**: Midpoint Circle Algorithm
  - **Purpose**: Draws filled circles using horizontal line segments
  - **Parameters**: Center (`xc`, `yc`), radius `r`
  - **How it works**: Uses decision parameter (`d`) to determine when to decrement `y` while incrementing `x`.
- **`drawRect(float x1, float y1, float x2, float y2)`**
  - **Purpose**: Draws filled rectangle as a quadrilateral
  - **Parameters**: Bottom-left (`x1`, `y1`), top-right (`x2`, `y2`)

### Scene Objects

#### Sky & Atmosphere

- **`drawSunMoon()`**
  - **Day mode**: Yellow filled circle at (900, 650) with radius 50
  - **Night mode**: Light gray circle with crescent shadow effect, array of twinkling stars with sine wave brightness.
- **`drawCloudCluster(float x, float y)`**
  - **Composition**: Multiple overlapping circles creating cloud shape
  - **Color**: White (day) or gray (night)
  - **Animation**: Scrolls continuously via `cloudX` variable
- **`drawMountains()`**
  - **Components**: Triangular mountains (brown/green base), white triangles on tops for snow peaks.
  - **Day/Night variation**: Color changes based on `isDay` flag.

#### Main House

- **`drawHouse()`**
  - **Structure**: Light beige walls (300-700, 200-450), red/brown sloped roof with peak, brown door with yellow knob, light blue windows (2 on sides), small chimney (600, 480-580).
  - **Door Animation**: Rotates using `doorAngle` (0° closed, -85° open).
- **`drawChimneySmoke()`**
  - **Purpose**: Particles rising from chimney.
  - **Implementation**: Gray circles that start at y=500 and move upward.
  - **Spawn**: Random new particles added to `smokeY` vector.

#### Farm Buildings

- **`drawBarn()`**
  - **Color**: Bright red walls.
  - **Structure**: Rectangle with triangular roof and large doors.
  - **Windows**: Two white rectangles for light.
- **`drawChickenCoop()`**
  - **Features**: Brown wooden structure (50, 220 to 150, 270), red peaked roof, dark rectangle entrance, yellow chickens with red combs.
- **`drawWell()`**
  - **Design**: Circular structure with dark center, vertical posts on sides, wooden top beam.

#### Farm Elements

- **`drawSimpleFence()`**
  - **Design**: Vertical posts every 40 units with horizontal rails.
  - **Materials**: Brown wood color.
  - **Spans**: From x=250 to x=750.
- **`drawVegetableField(float x, float y)`**
  - **Layout**: 5 rows × 6 columns of vegetables.
  - **Crops**: Carrots (orange circles with green tops for even rows), Cabbages (green circles for odd rows).
  - **Soil**: Brown ground with visible rows.
- **`drawGarden(float x, float y)`**
  - **Design**: White trellis/fence structure.
  - **Flowers**: Alternating colored flower heads (pink/yellow).

### Animated Characters

#### People

- **`drawPerson(float x, float y, float armAngle, float legAngle)`**
  - **Body Parts**: Blue rectangular legs (walking animation rotated by `legAngle`), red rectangular torso, red arms (rotating waving animation), skin-colored circular head with eyes and nose, small circular hand.
- **`drawButterfly()`**
  - **Structure**: Black body with pink wings.
  - **Animation**: Wings flap using sine wave, drifts left/right and up/down, resets position when off-screen.
- **`drawSimpleBird(float x, float y, float flapAngle)`**
  - **Structure**: Small dark circular body with line wings.
  - **Animation**: Wings move up/down based on `flapAngle`.

#### Animals

- **`drawCow(float x, float y)`**
  - **Body**: White rectangle (main body).
  - **Head**: Rotates with `cowHeadAngle` for grazing animation.
  - **Spots**: Black filled circles on body.
  - **Legs**: Four white vertical rectangles.
  - **Details**: Pink nose/udder area.
- **`drawSheep(float x, float y)`**
  - **Body**: Large white fluffy circle.
  - **Head**: Gray circle with small ears.
  - **Legs**: Black lines extending down.
  - **Details**: Black eye dot.

### Other Objects

- **`drawWindmill()`**
  - **Structure**: Light gray vertical rectangular pole (height 160), four white rectangular blades rotating around pivot.
  - **Animation**: Rotates continuously via `windmillAngle`.
- **`drawTree()`**
  - **Trunk**: Brown vertical rectangle (10 units wide, 120 tall).
  - **Canopy**: Large green circle at top.
  - **Animation**: "Breathing" effect via `treeScale` variable.

## User Interface

- **`drawUI()`**
  - **Background**: Semi-transparent dark overlay at top.
  - **Text Display**: Controls (Day/Night/Door) and Status (Current time of day).
  - **Rendering**: Uses `glRasterPos2f` and `glutBitmapCharacter`.

## Animation Loop

The `timer(int)` function is called every 16ms to update animations:

### Animation Update Methods

- **Clouds**: `cloudX += 0.7f`, wraps at 1100.
- **Windmill**: `windmillAngle += 3.0f`.
- **Flying Birds**: `birdFlap += 0.15f`.
- **Butterfly**: Sine wave motion, wraps off-screen.
- **Smoke Particles**: Rise and disappear at y=750.
- **Cow Head**: Oscillates between -15° and 0° (grazing).
- **Person 1 Arm**: Waves between -10° and 45°.
- **Person 2 Walking**: Sine wave leg angle with position change.
- **Sheep**: Move back/forth and slight sway.
- **Tree**: Scales between 0.95 and 1.15.
- **Stars**: Twinkling via phase sine wave.

### Keyboard Controls

| Key | Action                        |
| :-- | :---------------------------- |
| `D` | Switch to Day mode            |
| `N` | Switch to Night mode          |
| `O` | Open house door (rotate -85°) |
| `C` | Close house door (rotate 0°)  |

## Color Scheme

### Day Colors

| Element   | RGB                | Description |
| :-------- | :----------------- | :---------- |
| Sky       | (0.53, 0.81, 0.98) | Light blue  |
| Sun       | (1.0, 0.95, 0.0)   | Yellow      |
| Grass     | (0.4, 0.8, 0.3)    | Green       |
| Mountains | (0.5, 0.45, 0.4)   | Brown       |
| Clouds    | (1.0, 1.0, 1.0)    | White       |

### Night Colors

| Element   | RGB                | Description |
| :-------- | :----------------- | :---------- |
| Sky       | (0.05, 0.08, 0.25) | Dark blue   |
| Moon      | (0.9, 0.9, 0.95)   | Off-white   |
| Grass     | (0.1, 0.3, 0.1)    | Dark green  |
| Mountains | (0.2, 0.18, 0.16)  | Dark brown  |
| Clouds    | (0.6, 0.6, 0.7)    | Gray        |

## Main Function Flow

```
main()
├─ glutInit()                     - Initialize GLUT
├─ glutInitDisplayMode()          - DOUBLE buffering + RGB + Alpha
├─ glutInitWindowSize(1000, 750)  - Set window dimensions
├─ gluOrtho2D(0, 1000, 0, 750)    - 2D orthogonal projection
├─ Initialize 100 random stars for night sky
├─ glutDisplayFunc(display)       - Set render function
├─ glutKeyboardFunc(keyboard)     - Set input handler
├─ glutTimerFunc(0, timer, 0)     - Start animation loop
└─ glutMainLoop()                 - Event loop
```

## Display Pipeline

1.  Clear screen with sky color (changes day/night).
2.  Draw background: Gradient ground.
3.  Draw static objects: Mountains, fence, houses.
4.  Draw sky: Sun/Moon, clouds, stars.
5.  Draw farm objects: Garden, vegetable field, well, barn, coop.
6.  Draw animals: Cow, sheep, birds.
7.  Draw people: Person 1 (waving), Person 2 (walking).
8.  Draw effects: Chimney smoke, butterfly.
9.  Draw UI: Control text overlay.
10. Swap buffers: Display rendered frame.

## Technical Details

- **Graphics API**: OpenGL with GLUT
- **Rendering**: 2D orthographic projection
- **Buffer Mode**: Double buffering (smooth animation)
- **Frame Rate**: ~60 FPS (16ms timer)
- **Resolution**: 1000×750 pixels
- **Coordinate System**: Bottom-left origin, x: 0-1000, y: 0-750
