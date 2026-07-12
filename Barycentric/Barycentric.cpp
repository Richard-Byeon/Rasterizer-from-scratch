#include "Barycentric.h"


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

void RasterizeTriangle(
	const Vertex& v1, const Vertex& v2, const Vertex& v3,
	const BoundingBox& Bbox,
	std::vector<Color>& FrameBuffer,
	std::vector<float>& ZBuffer,
	int Width)
{
	float W = EdgeEquation(v3.Pos, v1.Pos, v2.Pos);
	if (W == 0.0f)
		return;   // degenerate triangle, skip
	float invW = 1.0f / W;

	for (float y = Bbox.ymin; y <= Bbox.ymax; y++)
	{
		for (float x = Bbox.xmin; x <= Bbox.xmax; x++)
		{
			float a, b, c;
			float l1, l2, l3;

			Vec3 P = { x, y };

			a = EdgeEquation(P, v1.Pos, v2.Pos);
			b = EdgeEquation(P, v2.Pos, v3.Pos);
			c = EdgeEquation(P, v3.Pos, v1.Pos);

			bool inside = (W > 0) ? (a >= 0 && b >= 0 && c >=0) 
				: (a <= 0 && b <= 0 && c <= 0);

			if (!inside)
				continue;

			l1 = b * invW;
			l2 = c * invW;
			l3 = a * invW;
			
			float z = l1 * v1.Pos.v[2] + l2 * v2.Pos.v[2] + l3 * v3.Pos.v[2];
			P.v[2] = z;

			int idx = (int)y * Width + (int)x;

			ZBuffer[idx] = { P.v[2] };

			uint8_t R = (uint8_t)(l1 * v1.Color.R + l2 * v2.Color.R + l3 * v3.Color.R);
			uint8_t G = (uint8_t)(l1 * v1.Color.G + l2 * v2.Color.G + l3 * v3.Color.G);
			uint8_t B = (uint8_t)(l1 * v1.Color.B + l2 * v2.Color.B + l3 * v3.Color.B);

			FrameBuffer[idx] = { R, G, B, 255 };
		}
	}
}