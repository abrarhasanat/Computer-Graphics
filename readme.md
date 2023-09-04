# offline - 2 magic_cube and clock

.

## Analog Clock

- Draws an analog clock with hour, minute and second hands  
- Implements a swinging pendulum with 2 second period using sinusoidal motion
- Allows setting current time using keyboard input
- Uses colors and gradients to enhance visual appeal

## Magic Cube

- Models a cube that transitions between sphere and octahedron
- Uses a single triangle, cylinder segment and sphere segment with transformations  
- Ensures smooth transitions between different surfaces
- Follows geometry of inscribing sphere into octahedron
- Implements object rotation and shifting between shapes

## Camera Movement 

- Orbiting camera to view cube from different angles
- Uses keyboard input for camera translation and rotation
- Supports moving forward, backward, left, right, up, down
- Allows looking left, right, up, down and tilt
- Bonus: Vertical movement without changing look direction




# offline-2 Resterizatoin Pipeline

This project implements the first 3 stages of a raster-based graphics pipeline in C++.

## Description 

The pipeline takes in a scene description from `scene.txt` and generates the output of the first 3 stages in `stage1.txt`, `stage2.txt`, and `stage3.txt`.

The input `scene.txt` contains:
- Camera parameters
- Projection parameters
- Modeling commands to draw triangles and apply transformations

The 3 stages implemented are:

1. Modeling transformation
2. View transformation 
3. Projection transformation

## Details

- Uses a transformation matrix stack to apply hierarchical modeling transformations
- Generates view transformation matrix based on camera parameters 
- Generates projection matrix based on field of view and aspect ratio
- Implements push/pop, translate, rotate, and scale modeling commands
- Outputs transformed vertices in homogeneous coordinates



# Ray Tracing Implementation

For this project, I implemented a basic ray tracer in C++ that supports ray-object intersections, recursive reflections, lighting, and textures.

## Implementation 

I started by creating a OpenGL 3D scene with pyramids, spheres, cubes, and an infinite checkerboard. I implemented the same orbital camera controls from Assignment 1 using l, r, u vectors. 

When the '0' key is pressed, the program switches to ray tracing mode. It shoots rays from the camera through each pixel on the screen.

To find intersections, I calculated analytic solutions for rays intersecting planes, spheres, and cubes. This was based on the mathematical formulas covered in class.

Once an intersection point was found, I calculated the shading using a combination of ambient, diffuse, specular, and reflective components. 

For the lighting calculations, I checked visibility from light sources and computed per-light Lambert term and Phong term. The light falloff with distance was also incorporated.

To implement reflections, I recursively called the ray tracing function with the reflected ray and decreasing depth. The ray color was accumulated at each bounce.

I also added texture mapping to the checkerboard by indexing into a texture image based on the intersection point. This handled translating texture coords to world space.

The final pixel color was determined by combining the ambient, diffuse, specular, and reflective terms. This gave realistic looking reflections and shading.

## Results

The program generated images that matched the scene from different camera positions. The recursive ray tracing produced reflections, refractions, shadows, shading, and texture mapping.

The different components including intersections, lighting, and reflections were implemented successfully as required.

## References

The ray tracing techniques were based on the theory covered in the course lectures and reference slides.