/*
	FILE: Barycentric.h

	CONTENTS : BoundingBox struct, EdgeEquation(), ComputeBoundingBox(), DepthTest() and more
*/

#pragma once

#include <algorithm>
#include "Vertex/Vertex.h"

struct BoundingBox
{
	float xmin, ymin, xmax, ymax;
};

// In here, Vec4 type variable requires 
float EdgeEquation(const Vec3& P, const Vec3& v1, const Vec3& v2);

// Create bounding box
BoundingBox ComputeBoundingBox(const Vec3& v1, const Vec3& v2, const Vec3& v3);

void RasterizeTriangle(
	const Vec3& v1, const Vec3& v2, const Vec3& v3,
	const BoundingBox& Bbox,
	std::vector<Color>& FrameBuffer,
	std::vector<float>& ZBuffer,
	int Width
);