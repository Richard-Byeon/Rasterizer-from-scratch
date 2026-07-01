#pragma once
/*
	FILE: Vertex.h


*/

#include "Math/Math.h"

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
	Color Color;        // 16
};