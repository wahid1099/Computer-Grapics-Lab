# Implementation Plan

- [x] 1. Add global variables for new features

  - Add `gateAngle` variable for gate rotation state
  - Add `person1ArmAngle` and `person1WavingUp` for person 1 arm animation
  - Add `person2X`, `person2LegOffset`, and `person2MovingRight` for person 2 walking animation
  - Initialize all variables with appropriate starting values
  - _Requirements: 2.3, 3.1, 3.2, 5.5_

- [x] 2. Implement mountain rendering function

  - Create `drawMountains()` function
  - Draw 2-3 mountain peaks using GL_TRIANGLES
  - Add white snow caps on mountain peaks
  - Implement day/night color variation for mountains
  - Position mountains in background coordinate range (y = 200-500)
  - _Requirements: 1.1, 1.2, 1.4, 1.5_

-

- [x] 3. Implement person rendering function

  - Create `drawPerson(float x, float y, float armAngle, float legAngle)` function
  - Draw head using `drawFilledCircle()` with radius ~15
  - Draw body (torso) using `drawRect()`
  - Draw arms as rectangles with rotation transform for `armAngle`
  - Draw legs as rectangles with position offset for `legAngle`
  - Use appropriate colors (skin tone for head, clothing colors for body)
  - _Requirements: 2.2, 2.4_

- [x] 4. Add person animations to timer function

  - Implement person 1 arm waving animation (±30 degrees oscillation)
  - Implement person 2 walking animation (horizontal movement with leg offset)
  - Update `person1ArmAngle` with bounded oscillation
  - Update `person2X` position and `person2LegOffset` for walking effect
  - Use similar pattern to existing cow head and tree scale animations
  - _Requirements: 2.3_

- [ ] 5. Modify garden function to include animated gate

  - Update `drawGarden()` function signature (keep existing parameters)
  - Separate first section of fence as the gate (first 40 pixels)
  - Apply rotation transform to gate section using `gateAngle`
  - Set hinge point at left edge of gate using glTranslatef/glRotatef
  - Ensure rest of fence renders normally
  - _Requirements: 3.1, 3.2, 3.3, 3.4_

- [ ] 6. Implement dynamic wall color in house function

  - Modify `drawHouse()` function
  - Add conditional logic to select wall color based on `isDay` flag
  - Use warm cream color (0.92f, 0.90f, 0.85f) for day mode
  - Use cool blue-gray color (0.65f, 0.68f, 0.75f) for night mode
  - Apply color before drawing main house body rectangle
  - Ensure windows, doors, and roof colors remain unchanged
  - _Requirements: 4.1, 4.2, 4.5_

- [ ] 7. Add keyboard controls for gate

  - Extend `keyboard()` function with new cases
  - Add case 'g'/'G' to set `gateAngle = -90.0f` (open)
  - Add case 'h'/'H' to set `gateAngle = 0.0f` (close)
  - Call `glutPostRedisplay()` after state change
  - _Requirements: 3.1, 3.2, 3.5_

- [ ] 8. Update display function to render new elements

  - Add `drawMountains()` call in background layer (before sun/moon)
  - Add two `drawPerson()` calls with different positions and animation parameters
  - Position person 1 near house (e.g., x=250, y=150) with arm animation
  - Position person 2 with dynamic x position for walking animation
  - Ensure rendering order maintains proper z-layering
  - _Requirements: 1.3, 2.1, 5.4_

- [ ] 9. Update UI overlay with new controls

  - Modify `drawUI()` function
  - Update controls text to include "[G] Open Gate [H] Close Gate"
  - Ensure text fits within the control bar
  - Maintain existing formatting and alignment
  - _Requirements: 3.1, 3.2_

- [ ] 10. Manual testing and verification
  - Compile and run the application
  - Test all keyboard controls (D, N, O, C, G, H)
  - Verify mountains appear correctly in day and night modes
  - Verify two people are visible and animated
  - Verify gate opens and closes correctly
  - Verify wall color changes between day and night
  - Check that all existing features still work (cow, tree, clouds, door)
  - Verify performance is acceptable (smooth 60 FPS)
  - _Requirements: All_
