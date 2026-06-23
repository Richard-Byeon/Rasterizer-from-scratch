/*
							REAL-TIME-RENDERER(RASTERIZER) FROM SCRATCH 

			FILE: main.cpp (temporary)
	
			LAST UPDATE: 23RD, JUNE, 2026
			LAST MODIFIED FEATURE: WINDOW ALLOCATE
*/

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

#include <SDL3/SDL.h>
#include "Math/Math.cpp"
#include "Framebuffer.h"

int main(int argc, char* argv[])
{
	std::vector<Pixel> framebuffer(WIDTH * HEIGHT);

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
	
	// Since my framebuffer uses RGBA, and type of those RGBA is uint_8, we set the texture pixel format to RGBA8888.
	SDL_Texture* texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH, HEIGHT
	);

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

		SDL_UpdateTexture(texture, nullptr, framebuffer.data(), WIDTH * sizeof(Pixel));

		SDL_RenderTexture(renderer, texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);
        

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


	return 0;
}