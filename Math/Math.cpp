#include "Math.h"

// Vec2, 3, 4 init

Vec2::Vec2(float x, float y)					: v{x, y} {}
Vec3::Vec3(float x, float y, float z)			: v{ x, y, z } {}
Vec4::Vec4(float x, float y, float z, float w)	: v{ x, y, z, w } {}

Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
	Vec2 result;
	result.v[0] = v1.v[0] + v2.v[0];
	result.v[1] = v1.v[1] + v2.v[1];
	
	return result;
}

Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	Vec3 result;

	result.v[0] = v1.v[0] + v2.v[0];
	result.v[1] = v1.v[1] + v2.v[1];
	result.v[2] = v1.v[2] + v2.v[2];

	return result;
}

Vec3 operator*(const Vec3& v, float s)
{
	Vec3 result;

	result.v[0] = v.v[0] * s;
	result.v[1] = v.v[1] * s;
	result.v[2] = v.v[2] * s;

	return result;
}

Vec3 operator*(float s, const Vec3& v)
{
	Vec3 result;

	result.v[0] = v.v[0] * s;
	result.v[1] = v.v[1] * s;
	result.v[2] = v.v[2] * s;

	return result;
}

float dot(const Vec3& v1, const Vec3& v2)
{
	float result = 0;

	result = v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];

	return result;
}

Vec3 cross(const Vec3& v1, const Vec3& v2)
{
	Vec3 result;

	result.v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
	result.v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
	result.v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];

	return result;
}

float length(const Vec3& v)
{
	// i'm wondering if calculating root causes massive overhead
	// answer to that is NO. Since it's done in FPU (or VPU) [ hardware level ], there is no overhead, so we're gonna use cmath.h
	// then moving on to the next question
	// is there any disadvantages to add additional variable? well, no. since the variables are exists only in register for a short period.
	// but, with that said, it's possible to use additional resource lot. but we're not sure yet.

	// so, i'm just going to use one additional variable so that we have more readability.

	float sq = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];

	return std::sqrt(sq);
}

Vec3 normalize(const Vec3& v)
{
	Vec3 result;
	float len = length(v);

	if (len == 0.00f) return v;

	result = (1 / len) * v; // less division means less clock cycle, and it means it's all gravy baby.

	return result;
}

Vec4 cross(const Vec4& v1, const Vec4& v2)
{
	// w holds no important value. so we just exclude them.
	Vec4 result;

	result.v[0] = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1];
	result.v[1] = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2];
	result.v[2] = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
	
	result.v[3] = 0;

	return result;
}

Mat4::Mat4()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->m[i][j] = 0;
		}
	}
	this->m[0][0] = this->m[1][1] = this->m[2][2] = this->m[3][3] = 1; // Set homogenous matrix as a I.
}

Mat4 operator*(const Mat4& m, float s)
{
	Mat4 target = m;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// this way, it's row majored (which ain't that matter really)
			target.m[i][j] *= s;
		}
	}
	return target;
}

Mat4 operator*(const Mat4& m1, const Mat4& m2)
{
	// at first, i'm going to implement the naive version of matrix multiplication. From later on, i'll use more optimized, sped up 
	// version of M.M.

	Mat4 c;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			c.m[i][j] = 0;

			for (int k = 0; k < 4; k++)
			{
				c.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	
	return c;
	// column pic? or row pic? << row pic is more convenient for me to comprehand, but as for computer, we don't know.
}

Vec4 operator*(const Mat4& M, const Vec4& v)
{
	Vec4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// now this is another problem because we have no way to access Vec4 type by index. -> it wouldn't be very serious
			// problem if we did change the Vec4 struct to array of float types.
			result.v[i] += (M.m[i][j] * v.v[j]);
		}
		
	}

	return result;

}
 
Mat4 transpose(const Mat4& M)
{
	Mat4 tmp = M;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i >= j) continue;
			else
			{
				float temp = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = temp;
			}

		}
	}

	return tmp;
}

