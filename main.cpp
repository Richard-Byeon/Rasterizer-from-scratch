/*
							REAL-TIME-RENDERER(RASTERIZER) FROM SCRATCH 

			FILE: main.cpp (temporary)
	
			LAST UPDATE: 23RD, JUNE, 2026
			LAST MODIFIED FEATURE: WINDOW ALLOCATE
*/
struct Z {
	float z = 1.0f;
};

#include <SDL3/SDL.h>
#include "Math/Math.cpp"
#include "Framebuffer/Framebuffer.h"
#include "Bresenhem/Bresenhem.h"


int main(int argc, char* argv[])
{
	std::vector<Pixel>		Framebuffer(WIDTH * HEIGHT);
	std::vector<Z>			ZBuffer(WIDTH * HEIGHT);

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

	// SAMPLE POINTS
	/*
		V0 = (10, 50)
		V1 = (60, 10)
		V2 = (60, 90)

		V3 = (40, 30)
		V4 = (100, 60)
		V5 = (50, 100)
	*/
	Vec3 v0, v1, v2, 
		 v3, v4, v5;

	v0 = { 10, 50 };
	v1 = { 60, 10 };
	v2 = { 60, 90 };

	v3 = { 40, 30 };
	v4 = { 100, 60 };
	v5 = { 50, 100 };
	int x0, x1, x2, y0, y1, y2;

	x0 = v0.v[0];	 x1 = v1.v[0];		x2 = v2.v[0];
	y0 = v0.v[1];	 y1 = v1.v[1];		y2 = v2.v[1];

	Bresenhem(x0, y0, x1, y1, Framebuffer);
	Bresenhem(x1, y1, x2, y2, Framebuffer);
	Bresenhem(x2, y2, x0, y0, Framebuffer);

	x0 = v3.v[0];	 x1 = v4.v[0];		x2 = v5.v[0];
	y0 = v3.v[1];	 y1 = v4.v[1];		y2 = v5.v[1];
	Bresenhem(x0, y0, x1, y1, Framebuffer);
	Bresenhem(x1, y1, x2, y2, Framebuffer);
	Bresenhem(x2, y2, x0, y0, Framebuffer);

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

		SDL_UpdateTexture(texture, nullptr, Framebuffer.data(), WIDTH * sizeof(Pixel));

		SDL_RenderTexture(renderer, texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


	return 0;
}