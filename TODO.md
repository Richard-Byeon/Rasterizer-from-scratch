# TODO.md

## Purpose of this file
- This file's purpose is to record things to do each day.
- Contents could be: Todo, Things I did today, and questions, etc.

---

## June 29th, 2026

### 1. Z-buffering research (and interface of depth testing)

**Logic** — For each primitive, for each fragment `P = {x, y, z}`:

1. **Coverage test** (edge function) — is `P` inside?
2. **Depth test** — is `P.z < ZBuffer[x][y]`?
   *(first primitive always passes, since buffer is cleared to 1.0)*
3. **If pass:** write `P.z` to ZBuffer **and** write color to framebuffer.
4. Repeat for all fragments, all primitives.

---

## July 1st, 2026

### 1. OBJ file format study

1. **`v` / `vt` / `vn` / `f` line structure.**
2. **`f` line indexing: position/UV/normal indices are independent per attribute, not shared.**
3. **Consequence: a single OBJ position can map to multiple distinct (v, vt, vn) combinations (UV seams, hard-edge normals), so unique-vertex count ≠ raw `v` line count.**

### 2. IA (Input Assembler) implementation
1. **Hardcode vertecies first to test buffers are working**
2. **Check if IA works `VertexBuffer`, `IndexBuffer`**

---

## July 14th, 2026

### Image Texturing (UV interpolate and such)

1. **Texture Coordinate(s,t) & Texel implement**
2. **Research what parameterization is and figure out what kind of method is used**
