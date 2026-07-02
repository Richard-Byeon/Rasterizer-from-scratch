/*
							REAL-TIME-RENDERER(RASTERIZER) FROM SCRATCH 

			FILE: main.cpp (temporary)
	
			LAST UPDATE: 2nd, JULY, 2026
			LAST MODIFIED FEATURE: VERTEX INDEXING (IA) && VERTEX SHADING
*/


#include				 <SDL3/SDL.h>
#include				"Math/Math.h"
#include	  "Bresenhem/Bresenhem.h"
#include	"Barycentric/Barycentric.h"
		
#include	"Vertex/VertexShading.h"



int main(int argc, char* argv[])
{
	std::vector<Color>			Framebuffer(WIDTH * HEIGHT);
	std::vector<float>			ZBuffer(WIDTH * HEIGHT);
	std::vector<Vertex>			VertexBuffer(6);
	std::vector<uint32_t>		IndexBuffer(3 * 2);

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




	// VERTICES
	Vec3 v0, v1, v2, 
		 v3, v4, v5;

	v0 = { 100, 100, 0 };
	v1 = { 250, 100, 0 };
	v2 = { 400, 100, 0 };
	v3 = { 175, 300, 0 };
	v4 = { 325, 300, 0 };
	v5 = { 475, 300, 0 };

	VertexBuffer[0].Pos = { 100, 100, 0 };   // top-left
	VertexBuffer[1].Pos = { 250, 100, 0 };   // top-mid-left
	VertexBuffer[2].Pos = { 400, 100, 0 };   // top-mid-right
	VertexBuffer[3].Pos = { 175, 300, 0 };   // bottom-left
	VertexBuffer[4].Pos = { 325, 300, 0 };   // bottom-mid
	VertexBuffer[5].Pos = { 475, 300, 0 };   // bottom-right

	IndexBuffer = {
		0, 3, 1,   // tri 0
		1, 3, 4,   // tri 1
		1, 4, 2,   // tri 2
		2, 4, 5    // tri 3
	};

	Vec3 Translation = { 100, 300, 0 };
	Vec3	   angle = { 10, 10, 10 };
	Vec3	   scale = { 0.5, 0.5, 0.5 };

	Draw(VertexBuffer, IndexBuffer, Framebuffer);

	ModelTransform(VertexBuffer, Translation, angle, scale);

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