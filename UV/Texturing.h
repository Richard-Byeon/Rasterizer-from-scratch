#pragma once

/*
	FILE: TEXTURING.h

*/

#include "Vertex/Vertex.h"

struct Texture
{
	int Width	= 0;
	int Height	= 0;
	std::vector<Color> Data;
};

// Where is this function used? << at the rasterizer.

Color GetTexel(int i, int j);

