#include "Math.h"

// Vec2, 3, 4 init

Vec2::Vec2(float x, float y)					:	v{x, y} {}

Vec3::Vec3(float x, float y, float z)			:	v{ x, y, z } {}

Vec4::Vec4(float x, float y, float z, float w)	:	v{ x, y, z, w } {}


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
	float sq = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];

	return std::sqrt(sq);
}

Vec3 normal(const Vec3& v)
{
	Vec3 result;
	float len = length(v);

	if (len == 0.00f) return v;

	result = (1 / len) * v; 

	return result;
}

Vec4 cross(const Vec4& v1, const Vec4& v2)
{
	Vec4 result;

	result.v[0] = (v1.v[1] * v2.v[2]) - (v1.v[2] * v2.v[1]);
	result.v[1] = (v1.v[2] * v2.v[0]) - (v1.v[0] * v2.v[2]);
	result.v[2] = (v1.v[0] * v2.v[1]) - (v1.v[1] * v2.v[0]);
	
	result.v[3] = 0;

	return result;
}

// Initialize homogenous matrix as a identity matrix
// Row-major convention
Mat4::Mat4()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		
			m[4 * i + j] = 0;
		
	}
	// It's possible to set diagonal of M as 1 in the loop, but if that's within the loop it's inevitable to branch during loop.
	m[0] = m[5] = m[10] = m[15] = 1;
}

Mat4 operator*(const Mat4& src, float s)
{
	Mat4 result = src;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			result.m[4 * i + j] *= s;
	}

	return result;
}

Mat4 operator*(const Mat4& m1, const Mat4& m2)
{
	Mat4 c;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			c.m[4 * i + j] = 0;

			for (int k = 0; k < 4; k++)
				c.m[4 * i + j] += m1.m[4 * i + k] * m2.m[4 * k + j];
		}
	}
	
	return c;
}

// Row convention
Vec4 operator*(const Mat4& mat, const Vec4& vec)
{
	return Vec4(
		(mat.m[0]  * vec.v[0] + mat.m[1]  * vec.v[1] + mat.m[2]  * vec.v[2] + mat.m[3]  * vec.v[3]), // ROW 1
		(mat.m[4]  * vec.v[0] + mat.m[5]  * vec.v[1] + mat.m[6]  * vec.v[2] + mat.m[7]  * vec.v[3]), // ROW 2
		(mat.m[8]  * vec.v[0] + mat.m[9]  * vec.v[1] + mat.m[10] * vec.v[2] + mat.m[11] * vec.v[3]), // ROW 3
		(mat.m[12] * vec.v[0] + mat.m[13] * vec.v[1] + mat.m[14] * vec.v[2] + mat.m[15] * vec.v[3])  // ROW 4
	);

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
				float temp = tmp.m[4 * i + j];
				tmp.m[4 * i + j] = tmp.m[4 * j + i];
				tmp.m[4 * j + i] = temp;
			}

		}
	}

	return tmp;
}

