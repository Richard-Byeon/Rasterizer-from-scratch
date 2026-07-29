#pragma once

#include "Vertex/Vertex.h"
#include "UV/Texturing.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

void Bresenhem(int x0, int y0, int x1, int x2, std::vector<Color>& dest);

// void ClearBuffer(std::vector<Color>& fBuffer);

bool isBackFace(const Vec4& v1, const Vec4& v2, const Vec4& v3);


void Draw(std::vector<Vertex>& vbuffer, std::vector<uint32_t>& ibuffer, std::vector<Color>& fbuffer);
void Draw(std::vector<Vertex>& vbuffer, std::vector<uint32_t>& ibuffer, std::vector<Color>& fbuffer, std::vector<float>& zbuffer, const Texture& Tex);