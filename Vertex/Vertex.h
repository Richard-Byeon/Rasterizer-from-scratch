#pragma once
/*
	FILE: Vertex.h


*/

#include <vector>
#include "Math/Math.h"

#define WHITE 255
#define BLACK 0

struct Color
{
	uint8_t R = 255;
	uint8_t G = 255;
	uint8_t B = 255;
	uint8_t A = 255;
};

struct Vertex
{
	Vec2 UV;            // 8
	Vec3 Normal;        // 12
	Vec3 Pos;           // 12
	Color Color;        // 4
	float W;
};