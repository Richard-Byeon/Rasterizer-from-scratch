# NOTES.md

## Purpose
**This file's purpose is to note concepts of graphics or various other topics that I've learned.**

---

## June 29th, 2026

### Z-Buffer

**What is a Z-buffer?** |
Z-buffer (A.K.A. Depth buffer) stores depth info of **fragments**.

**What is a fragment?** |
  A fragment is data necessary to generate a single pixel's worth of a drawing (or primitive).

  Long story short, this buffer enables us to see which fragment is more close to the camera (user).

#### Things to think about

1. **Does z-buffer have to be the same as framebuffer?**
   → Yes. Same pixel grid as framebuffer, and same dimension as framebuffer.

2. **Then what about the non-fragment?**
   → Set every pixel's z-value as 1.0 (furthest), and then allocate z-value as its value is fed to the buffer.
