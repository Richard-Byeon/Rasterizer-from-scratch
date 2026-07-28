#include "Texturing.h"

Color GetTexel(const Texture& Texture, int i, int j)
{
	return Texture.Data[(j * Texture.Width + i)];
}