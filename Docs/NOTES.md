# NOTES.md

## Purpose
**This file's purpose is to note concepts of graphics or various other topics that I've learned.**

---

## June 29th, 2026

### Z-Buffer

**What is a Z-buffer?** |
Z-buffer (A.K.A. Depth buffer) stores depth info of **fragments**.

**What is a fragment?** |
  A fragment is the data necessary to generate a single pixel's worth of a drawing (or primitive).

  Long story short, this buffer enables us to see which fragment is more close to the camera (user).

#### Things to think about

1. **Does z-buffer have to be the same as framebuffer?**
   → Yes. Same pixel grid as framebuffer, and same dimension as framebuffer.

2. **Then what about the non-fragment?**
   → Set every pixel's z-value as 1.0 (furthest), and then allocate z-value as its value is fed to the buffer.

## July 2nd, 2026
### Vertex Shading
Model(World) Transform → View Transform → Projection

**1. What is a Model(World) transform?** |
  Model transform is to move objects that exist in their own coordinate system onto world coordinate. Linear transformation, such as rotation, scaling, and translation is done by every vertex of the object.
  `M = T * R * S`

#### Things to think about
1. **Does order matter?**
   → Since M is a matrix, order matters. Rotation order matters too (yaw, pitch, roll) — and if Euler angle is used, then there is a risk of 'gimbal lock'.

**2. What is View transform?** |
  Camera is also an object, it exists in world space. Camera pose specification is defined as follows:
  **EYE**: Camera position.
  **AT**: Point or reference toward which the camera is aimed.
  **UP**: Vector that is orthogonal to the camera (Obviously, vector points upward)

  From this, camera space
  
$$
\begin{pmatrix} u \\ v \\ n \\ \mathbf{EYE} \end{pmatrix}
$$

  can be created.

$$
\begin{aligned}
n &= \frac{\mathbf{AT} - \mathbf{EYE}}{|\mathbf{AT} - \mathbf{EYE}|} \\
u &= \frac{\mathbf{UP} \times n}{|\mathbf{UP} \times n|} \\
v &= n \times u
\end{aligned}
$$

**3. What is Projection?**

-> MVP transform → Clipping → Perspective divide → Viewport / Screen Mapping

## July 17th, 2026

### UV coordinate & interpolation & etc.

1. **What is texel?** |  Texture element — a cell of the 2D texture array.
   Contains only its integer index (i, j) and Color. Does NOT own (s, t);
   texels are *referenced by* interpolated UVs, never the reverse (lookup
   is one-directional: mesh -> texture)
   
   NOTE: TEXEL IS NOT CORRESPONDS TO PIXEL ONE TO ONE (IMPORTANT FOR PIXEL SAMPLING)
3. **Flow of texture mapping** |
   
   (1) Modeling step: assign normalized (s, t) ∈ [0,1]² (parameter space)
       to each VERTEX. UV is a per-vertex attribute, independent of any
       specific texture/resolution.
   
   (2) Rasterization: interpolate (s, t) per fragment via barycentric
       weights — same mechanism as Gouraud color interp (l1, l2, l3 in
       RasterizeTriangle). Affine for now; perspective correction (1/w) later.
   
   (3) Sampling: scale to texel space — (s', t') = (s, t) · (width_tex, height_tex).
       s pairs with WIDTH, t with HEIGHT (order matters for non-square textures).
   
   (4) Filtering: discretize continuous (s', t') to texel index
       (nearest / bilinear).
       Texel center convention : (i + 0.5, j + 0.5)

   ## July 28th, 2026

   ### GenerateUVSphere() - Index Allocation
      **Goal**: Connect allocated vertices into triangles. Vertices are laid out as a 2D `(ring, seg)` grid but stored in a 1D `vbuf`, so I need a formula to map grid position. This formula is exactly same as accessing 1D array like a 2D array.
   
      **Strategy: quad-first.** The grid's natural cell is not a triangle. So let's start with quad. Pick the 4 corners of a quad, then split it along a diagonal into 2 triangles. This is easier than picking 3 vertices directly because coverage (no gaps, no overlaps) is automatic once you cut a rectangle in half.
      
      ```
                       seg     seg + 1
                        |         |
          ring     ---- A ------- B
                        |         |
          ring + 1 ---- C ------- D
      ```
      
      **Index formula** (row-major, since the generation loop fills ring 0 fully,
      then ring 1, ...):
      
      ```
      index(ring, seg) = ring * (slices + 1) + seg
      
      A = ring * (slices + 1) + seg      // top-left
      B = A + 1                          // top-right
      C = A + (slices + 1)               // bottom-left  (one row down)
      D = C + 1                          // bottom-right
      ```
      
      The `(slices + 1)` is the per-row vertex count. It's `slices + 1`, not `slices`, because of seam duplication (see below).
      
      **Split into triangles** along diagonal A-D:
      
      ```
      triangle 1: (A, C, D)
      triangle 2: (A, D, B)
      ```
      
      A-D is the shared diagonal. Keep the same winding rule for every cell so that the culling stays consistent.
      
      **Loop** (poles absorbed as rings — no cap special-casing):
      
      ```cpp
      for (int ring = 0; ring < stacks; ring++)
          for (int seg = 0; seg < slices; seg++)
              // compute A, B, C, D; push 2 triangles
      ```
      
      **Two independent design decisions** (don't conflate them):
      
      1. **Poles absorbed into the loop** This was what AI-made `GenerateUVSphere()` and hand-made `GenerateUVSpehre()` sets apart. (ring 0 = north, ring stacks = south, each a degenerate ring of `slices+1` coincident vertices).
         Purpose: makes UV possible at the poles AND removes the top-cap/bottom-cap special cases — the whole grid becomes one uniform quad loop. Nothing todo with the seam.
         Cost: pole-adjacent quads produce degenerate (Area2 == 0) triangles.
         These are harmless — `RasterizeTriangle` already guards `if (Area2 == 0)
         return;`.
      
      2. **Seam duplication** (`slices + 1` columns, i.e. `seg` goes 0..slices).
         Purpose: NOT to remove the seam — the seam is a topological necessity and
         cannot be removed. It removes the UV *wraparound artifact*. Without it,
         the last triangle connects seg(slices-1) → seg 0, so u runs 0.9375 → 0.0 (backwards), compressing the whole texture into one strip.
         With it, seg=slices is a copy of seg=0 (same position, u=1.0 instead of
         0.0), so u runs 0.9375 → 1.0 monotonically.
      
      **Counts** (stacks=12, slices=16 → verified):
      - vertices = (stacks+1)(slices+1) = 221
      - triangles = 2 * stacks * slices = 182  (degenerate included)
      - indices = 6 * stacks * slices = 1326
