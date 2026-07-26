/*
							REAL-TIME-RENDERER(RASTERIZER) FROM SCRATCH 

			FILE: main.cpp (temporary name)
	
			LAST UPDATE: 26th, JULY, 2026
			LAST MODIFIED FEATURE: GenerateUVSphere() modifeded, UV coordinate configuration underway
*/

#include				 <algorithm>
#include				 <SDL3/SDL.h>

#include				"Math/Math.h"
#include	  "Bresenhem/Bresenhem.h"
#include  "Barycentric/Barycentric.h"
#include			"Camera/Camera.h"
#include	 "Vertex/VertexShading.h"
#include			"UV/Texturing.h"
// ASPECT = HEIGHT(600) / WIDTH(800) = 4/3

constexpr float ASPECT = (float)WIDTH / (float)HEIGHT;

float fovy = 60 * (PI / 180.0f);
float fovx = 2 * atan(ASPECT * tan(fovy / 2));

void GenerateUVSphere(int stacks, int slices, float radius,
	std::vector<Vertex>& vbuf, std::vector<uint32_t>& ibuf)
{
	vbuf.clear();
	ibuf.clear();
	vbuf.reserve((stacks + 1) * (slices + 1));
	ibuf.reserve(3 * (2 * slices + 2 * (stacks - 1) * slices));

	////////////////////////
	//	Vertex Allocate	 //
	///////////////////////
	Vertex v;
	
	for (int r = 0; r <= stacks; r++)
	{
		float phi = PI * (r / float(stacks));
		// calculate which "floor" this circle will be located in.
		float ny = cosf(phi);
		// calculates radius of this floor's circle
		float Radius = sinf(phi);

		for (int seg = 0; seg <= slices; seg++)
		{
			float theta = 2.0f * PI * (seg / (float)slices);
			// normalized x coordinate
			float nx = Radius * cosf(theta);
			// normalized z coordinate
			float nz = Radius * sinf(theta);
			v.Pos = { radius * nx, radius * ny, radius * nz };
			vbuf.push_back(v);
		}
	}
	int vertexCount = vbuf.size();

	SDL_Log("Vertex Count: %d", vertexCount);

	//////////////////////////////////////////////

	///////////////////////////////////////
	//	Idx Allocate & Face Allocate	//
	//////////////////////////////////////

	// We'll be using quad since the vertices are allocated as a grid and then slice that quad into two trinagle.


}

int main(int argc, char* argv[])
{
	std::vector<Color>			Framebuffer(WIDTH * HEIGHT);
	std::vector<float>			ZBuffer(WIDTH * HEIGHT, 1.0f);
	std::vector<Vertex>			VertexBuffer;
	std::vector<uint32_t>		IndexBuffer;

	GenerateUVSphere(5, 8, 70.0f, VertexBuffer, IndexBuffer);

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
