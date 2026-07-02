#pragma once

#include <cmath>
#define PI 3.141592

struct Vec2 
{
	float v[2];			// [x][y]
	Vec2(float x = 0.0f, float y = 0.0f);
};

struct Vec3
{
	float v[3];
	Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
};

struct Vec4
{
	float v[4];
	Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
	Vec4(Vec3 pos, float w = 0.0f);
};

// Mat4 struct uses 1D array, accessing convention is like this: m[4 * rows + col].
// Mat4 feature: matrix * scalar // matrix * vector // matrix mul // transpose
struct Mat4 {
	float m[16];
	Mat4();	
};

////////////////////////////
////	VEC4 FUNCT	    ////
///////////////////////////

Vec2 operator+(const Vec2& v1, const Vec2& v2);

////////////////////////////
////	VEC4 FUNCT	    ////
///////////////////////////

Vec3 operator+(const Vec3& v1, const Vec3& v2);		// Simple addition
Vec3 operator-(const Vec3& v1, const Vec3& v2);
Vec3 operator*(const Vec3& v1, float s);			// Multiplied by scalar s
Vec3 operator*(float s, const Vec3& v1);			// Commutative law
float dot(const Vec3& v1, const Vec3& v2);			// Dot product
Vec3 cross(const Vec3& v1, const Vec3& v2);			// Cross prodcut
float length(const Vec3& v);						// Get a length of the target vector
Vec3 normal(const Vec3& v);							// Normalize 

////////////////////////////
////	VEC4 FUNCT	    ////
///////////////////////////

Vec4 cross(const Vec4& v1, const Vec4& v2);

////////////////////////////
////	MAT4 FUNCT	    ////
///////////////////////////

Mat4 operator*(const Mat4&, float);				
Mat4 operator*(const Mat4&, const Mat4&);		
Vec4 operator*(const Mat4&, const Vec4&);		
Mat4 transpose(const Mat4& m);						
Mat4& invert(const Mat4& m);						
Mat4 HomogenousMatrix(const Vec3& translation, const Vec3& angle, const Vec3& scale);

/////////////////////////
////	TRNSFRMTN	 ////
/////////////////////////

// Rotation
Mat4 RotationX(float angle);
Mat4 RotationY(float angle);
Mat4 RotationZ(float angle);

// Scale
Mat4 Scale(const Vec3& scalar = {1, 1, 1});

Mat4 Translation(const Vec3& v);