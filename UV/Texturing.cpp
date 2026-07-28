/*
	FILE: Texturing.cpp

*/

#include "Texturing.h"

Color GetTexel(const Texture& Tex, int i, int j)
{
	return Tex.Data[j * Tex.Width + i];	
}

Color Sample(const Texture& Tex, Vec2 UV)
{
	int i = std::min((int)(UV.v[0] * Tex.Width), Tex.Width - 1);
	int j = std::min((int)(UV.v[1] * Tex.Height), Tex.Height - 1);

	return GetTexel(Tex, i, j);
}

Texture GenerateCheckerboard(int Width, int Height, int cellSize)
{
	Texture Tex;
	Tex.Width = Width;
	Tex.Height = Height;

	Tex.Data.resize(Width * Height);
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			int cellX = i / cellSize;
			int cellY = j / cellSize;

			if ((cellX + cellY) % 2)
				Tex.Data[j * Width + i] = { 255, 0, 255, 255 };
			else
				Tex.Data[j * Width + i] = { 255, 255, 255, 255 };
		}
	}

	return Tex;
}
