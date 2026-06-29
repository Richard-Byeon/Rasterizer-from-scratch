# TODO.md file

## Purpose of this file
** This file's purpose is to record things to do each day.
** Contents could be...    Todo, Things I did today, and questions etc.

## June 29th, 2026

## 1. Z-buffering research (And interface of depth testing)

Logic || For each primitive, for each fragment P = {x, y, z}:
         1. Coverage test (edge function) — is P inside?
         2. Depth test — is P.z < ZBuffer[x][y]?
              (first primitive always passes, since buffer is cleared to 1.0)
         3. If pass: write P.z to ZBuffer AND write color to framebuffer.
         4. Repeat for all fragments, all primitives.
