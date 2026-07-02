#include "VertexShading.h"

void ModelTransform(
	std::vector<Vertex>& vBuffer, 
	const Vec3& translation,
	const Vec3& angle,
	const Vec3& scale)
{
	int VertexCount = vBuffer.size();

	Vec4* Vertices = new Vec4[VertexCount];
	
	// From vec3 -> vec4
	for (int i = 0; i < VertexCount; i++)
	{
		Vertices[i] = Vec4(vBuffer[i].Pos, 1);
	}

	Mat4 Model;

	Model = HomogenousMatrix(translation, angle, scale);

	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = Model * Vertices[i];
	
	for (int i = 0; i < VertexCount; i++)
	{
		vBuffer[i].Pos.v[0] = Vertices[i].v[0];
		vBuffer[i].Pos.v[1] = Vertices[i].v[1];
		vBuffer[i].Pos.v[2] = Vertices[i].v[2];
	}

	delete[] Vertices;
}