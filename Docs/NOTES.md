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

July 2th, 2026

Vertex shading

Model(world) transform
View transfom
Proejection


1- What is a Model(world) transform?
Model transform is to move objects that exist in their own coordinate system onto world coordinate. Linear transformation, such as rotation, scaling, and translation is done by every vertices of object.  
`M = T * R * S`
Since M is a matrix, order matters. And rotation order matters too (yaw, pitch, roll) -> and if Euler angle is used, then there is a risk of 'gymbal lock'

2- View transform
Camera is also an object, it exists in world space. Camera pose specification is defined as follows: 
**EYE**: Camera position.
**AT**: Point or reference toward which the camera is aimed.
**UP**: Vector that is orthogonal to the camera (Obviously, vector points upward)
From this, camera space 

$$
\begin{pmatrix}
u \\
v \\
n \\
\mathbf{EYE}
\end{pmatrix}
$$

can be created. 

$$
\begin{aligned} 
n &= \frac{\mathbf{AT} - \mathbf{EYE}}{|\mathbf{AT} - \mathbf{EYE}|} \\ 
u &= \frac{\mathbf{UP} \times n}{|\mathbf{UP} \times n|} \\ 
v &= n \times u 
\end{aligned}
$$


-> MVP transform

Clipping
Perspective divide
Viewport / Screen Mapping
