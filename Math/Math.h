#pragma once

#include <cmath>
/////////////////////////////////
//// Vectors & Matrix struct ////
/////////////////////////////////

struct Vec2 
{
	float v[2];
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
};

struct Mat4 {
	Vec4 m[4];
	
	// another big question: how do we implement such thing as a homogenous matrix? 
	// T = [R T]
	//	   [O 1] >> we need to keep this format, but not sure how....
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
Vec3 operator*(const Vec3& v1, float s);			// Multiplied by scalar s
Vec3 operator*(float s, const Vec3& v1);			// Commutative law
float dot(const Vec3& v1, const Vec3& v2);			// Dot product
Vec3 cross(const Vec3& v1, const Vec3& v2);			// Cross prodcut
float length(const Vec3& v);						// Get a length of the target vector
Vec3 normalize(const Vec3& v);						// Normalize 

////////////////////////////
////	VEC4 FUNCT	    ////
///////////////////////////

Vec4 cross(const Vec4& v1, const Vec4& v2);


////////////////////////////
////	MAT4 FUNCT	    ////
///////////////////////////

Mat4 operator*(const Mat4&, float s);				// M * scalar
Mat4 operator*(const Mat4& m1, const Mat4& m2);		// M * M
Vec4 operator*(const Mat4& m, const Vec4& v);		// M * scalar
Mat4 transpose(const Mat4& m);						// transpose
Mat4& invert(const Mat4& m);						// invert

/////////////////////////
////	TRNSFRMTN	 ////
/////////////////////////

// translation
Mat4 translation(const Mat4& m, float translation);

// rotation
Mat4 rotation(const Mat4& m, float angle);

// scale
Mat4 scale(const Mat4& m, float scalar);

Mat4 projcetion(const Mat4& src, const Mat4& dst);