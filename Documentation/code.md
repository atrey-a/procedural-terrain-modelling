# 3D Terrain Generation Documentation

This documentation provides an overview of the C++ code for generating a 3D terrain using OpenGL and Perlin noise. The code includes definitions for different terrains such as general land, pond, mountain, and river. It also implements camera controls for panning, sliding, zooming, yawing, and resetting the view.

## Definitions

### General Land Definition
- `width`: Width of the terrain.
- `height`: Height of the terrain.
- Parameters for general land noise generation:
  - `defaultNoiseLimit`: Default noise limit.
  - `defaultOctaves`: Number of octaves.
  - `defaultPersistence`: Persistence value.
  - `defaultLacunarity`: Lacunarity value.
  - `defaultInterpolation`: Interpolation method.

### Pond Definition
- Parameters for pond noise generation and characteristics:
  - `pond_center_x`, `pond_center_y`: Center coordinates of the pond.
  - `pond_radius`: Radius of the pond.
  - Similar noise parameters as general land.

### Mountain Definition
- Parameters for mountain noise generation and characteristics:
  - `mountain_center_x`, `mountain_center_y`: Center coordinates of the mountain.
  - `mountain_radius`: Radius of the mountain.
  - Additional persistence values (`mountainPersistence1`, `mountainPersistence2`, `mountainPersistence3`).
  - Similar noise parameters as general land.

### River Definition
- Parameters for river noise generation and characteristics:
  - `river_x1`, `river_x2`, `river_y1`, `river_y2`: Coordinates defining the river path.
  - Similar noise parameters as general land.

## Camera Controls

### Functions
1. `slide_u(float delU)`: Slide/pan in 'u' direction.
2. `slide_v(float delV)`: Slide/pan in 'v' direction.
3. `zoom(float delN)`: Zoom in 'n' direction.
4. `reset()`: Reset to the initial view.
5. `yaw(float angle)`: Yaw the camera about the 'v' vector.

### Variables
- Camera position (`eye_x`, `eye_y`, `eye_z`).
- Look-at position (`look_x`, `look_y`, `look_z`).
- Up vector (`up_x`, `up_y`, `up_z`).

## Perlin Noise Functions

1. `noise(int x, int y, int z, int noiseLimit)`: Generate 3D Perlin noise.
2. `interpolate(double a, double b, double t, char f)`: Interpolate between two values using different methods.
3. `perlinCalculate(int x, int y, int z, int noiseLimit, char interpolationFunc)`: Generate 3D Perlin noise using interpolation.
4. `perlin(int x, int y, int z, int noiseLimit, int octaves, double persistence, double lacunarity, char interpolationFunc)`: Generate Perlin noise with octaves, persistence, and lacunarity.

## Terrain Rendering

1. `setColor(double perlinValue)`: Determine color based on height.
2. Terrain features are defined using functions like `pond()`, `mountain()`, `innerMountain1()`, `innerMountain2()`, `innerMountain3()`, and `river()`.

## OpenGL Initialization and Display

1. `init()`: Initialize OpenGL settings.
2. `renderTerrain(int width, int height)`: Render the 3D terrain using Perlin noise.
3. Keyboard and special keyboard input functions (`keyInput`, `SpecialInput`) handle camera movements and actions.

## Main Function

1. Initialize OpenGL, set display mode, window size, and title.
2. Set up initial parameters and callbacks.
3. Enter the GLUT main loop for rendering and interaction.

**Note**: Some features like the river are mentioned in comments as future work and are currently not implemented.
