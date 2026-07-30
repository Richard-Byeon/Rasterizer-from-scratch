/*
	FILE: Camera.cpp
	
	On 30th, July
	I added this file since it seemed to me that calculating u, v, n at the main was bit inefficient and confusing

*/

#include "Camera.h"

Mat4 Camera::ViewMatrix() const
{
	
	Vec3 n = normal(this->AT - this->EYE);
	Vec3 u = normal(cross(this->UP, n));
	Vec3 v = cross(n, u);

	Mat4 V;

	V.m[0] = u.v[0]; V.m[1] = u.v[1]; V.m[2]  = u.v[2];	 V.m[3]  = -dot(u, this->EYE); // ROW 1
	V.m[4] = v.v[0]; V.m[5] = v.v[1]; V.m[6]  = v.v[2];  V.m[7]  = -dot(v, this->EYE); // ROW 2
	V.m[8] = n.v[0]; V.m[9] = n.v[1]; V.m[10] = n.v[2];  V.m[11] = -dot(n, this->EYE); // ROW 3
	
	return V;
}