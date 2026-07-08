#pragma once

/*
	FILE: VertexShading.h

*/

#include "Vertex.h"
#include "Camera/Camera.h"

struct ViewPort
{
	float TopLeftX = 0.0f;
	float TopLeftY = 0.0f;
	float Width;
	float Height;
	float MinDepth = 0.0f;
	float MaxDepth = 1.0f;
};

void ModelTransform(
	std::vector<Vertex>& vBufferIn,
	const Vec3& translation, 
	const Vec3& angle, 
	const Vec3& scale
);

void ViewTransform(std::vector<Vertex>& vBufferIn, const Camera& Camera);

void OrthographicProjection(std::vector<Vertex>& vBufferIn);

void PerspectiveProjection(std::vector<Vertex>& vBufferIn, float fovy, float ASPECT);

void ViewPortTransfrom(std::vector<Vertex>& vBufferIn, int width, int height);