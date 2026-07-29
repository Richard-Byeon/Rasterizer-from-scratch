/*
	FILE: Raterizer.cpp

	LAST MODIFIDED: Added UV interpolation to RasterizeTriangle()
*/

#include "Rasterizer.h"


float EdgeEquation(const Vec3& P, const Vec3& v1, const Vec3& v2)
{
	return (P.v[0] - v1.v[0]) * (v2.v[1] - v1.v[1]) - (P.v[1] - v1.v[1]) * (v2.v[0] - v1.v[0]);
}

BoundingBox ComputeBoundingBox(const Vec3& v1, const Vec3& v2, const Vec3& v3)
{
	float xmin = floor(std::min({ v1.v[0], v2.v[0], v3.v[0] }));
	float xmax = ceil(std::max({ v1.v[0], v2.v[0], v3.v[0] }));

	float ymin = floor(std::min({ v1.v[1], v2.v[1], v3.v[1] }));
	float ymax = ceil(std::max({ v1.v[1], v2.v[1], v3.v[1] }));

	BoundingBox result = { xmin, ymin, xmax, ymax };
	
	return result;
}

Color ShadeFragment(const Vertex& v1, const Vertex& v2, const Vertex& v3,
	float l1, float l2, float l3, const Texture& Tex)
{
	float u = l1 * v1.UV.v[0] + l2 * v2.UV.v[0] + l3 * v3.UV.v[0];
	float v = l1 * v1.UV.v[1] + l2 * v2.UV.v[1] + l3 * v3.UV.v[1];

	return Sample(Tex, Vec2(u, v));
}

void RasterizeTriangle(
	const Vertex& v1, const Vertex& v2, const Vertex& v3,
	const BoundingBox& Bbox,
	std::vector<Color>& FrameBuffer,
	std::vector<float>& ZBuffer,
	int Width, int Height, const Texture& Tex)
{
	float Area2 = EdgeEquation(v3.Pos, v1.Pos, v2.Pos);
	
	if (Area2 == 0.0f) return;
	
	float invArea2 = 1.0f / Area2;

	float invW1 = 1.0f / v1.W;
	float invW2 = 1.0f / v2.W;
	float invW3 = 1.0f / v3.W;
	
	// Clamping
	int x0 = std::max((int)Bbox.xmin, 0);
	int x1 = std::min((int)Bbox.xmax, Width - 1);
	int y0 = std::max((int)Bbox.ymin, 0);
	int y1 = std::min((int)Bbox.ymax, Height - 1);

	for (int y = y0; y <= y1; y++)
	{
		for (int x = x0; x <= x1; x++)
		{
			int idx = y * Width + x;

			float a, b, c;
			float l1, l2, l3;

			// Moved sample point from top-left corner to center of the pixel
			Vec3 P = { x + 0.5f, y + 0.5f };

			a = EdgeEquation(P, v1.Pos, v2.Pos);
			b = EdgeEquation(P, v2.Pos, v3.Pos);
			c = EdgeEquation(P, v3.Pos, v1.Pos);

			bool inside = (Area2 > 0) ?	 (a >= 0 && b >= 0 && c >=0) 
									  : (a <= 0 && b <= 0 && c <= 0);

			if (!inside)
				continue;

			l1 = b * invArea2;
			l2 = c * invArea2;
			l3 = a * invArea2;
			
			// Depth interpolate
			float z = l1 * v1.Pos.v[2] + l2 * v2.Pos.v[2] + l3 * v3.Pos.v[2];
			
			if (z >= ZBuffer[idx])
				continue;

			float denom = l1 * invW1 + l2 * invW2 + l3 * invW3;
			float invDenom = 1.0f / denom;

			float pl1 = l1 * invW1 * invDenom;
			float pl2 = l2 * invW2 * invDenom;
			float pl3 = l3 * invW3 * invDenom;

			ZBuffer[idx] = z;

			// perpective correction (UV)

			/*uint8_t R = (uint8_t)(l1 * v1.Color.R + l2 * v2.Color.R + l3 * v3.Color.R);
			uint8_t G = (uint8_t)(l1 * v1.Color.G + l2 * v2.Color.G + l3 * v3.Color.G);
			uint8_t B = (uint8_t)(l1 * v1.Color.B + l2 * v2.Color.B + l3 * v3.Color.B);*/
		
			FrameBuffer[idx] = ShadeFragment(v1, v2, v3, pl1, pl2, pl3, Tex);
		}
	}
}
