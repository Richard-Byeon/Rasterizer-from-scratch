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
   
