#pragma once

/*
	FILE: VertexShading.h

*/

#include "Vertex.h"

void ModelTransform(
	std::vector<Vertex>& vBuffer,
	const Vec3& translation, 
	const Vec3& angle, 
	const Vec3& scale);