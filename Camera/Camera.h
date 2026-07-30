#pragma once

#include "Math/Math.h"

struct Camera
{
	Vec3 EYE = { 0.0f, 0.0f, -5.0f };
	Vec3 AT  = { 0.0f, 0.0f, 0.0f };
	Vec3 UP  = { 0.0f, 1.0f, 0.0f };

	// Basis doesn't have to be here.


	Mat4 ViewMatrix() const;
};