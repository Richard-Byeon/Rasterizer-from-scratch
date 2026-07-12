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

// ASPECT = HEIGHT(600) / WIDTH(800) = 3/4

constexpr float ASPECT = (float)WIDTH / (float)HEIGHT;

float fovy = 60 * (PI / 180.0f);
float fovx = 2 * atan(ASPECT * tan(fovy / 2));

int main(int argc, char* argv[])
{
	std::vector<Color>			Framebuffer(WIDTH * HEIGHT);
	std::vector<float>			ZBuffer	   (WIDTH * HEIGHT);
	
	
	int VertexCount = 42;
	int TriangleCount = 80;
	std::vector<Vertex>			VertexBuffer(VertexCount);
	std::vector<uint32_t>		IndexBuffer (TriangleCount);

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
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH, HEIGHT
	);


	Camera camera;

	VertexBuffer[0].Pos = { 0.00, 100.00, 0.00 };   // top pole
	VertexBuffer[1].Pos = { 50.00, 86.60, 0.00 };   // ring 1, seg 0
	VertexBuffer[2].Pos = { 35.36, 86.60, 35.36 };   // ring 1, seg 1
	VertexBuffer[3].Pos = { 0.00, 86.60, 50.00 };   // ring 1, seg 2
	VertexBuffer[4].Pos = { -35.36, 86.60, 35.36 };   // ring 1, seg 3
	VertexBuffer[5].Pos = { -50.00, 86.60, 0.00 };   // ring 1, seg 4
	VertexBuffer[6].Pos = { -35.36, 86.60, -35.36 };   // ring 1, seg 5
	VertexBuffer[7].Pos = { 0.00, 86.60, -50.00 };   // ring 1, seg 6
	VertexBuffer[8].Pos = { 35.36, 86.60, -35.36 };   // ring 1, seg 7
	VertexBuffer[9].Pos = { 86.60, 50.00, 0.00 };   // ring 2, seg 0
	VertexBuffer[10].Pos = { 61.24, 50.00, 61.24 };   // ring 2, seg 1
	VertexBuffer[11].Pos = { 0.00, 50.00, 86.60 };   // ring 2, seg 2
	VertexBuffer[12].Pos = { -61.24, 50.00, 61.24 };   // ring 2, seg 3
	VertexBuffer[13].Pos = { -86.60, 50.00, 0.00 };   // ring 2, seg 4
	VertexBuffer[14].Pos = { -61.24, 50.00, -61.24 };   // ring 2, seg 5
	VertexBuffer[15].Pos = { 0.00, 50.00, -86.60 };   // ring 2, seg 6
	VertexBuffer[16].Pos = { 61.24, 50.00, -61.24 };   // ring 2, seg 7
	VertexBuffer[17].Pos = { 100.00, 0.00, 0.00 };   // ring 3, seg 0
	VertexBuffer[18].Pos = { 70.71, 0.00, 70.71 };   // ring 3, seg 1
	VertexBuffer[19].Pos = { 0.00, 0.00, 100.00 };   // ring 3, seg 2
	VertexBuffer[20].Pos = { -70.71, 0.00, 70.71 };   // ring 3, seg 3
	VertexBuffer[21].Pos = { -100.00, 0.00, 0.00 };   // ring 3, seg 4
	VertexBuffer[22].Pos = { -70.71, 0.00, -70.71 };   // ring 3, seg 5
	VertexBuffer[23].Pos = { 0.00, 0.00, -100.00 };   // ring 3, seg 6
	VertexBuffer[24].Pos = { 70.71, 0.00, -70.71 };   // ring 3, seg 7
	VertexBuffer[25].Pos = { 86.60, -50.00, 0.00 };   // ring 4, seg 0
	VertexBuffer[26].Pos = { 61.24, -50.00, 61.24 };   // ring 4, seg 1
	VertexBuffer[27].Pos = { 0.00, -50.00, 86.60 };   // ring 4, seg 2
	VertexBuffer[28].Pos = { -61.24, -50.00, 61.24 };   // ring 4, seg 3
	VertexBuffer[29].Pos = { -86.60, -50.00, 0.00 };   // ring 4, seg 4
	VertexBuffer[30].Pos = { -61.24, -50.00, -61.24 };   // ring 4, seg 5
	VertexBuffer[31].Pos = { 0.00, -50.00, -86.60 };   // ring 4, seg 6
	VertexBuffer[32].Pos = { 61.24, -50.00, -61.24 };   // ring 4, seg 7
	VertexBuffer[33].Pos = { 50.00, -86.60, 0.00 };   // ring 5, seg 0
	VertexBuffer[34].Pos = { 35.36, -86.60, 35.36 };   // ring 5, seg 1
	VertexBuffer[35].Pos = { 0.00, -86.60, 50.00 };   // ring 5, seg 2
	VertexBuffer[36].Pos = { -35.36, -86.60, 35.36 };   // ring 5, seg 3
	VertexBuffer[37].Pos = { -50.00, -86.60, 0.00 };   // ring 5, seg 4
	VertexBuffer[38].Pos = { -35.36, -86.60, -35.36 };   // ring 5, seg 5
	VertexBuffer[39].Pos = { 0.00, -86.60, -50.00 };   // ring 5, seg 6
	VertexBuffer[40].Pos = { 35.36, -86.60, -35.36 };   // ring 5, seg 7
	VertexBuffer[41].Pos = { 0.00, -100.00, 0.00 };   // bottom pole

	IndexBuffer = {
		// top cap
		0, 2, 1,
		0, 3, 2,
		0, 4, 3,
		0, 5, 4,
		0, 6, 5,
		0, 7, 6,
		0, 8, 7,
		0, 1, 8,
		// band ring1 -> ring2
		1, 10, 9,
		1, 2, 10,
		2, 11, 10,
		2, 3, 11,
		3, 12, 11,
		3, 4, 12,
		4, 13, 12,
		4, 5, 13,
		5, 14, 13,
		5, 6, 14,
		6, 15, 14,
		6, 7, 15,
		7, 16, 15,
		7, 8, 16,
		8, 9, 16,
		8, 1, 9,
		// band ring2 -> ring3
		9, 18, 17,
		9, 10, 18,
		10, 19, 18,
		10, 11, 19,
		11, 20, 19,
		11, 12, 20,
		12, 21, 20,
		12, 13, 21,
		13, 22, 21,
		13, 14, 22,
		14, 23, 22,
		14, 15, 23,
		15, 24, 23,
		15, 16, 24,
		16, 17, 24,
		16, 9, 17,
		// band ring3 -> ring4
		17, 26, 25,
		17, 18, 26,
		18, 27, 26,
		18, 19, 27,
		19, 28, 27,
		19, 20, 28,
		20, 29, 28,
		20, 21, 29,
		21, 30, 29,
		21, 22, 30,
		22, 31, 30,
		22, 23, 31,
		23, 32, 31,
		23, 24, 32,
		24, 25, 32,
		24, 17, 25,
		// band ring4 -> ring5
		25, 34, 33,
		25, 26, 34,
		26, 35, 34,
		26, 27, 35,
		27, 36, 35,
		27, 28, 36,
		28, 37, 36,
		28, 29, 37,
		29, 38, 37,
		29, 30, 38,
		30, 39, 38,
		30, 31, 39,
		31, 40, 39,
		31, 32, 40,
		32, 33, 40,
		32, 25, 33,
		// bottom cap
		41, 33, 34,
		41, 34, 35,
		41, 35, 36,
		41, 36, 37,
		41, 37, 38,
		41, 38, 39,
		41, 39, 40,
		41, 40, 33,
	};

	std::vector<Vertex> VertexBufferOriginal = VertexBuffer;
	
	Vec3 Translation = { 400, 300, 300 };
	Vec3	   Scale = { 1.3, 1.3, 1.3 };
	Vec3	   Angle = { 0, 0, 0 };

	ModelTransform(VertexBuffer, Translation, Angle, Scale);

	camera.AT = Translation;
	camera.EYE = { 400, 100, 30 };
	camera.UP = { 170, 150, 20 };

	camera.n = normal(camera.AT - camera.EYE);
	camera.u = normal(cross(camera.UP, camera.n));
	camera.v = normal(cross(camera.n, camera.u));

	ViewTransform(VertexBuffer, camera);

	PerspectiveProjection(VertexBuffer, fovy, ASPECT);

	ViewPortTransfrom(VertexBuffer, WIDTH, HEIGHT);

	Draw(VertexBuffer, IndexBuffer, Framebuffer);
	

	bool running = true;
	SDL_Event event;
	
    while (running)
	{
        while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				running = false;
        }
		
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