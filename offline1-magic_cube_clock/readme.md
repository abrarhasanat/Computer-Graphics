



# Assignment 2: Graphics Pipeline Adventure

# Graphics Pipeline Implementation

For this project, I implemented the first 4 stages of a raster-based graphics pipeline in C++. This implements modeling, view, and projection transformations along with a Z-buffer visible surface determination algorithm.

## Implementation

I started by parsing the scene description from `scene.txt` which contained:

- Camera position and orientation
- Projection parameters like field of view 
- Modeling commands to draw triangles

### Stage 1: Modeling 

- Used a transformation matrix stack to hierarchically apply translations, rotations, and scaling
- Generated the modeling transformation matrix by multiplying the stacks  
- Transformed vertices by the modeling matrix
- Wrote the transformed vertices to `stage1.txt`

### Stage 2: View

- Computed view matrix using camera look-at parameters
- Aligned camera Z axis along looking direction
- Transformed vertices by view matrix
- Wrote transformed vertices to `stage2.txt`

### Stage 3: Projection

- Generated projection matrix from field of view and aspect ratio
- Transformed vertices by projection matrix 
- Wrote transformed vertices to `stage3.txt` 

### Stage 4: Z-buffering

- Initialized image and z-buffer from `config.txt`
- Clipped triangles against viewing volume 
- Rendered triangles to image buffer using z-buffer for hidden surface removal
- Saved final image to `out.bmp`
- Saved z-buffer to `zbuffer.txt`

## Outcome

The pipeline successfully rendered a 3D scene with visible surface determination. The modeling, view and projection transforms were implemented along with clipping and scan-conversion using a z-buffer algorithm.

The rendered `out.bmp` image and `zbuffer.txt` matched the expected results.

## References

The graphics pipeline was implemented based on the overview provided in [Assignment.pdf].

[Assignment.pdf]: <assignment.pdf>
[Assignment PDF Part 1]: <./Resterization Part 1/Offline 2 Rasterization (Part 1)>