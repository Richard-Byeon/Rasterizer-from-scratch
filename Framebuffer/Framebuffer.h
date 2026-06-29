#pragma once
/*
		FILE: Framebuffer.

		I wrote this separate to the main file. I wanted to leave a room for dynamic allocation of WIDTH and HEIGHT

*/

#include <vector>
#include "Math/Math.h"

#define WHITE	255
#define BLACK	0


// Rather than keeping the window size to the user, I'm just going to fix the window size and modify it later.


// Element of framebuffer
// Initial value is white, and Alpha value is 255 
struct Pixel
{
	uint8_t R = WHITE;
	uint8_t G = WHITE;
	uint8_t B = WHITE;
	uint8_t A = WHITE;
};