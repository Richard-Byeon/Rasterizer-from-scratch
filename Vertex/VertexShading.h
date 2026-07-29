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
// Too many arugments: Needs to be fixed later.
void ModelTransform(std::vector<Vertex>& vBufferIn, const Mat4& TransfromM, const Mat4& NormalM);

void ViewTransform(std::vector<Vertex>& vBufferIn, const Camera& Camera);

void OrthographicProjection(std::vector<Vertex>& vBufferIn);

void PerspectiveProjection(std::vector<Vertex>& vBufferIn, float fovy, float ASPECT);

void ViewPortTransfrom(std::vector<Vertex>& vBufferIn, int width, int height);