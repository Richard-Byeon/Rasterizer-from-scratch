#include "VertexShading.h"

void ModelTransform(
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
	{
		Vertices[i] = Vec4(vBufferIn[i].Pos, 1);
	}

	Mat4 Model;

	Model = HomogenousMatrix(translation, angle, scale);

	for (int i = 0; i < VertexCount; i++)
		Vertices[i] = Model * Vertices[i];
	
	for (int i = 0; i < VertexCount; i++)
	{
		vBufferIn[i].Pos.v[0] = Vertices[i].v[0];
		vBufferIn[i].Pos.v[1] = Vertices[i].v[1];
		vBufferIn[i].Pos.v[2] = Vertices[i].v[2];
	}
}