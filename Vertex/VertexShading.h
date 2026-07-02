#pragma once

/*
	FILE: VertexShading.h

*/

#include "Vertex.h"
#include "Camera/Camera.h"

std::vector<Vertex> ModelTransform(
	std::vector<Vertex>& vBufferIn,
	const Vec3& translation, 
	const Vec3& angle, 
	const Vec3& scale
);

std::vector<Vertex> ViewTransform(std::vector<Vertex>& vBufferIn, const Camera& Camera);