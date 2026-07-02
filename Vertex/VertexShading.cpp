#include "VertexShading.h"

std::vector<Vertex> ModelTransform(
	std::vector<Vertex>& vBufferIn, 
	const Vec3& translation,
	const Vec3& angle,
	const Vec3& scale)
{
	
	int VertexCount = vBufferIn.size();

	std::vector<Vertex> vBufferOut(VertexCount);
	std::vector<Vec4>	Vertices(VertexCount);

	
	// From vec3 -> vec4
	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = Vec4(vBufferIn[i].Pos, 1);

	Mat4 ModelM;

	ModelM = HomogenousMatrix(translation, angle, scale);

	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = ModelM * Vertices[i];
	
	for (int i = 0; i < VertexCount; i++)
	{
		vBufferOut[i].Pos.v[0] = Vertices[i].v[0];
		vBufferOut[i].Pos.v[1] = Vertices[i].v[1];
		vBufferOut[i].Pos.v[2] = Vertices[i].v[2];
	}

	return vBufferOut; // this goes straight to ViewTransform(...)
}

std::vector<Vertex> ViewTransform(std::vector<Vertex>& vBufferIn, const Camera& Camera)
{
	int VertexCount = vBufferIn.size();
	std::vector<Vertex> vBufferOut(VertexCount);
	std::vector<Vec4>	Vertices(VertexCount);

	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = Vec4(vBufferIn[i].Pos, 1);
	

	Mat4 ViewM;
	Mat4 T;
	Mat4 R;

	Vec3 EYE = Camera.EYE;
	Vec3 U, V, N;

	U = Camera.u;
	V = Camera.v;
	N = Camera.n;
	
	EYE.v[0] = -EYE.v[0];
	EYE.v[1] = -EYE.v[1];
	EYE.v[2] = -EYE.v[2];

	T = Translation(EYE);

	for (int i = 0; i < 3; i++) R.m[i]	   = U.v[i];
	for (int i = 0; i < 3; i++) R.m[4 + i] = V.v[i];
	for (int i = 0; i < 3; i++) R.m[8 + i] = N.v[i];

	ViewM = R * T;

	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = ViewM * Vertices[i];

	for (int i = 0; i < VertexCount; i++)
	{
		vBufferOut[i].Pos.v[0] = Vertices[i].v[0];
		vBufferOut[i].Pos.v[1] = Vertices[i].v[1];
		vBufferOut[i].Pos.v[2] = Vertices[i].v[2];
	}

	return vBufferOut; // this goes straight to PerspectiveTransform
}