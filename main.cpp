/*
							REAL-TIME-RENDERER(RASTERIZER) FROM SCRATCH 

			FILE: main.cpp (temporary name)
	
			LAST UPDATE: 8th, JULY, 2026
			LAST MODIFIED FEATURE: VIEWPORT TRANSFORM & PERSPECTIVE DIVISION & W GUARD
*/

#include				 <algorithm>
#include				 <SDL3/SDL.h>

#include				"Math/Math.h"
#include	  "Bresenhem/Bresenhem.h"
#include  "Barycentric/Barycentric.h"
#include			"Camera/Camera.h"
#include	 "Vertex/VertexShading.h"

// ASPECT = HEIGHT(600) / WIDTH(800) = 4/3

constexpr float ASPECT = (float)WIDTH / (float)HEIGHT;

float fovy = 60 * (PI / 180.0f);
float fovx = 2 * atan(ASPECT * tan(fovy / 2));

void GenerateUVSphere(int rings, int segments, float radius,
	std::vector<Vertex>& vbuf, std::vector<uint32_t>& ibuf)
{
	vbuf.clear();
	ibuf.clear();
	vbuf.reserve(2 + rings * segments);
	ibuf.reserve(3 * (2 * segments + 2 * (rings - 1) * segments));

	auto colorFromNormal = [](float nx, float ny, float nz) -> Color {
		// 법선 방향 [-1,1] → [28, 228] 매핑: 부드러운 위치 기반 그라디언트
		auto ch = [](float n) { return (uint8_t)(128.0f + 100.0f * n + 0.5f); };
		return { ch(nx), ch(ny), ch(nz), 255 };
		};

	// top pole
	Vertex v;
	v.Pos = { 0.0f, radius, 0.0f };
	v.Color = colorFromNormal(0.0f, 1.0f, 0.0f);
	vbuf.push_back(v);

	// rings
	for (int r = 1; r <= rings; r++)
	{
		float theta = PI * (float)r / (float)(rings + 1);
		float y = cosf(theta);
		float rr = sinf(theta);

		for (int s = 0; s < segments; s++)
		{
			float phi = 2.0f * PI * (float)s / (float)segments;
			float nx = rr * cosf(phi);
			float nz = rr * sinf(phi);
			v.Pos = { radius * nx, radius * y, radius * nz };
			v.Color = colorFromNormal(nx, y, nz);
			vbuf.push_back(v);
		}
	}

	// bottom pole
	v.Pos = { 0.0f, -radius, 0.0f };
	v.Color = colorFromNormal(0.0f, -1.0f, 0.0f);
	vbuf.push_back(v);

	uint32_t bottomPole = (uint32_t)(1 + rings * segments);

	// top cap: (pole, s+1, s)
	for (int s = 0; s < segments; s++)
	{
		uint32_t a = 1 + s;
		uint32_t b = 1 + (s + 1) % segments;
		ibuf.insert(ibuf.end(), { 0u, b, a });
	}

	// bands: (a_s, b_{s+1}, b_s), (a_s, a_{s+1}, b_{s+1})
	for (int r = 0; r < rings - 1; r++)
	{
		uint32_t baseA = 1 + r * segments;
		uint32_t baseB = 1 + (r + 1) * segments;
		for (int s = 0; s < segments; s++)
		{
			uint32_t a0 = baseA + s;
			uint32_t a1 = baseA + (s + 1) % segments;
			uint32_t b0 = baseB + s;
			uint32_t b1 = baseB + (s + 1) % segments;
			ibuf.insert(ibuf.end(), { a0, b1, b0 });
			ibuf.insert(ibuf.end(), { a0, a1, b1 });
		}
	}

	// bottom cap: (pole, s, s+1)
	{
		uint32_t baseA = 1 + (rings - 1) * segments;
		for (int s = 0; s < segments; s++)
		{
			uint32_t a0 = baseA + s;
			uint32_t a1 = baseA + (s + 1) % segments;
			ibuf.insert(ibuf.end(), { bottomPole, a0, a1 });
		}
	}
}

int main(int argc, char* argv[])
{
	std::vector<Color>			Framebuffer(WIDTH * HEIGHT);
	std::vector<float>			ZBuffer(WIDTH * HEIGHT, 1.0f);
	std::vector<Vertex>			VertexBuffer;
	std::vector<uint32_t>		IndexBuffer;

	GenerateUVSphere(23, 32, 70.0f, VertexBuffer, IndexBuffer);

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("SDL_Init failed : %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Rasterizer", WIDTH, HEIGHT, 0);
	
	if (!window)
	{
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	if (!renderer)
	{
		SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	
	SDL_Texture* texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH, HEIGHT
	);

	Camera camera;
	std::vector<Vertex> VertexBufferOriginal = VertexBuffer;

	Vec3 Translation = { 400, 300, 300 };
	Vec3       Scale = { 1.3, 1.3, 1.3 };
	const float RotationSpeedDegPerSec = 45.0f;

	camera.AT = Translation;
	camera.EYE = { 400, 100, 30 };
	camera.UP = { 0, 1, 0 };

	camera.n = normal(camera.AT - camera.EYE);
	camera.u = normal(cross(camera.UP, camera.n));
	camera.v = normal(cross(camera.n, camera.u));

	Uint64 startTicks = SDL_GetTicks();


	bool running = true;
	SDL_Event event;
	
    while (running)
	{
        while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				running = false;
        }
		
		Uint64 nowTicks = SDL_GetTicks();
		float elapsedSec = (nowTicks - startTicks) / 1000.0f;
		Vec3 Angle = {
			fmodf(elapsedSec * 30.0f,  360.0f),   // x
			fmodf(elapsedSec * 45.0f,  360.0f),   // y
			0.0f   // z
		};

		std::fill(Framebuffer.begin(), Framebuffer.end(), Color{});
		std::fill(ZBuffer.begin(), ZBuffer.end(), 1.0f);

		VertexBuffer = VertexBufferOriginal;
		ModelTransform(VertexBuffer, Translation, Angle, Scale);
		ViewTransform(VertexBuffer, camera);
		PerspectiveProjection(VertexBuffer, fovy, ASPECT);
		ViewPortTransfrom(VertexBuffer, WIDTH, HEIGHT);
		Draw(VertexBuffer, IndexBuffer, Framebuffer, ZBuffer);

		SDL_RenderClear(renderer);
		SDL_UpdateTexture(texture, nullptr, Framebuffer.data(), WIDTH * sizeof(Color));
		SDL_RenderTexture(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


	return 0;
}