#pragma once

#include "Framebuffer/Framebuffer.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

void Bresenhem(int x0, int y0, int x1, int x2, std::vector<Pixel>& dest);
