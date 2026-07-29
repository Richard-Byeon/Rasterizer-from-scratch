/*
	FILE: VertexShading.cpp

*/

#include "VertexShading.h"

void ModelTransform(std::vector<Vertex>& vBufferIn, const Mat4& TransformM, const Mat4& NormalM)
{
	int vertexCount = vBufferIn.size();

	for (int i = 0; i < vertexCount; i++)
	{
		Vec4 p = TransformM * Vec4(vBufferIn[i].Pos,	1.0f);
		Vec4 n = NormalM	* Vec4(vBufferIn[i].Normal, 0.0f);
		
		vBufferIn[i].Pos = { p.v[0], p.v[1], p.v[2] };
		vBufferIn[i].Normal = normal({ n.v[0], n.v[1], n.v[2] });
	}
}

void ViewTransform(std::vector<Vertex>& vBufferIn, const Camera& Camera)
{
	int vertexCount = vBufferIn.size();
	/*std::vector<Vertex> vBufferOut(VertexCount);*/
	std::vector<Vec4>	Vertices(vertexCount);

	for (int i = 0; i < vertexCount; i++)
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

	for (int i = 0; i < vertexCount; i++)
		Vertices[i] = ViewM * Vertices[i];

	for (int i = 0; i < vertexCount; i++)
	{
		vBufferIn[i].Pos.v[0] = Vertices[i].v[0];
		vBufferIn[i].Pos.v[1] = Vertices[i].v[1];
		vBufferIn[i].Pos.v[2] = Vertices[i].v[2];
	}

	//return vBufferOut; // this goes straight to PerspectiveTransform
}

void OrthographicProjection(std::vector<Vertex>& vBufferIn)
{
	Mat4 ProjM;
	ProjM.m[10] = 0;

	int VertexCount = vBufferIn.size();
	std::vector<Vec4>	Vertices(VertexCount);

	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = Vec4(vBufferIn[i].Pos, 0);


	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = ProjM * Vertices[i];

	for (int i = 0; i < VertexCount; i++)
	{
		vBufferIn[i].Pos.v[0] = Vertices[i].v[0];
		vBufferIn[i].Pos.v[1] = Vertices[i].v[1];
		vBufferIn[i].Pos.v[2] = Vertices[i].v[2];
	}
}

// Spaghetti code. refactor required
void PerspectiveProjection(std::vector<Vertex>& vBufferIn, float fovy, float ASPECT)
{
	Mat4 ProjM;		
	float cot = 1 / tan(fovy / 2);
	float f, n;
	f = 1500.0f; n = 1.0f;
	ProjM.m[0] = cot / ASPECT;
	ProjM.m[5] = cot;
	ProjM.m[10] = f / (f - n);
	ProjM.m[11] = 1;
	ProjM.m[14] = -(f * n) / (f - n);
	ProjM.m[15] = 0;

	ProjM = Transpose(ProjM);

	int VertexCount = vBufferIn.size();
	std::vector<Vec4>	Vertices(VertexCount);

	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = Vec4(vBufferIn[i].Pos, 1);


	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = ProjM * Vertices[i];

	for (int i = 0; i < VertexCount; i++)
	{
		vBufferIn[i].Pos.v[0] = Vertices[i].v[0];
		vBufferIn[i].Pos.v[1] = Vertices[i].v[1];
		vBufferIn[i].Pos.v[2] = Vertices[i].v[2];
		vBufferIn[i].W		  = Vertices[i].v[3];
	}
}

void ViewPortTransfrom(std::vector<Vertex>& vBufferIn, int width, int height)
{
	// Perspective Division first
	int VertexCount = vBufferIn.size();
	std::vector<Vec4>	Vertices(VertexCount);

	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = Vec4(vBufferIn[i].Pos, vBufferIn[i].W);

	
	for (int i = 0; i < VertexCount; i++)
	{
		float w = Vertices[i].v[3];

		Vertices[i].v[0] /= w; 
		Vertices[i].v[1] /= w;
		Vertices[i].v[2] /= w;
		Vertices[i].v[3] = 1.0f;
		vBufferIn[i].W = w;
	}

	ViewPort View;
	
	Mat4 ViewPortM;
	Mat4 S;
	Mat4 T;

	View.Height = height;
	View.Width = width;

	S = Scale({ (float)width / 2, -(float)height / 2, View.MaxDepth - View.MinDepth });
	T = Translation({ View.TopLeftX + width / 2, View.TopLeftY + height / 2, View.MinDepth });
	ViewPortM = T * S;

	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = ViewPortM * Vertices[i];

	for (int i = 0; i < VertexCount; i++)
	{
		vBufferIn[i].Pos.v[0] = Vertices[i].v[0];
		vBufferIn[i].Pos.v[1] = Vertices[i].v[1];
		vBufferIn[i].Pos.v[2] = Vertices[i].v[2];
	}
}