#include "Math.h"

// Vec2, 3, 4 init

Vec2::Vec2(float x, float y)					:	v{x, y} {}

Vec3::Vec3(float x, float y, float z)			:	v{ x, y, z } {}

Vec4::Vec4(Vec3 pos, float w)					:	v{ pos.v[0], pos.v[1], pos.v[2], w } {}
Vec4::Vec4(float x, float y, float z, float w)	:	v{ x, y, z, w } {}

Vec2 operator+(const Vec2& A, const Vec2& B)
{
	return Vec2(A.v[0] + B.v[0], 
				A.v[1] + B.v[1]);
}
Vec3 operator-(const Vec3& A, const Vec3& B)
{
	return Vec3(A.v[0] - B.v[0],
				A.v[1] - B.v[1],
				A.v[2] - B.v[2]);
}
Vec3 operator+(const Vec3& A, const Vec3& B)
{
	return Vec3(A.v[0] + B.v[0], 
				A.v[1] + B.v[1], 
				A.v[2] + B.v[2]);
}


Vec3 operator*(const Vec3& A, float s)
{
	return Vec3(A.v[0] * s, 
				A.v[1] * s, 
				A.v[2] * s);
}

Vec3 operator*(float s, const Vec3& A)
{
	return Vec3(A.v[0] * s,
				A.v[1] * s,
				A.v[2] * s);
}


float dot(const Vec3& A, const Vec3& B)
{
	return ( A.v[0] * B.v[0] + 
			 A.v[1] * B.v[1] + 
			 A.v[2] * B.v[2] );
}

Vec3 cross(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1], 
				v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2], 
				v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]);
}

float length(const Vec3& v)
{
	float sq = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];

	return std::sqrt(sq);
}

Vec3 normal(const Vec3& v)
{
	float len = length(v);

	if (len == 0.00f) return v;

	return Vec3((1/len) * v);
}

// Cross product of two Vec4
Vec4 cross(const Vec4& A, const Vec4& B)
{
	return Vec4(A.v[1] * B.v[2] - A.v[2] * B.v[1], 
				A.v[2] * B.v[0] - A.v[0] * B.v[2], 
				A.v[0] * B.v[1] - A.v[1] * B.v[0], 0);
}

// Constructor of Mat4 sets homogenous matrix as a identity matrix
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

Mat4 RotationX(float angle = 0.0f)
{
	angle = angle * (PI / 180);

	Mat4 Matrix;
	Matrix.m[5] = cos(angle);	Matrix.m[6]	 = -sin(angle);
	Matrix.m[9] = sin(angle);	Matrix.m[10] = cos(angle);

	return Matrix;
}

Mat4 RotationY(float angle = 0.0f)
{
	angle = angle * (PI / 180);

	Mat4 Matrix;
	Matrix.m[0] =  cos(angle);	Matrix.m[2]  = sin(angle);
	Matrix.m[8] = -sin(angle);	Matrix.m[10] = cos(angle);

	return Matrix;
}

Mat4 RotationZ(float angle = 0.0f)
{
	angle = angle * (PI / 180);

	Mat4 Matrix;
	Matrix.m[0] = cos(angle);	Matrix.m[1] = -sin(angle);
	Matrix.m[4] = sin(angle);	Matrix.m[5] = cos(angle);

	return Matrix;
}

Mat4 Scale(const Vec3& scalar)
{	
	Mat4 Matrix;

	Matrix.m[0] = scalar.v[0];
	Matrix.m[5] = scalar.v[1];
	Matrix.m[10] = scalar.v[2];
	
	return Matrix;
}

Mat4 Translation(const Vec3& v)
{
	Mat4 Matrix;

	Matrix.m[3]	 = v.v[0];
	Matrix.m[7]	 = v.v[1];
	Matrix.m[11] = v.v[2];

	return Matrix;
}

Mat4 HomogenousMatrix(const Vec3& translation, const Vec3& angle, const Vec3& scale)
{
	Mat4 T;
	Mat4 R;
	Mat4 S;

	T = Translation(translation);
	R = RotationZ(angle.v[2]) * RotationY(angle.v[1]) * RotationX(angle.v[0]);
	S = Scale(scale);

	// Product order: RZ * RY * RX ( yaw * pitch * roll )

	return T * R * S;
}

