/*
							REAL-TIME-RENDERER(RASTERIZER) FROM SCRATCH 

			FILE: main.cpp (temporary)
	
			LAST UPDATE: 2nd, JULY, 2026
			LAST MODIFIED FEATURE: VERTEX INDEXING (IA) && VERTEX SHADING
*/


#include				 <SDL3/SDL.h>
#include				"Math/Math.h"
#include	  "Bresenhem/Bresenhem.h"
#include  "Barycentric/Barycentric.h"
#include			"Camera/Camera.h"
#include	 "Vertex/VertexShading.h"



int main(int argc, char* argv[])
{
	std::vector<Color>			Framebuffer(WIDTH * HEIGHT);
	std::vector<float>			ZBuffer(WIDTH * HEIGHT);

	
	int VertexCount = 20;
	int TriangleCount = 36;
	std::vector<Vertex>			VertexBuffer(VertexCount);
	std::vector<uint32_t>		IndexBuffer(TriangleCount);

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

	// VERTICES
	//Vec3 v0, v1, v2, 
	//	 v3, v4, v5;

	//v0 = { 100, 100, 0 };
	//v1 = { 250, 100, 0 };
	//v2 = { 400, 100, 0 };
	//v3 = { 175, 300, 0 };
	//v4 = { 325, 300, 0 };
	//v5 = { 475, 300, 0 };

	//VertexBuffer[0].Pos = { 100, 100, 0 };   // top-left
	//VertexBuffer[1].Pos = { 250, 100, 0 };   // top-mid-left
	//VertexBuffer[2].Pos = { 400, 100, 0 };   // top-mid-right
	//VertexBuffer[3].Pos = { 175, 300, 0 };   // bottom-left
	//VertexBuffer[4].Pos = { 325, 300, 0 };   // bottom-mid
	//VertexBuffer[5].Pos = { 475, 300, 0 };   // bottom-right

	//IndexBuffer = {
	//	0, 3, 1,   // tri 0
	//	1, 3, 4,   // tri 1
	//	1, 4, 2,   // tri 2
	//	2, 4, 5    // tri 3
	//};

	VertexBuffer[0].Pos = { 0.00, 100.00, 0.00 };   // top pole
	VertexBuffer[1].Pos = { 70.71, 70.71, 0.00 };   // ring 1, seg 0
	VertexBuffer[2].Pos = { 35.36, 70.71, 61.24 };   // ring 1, seg 1
	VertexBuffer[3].Pos = { -35.36, 70.71, 61.24 };   // ring 1, seg 2
	VertexBuffer[4].Pos = { -70.71, 70.71, 0.00 };   // ring 1, seg 3
	VertexBuffer[5].Pos = { -35.36, 70.71, -61.24 };   // ring 1, seg 4
	VertexBuffer[6].Pos = { 35.36, 70.71, -61.24 };   // ring 1, seg 5
	VertexBuffer[7].Pos = { 100.00, 0.00, 0.00 };   // ring 2, seg 0
	VertexBuffer[8].Pos = { 50.00, 0.00, 86.60 };   // ring 2, seg 1
	VertexBuffer[9].Pos = { -50.00, 0.00, 86.60 };   // ring 2, seg 2
	VertexBuffer[10].Pos = { -100.00, 0.00, 0.00 };   // ring 2, seg 3
	VertexBuffer[11].Pos = { -50.00, 0.00, -86.60 };   // ring 2, seg 4
	VertexBuffer[12].Pos = { 50.00, 0.00, -86.60 };   // ring 2, seg 5
	VertexBuffer[13].Pos = { 70.71, -70.71, 0.00 };   // ring 3, seg 0
	VertexBuffer[14].Pos = { 35.36, -70.71, 61.24 };   // ring 3, seg 1
	VertexBuffer[15].Pos = { -35.36, -70.71, 61.24 };   // ring 3, seg 2
	VertexBuffer[16].Pos = { -70.71, -70.71, 0.00 };   // ring 3, seg 3
	VertexBuffer[17].Pos = { -35.36, -70.71, -61.24 };   // ring 3, seg 4
	VertexBuffer[18].Pos = { 35.36, -70.71, -61.24 };   // ring 3, seg 5
	VertexBuffer[19].Pos = { 0.00, -100.00, 0.00 };   // bottom pole

	IndexBuffer = {
		// top cap
		0, 2, 1,
		0, 3, 2,
		0, 4, 3,
		0, 5, 4,
		0, 6, 5,
		0, 1, 6,
		// band ring1 -> ring2
		1, 8, 7,
		1, 2, 8,
		2, 9, 8,
		2, 3, 9,
		3, 10, 9,
		3, 4, 10,
		4, 11, 10,
		4, 5, 11,
		5, 12, 11,
		5, 6, 12,
		6, 7, 12,
		6, 1, 7,
		// band ring2 -> ring3
		7, 14, 13,
		7, 8, 14,
		8, 15, 14,
		8, 9, 15,
		9, 16, 15,
		9, 10, 16,
		10, 17, 16,
		10, 11, 17,
		11, 18, 17,
		11, 12, 18,
		12, 13, 18,
		12, 7, 13,
		// bottom cap
		19, 13, 14,
		19, 14, 15,
		19, 15, 16,
		19, 16, 17,
		19, 17, 18,
		19, 18, 13,
	};

	camera.AT = VertexBuffer[0].Pos;
	camera.EYE = { 400, 100, 30 };
	camera.UP = { 170, 150, 20 };

	camera.n = normal(camera.AT - camera.EYE);
	camera.u = cross(camera.UP, camera.n);
	camera.v = cross(camera.n, camera.u);

	Vec3 Translation = { 500, 200, 120 };
	Vec3	   Angle = { 14, 14, 14 };
	Vec3	   Scale = { 1.4, 1.4, 1.4 };
	
	ModelTransform(VertexBuffer, Translation, Angle, Scale);

	Draw(VertexBuffer, IndexBuffer, Framebuffer);
	
	ViewTransform(VertexBuffer, camera); // without ProjectionTransform, this does nothing to object. besides, we don't even have a object yet.
	
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