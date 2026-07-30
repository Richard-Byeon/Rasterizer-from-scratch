/*
	FILE: Texturing.cpp

*/

#include "Texturing.h"
#include "ThirdParty/stb_image.h"
#include <iostream>
#include <cstring>

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
				Tex.Data[j * Width + i] = { 60,  60,  60, 255 };
		}
	}

	return Tex;
}

Texture LoadTexture(const char* path)
{
	int w, h, srcChannels;
	stbi_uc* pixels = stbi_load(path, &w, &h, &srcChannels, 4);
	if (!pixels)
	{
		std::cerr << "LoadTexture failed: " << path
			<< " (" << stbi_failure_reason() << ")\n";
		return Texture{};
	}

	Texture Tex;

	Tex.Width = w;
	Tex.Height = h;
	Tex.Data.resize((size_t)w * h);
	memcpy(Tex.Data.data(), pixels, (size_t)w * h * 4);
	stbi_image_free(pixels);

	return Tex;
}