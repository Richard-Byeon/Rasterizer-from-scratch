#pragma once

/*
	FILE: Texturing.h

	LAST MODIFIDED: Added Sample(), GetTexel(), GenerateCheckerboard()
*/

#include "Vertex/Vertex.h"

struct Texture
{
	int Width	= 0;
	int Height	= 0;
	std::vector<Color> Data;
};

Color Sample(const Texture& Tex, Vec2 UV, int i, int j);

Color GetTexel(const Texture& Tex, int i, int j);

Texture GenerateCheckerboard(int Width, int Height, int cellSize);


