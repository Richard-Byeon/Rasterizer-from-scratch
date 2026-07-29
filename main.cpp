/*
							REAL-TIME-RENDERER(RASTERIZER) FROM SCRATCH 

			FILE: main.cpp (temporary name)
	
			LAST UPDATE: 29th, JULY, 2026
			LAST MODIFIED FEATURE: Refactored ModelTrasnform, Texturing, Reading a file (.png)
			*/


#include				 <algorithm>
#include				 <SDL3/SDL.h>
#include				 <iostream>
#include				"Math/Math.h"
#include	  "Bresenhem/Bresenhem.h"
#include	"Rasterizer/Rasterizer.h"
#include			"Camera/Camera.h"
#include	 "Vertex/VertexShading.h"
#include			"UV/Texturing.h"
#include			"Shading/Phong.h"

// ASPECT = HEIGHT(600) / WIDTH(800) = 4/3

constexpr float ASPECT = (float)WIDTH / (float)HEIGHT;

float fovy = 60 * (PI / 180.0f);
float fovx = 2 * atan(ASPECT * tan(fovy / 2));

void GenerateUVSphere(int stacks, int slices, float radius,
	std::vector<Vertex>& vbuf, std::vector<uint32_t>& ibuf)
{
	// Difference between AI made version and hand-made version: vertex/index allocating, UV coordinate
	vbuf.clear();
	ibuf.clear();
	vbuf.reserve((stacks + 1) * (slices + 1));
	ibuf.reserve(3 * 2 * stacks * slices);

	///////////////////////////
	//	 Vertices Allocate	 //
	///////////////////////////
	Vertex vertex;
	int ring, segment;
	

	for (ring = 0; ring <= stacks; ring++)
	{
		// r == 0 && r == stacks -> pole. (Has the same amount of vertex as slices)
		float phi = PI * (ring / float(stacks));
		// calculate which "floor" this circle will be located in.
		float ny = cosf(phi);
		// calculates radius of this floor's circle
		float Radius = sinf(phi);

		for (segment = 0; segment <= slices; segment++)
		{
			float theta = 2.0f * PI * (segment / (float)slices);
			// normalized x coordinate
			float nx = Radius * cosf(theta);
			// normalized z coordinate
			float nz = Radius * sinf(theta);
			vertex.Pos = { radius * nx, radius * ny, radius * nz };
			vertex.Normal = { nx, ny, nz };
			vertex.UV = { segment / (float)slices, ring / (float)stacks };

			auto ch = [](float n) { return (uint8_t)(128.0f + 100.0f * n + 0.5f); }; // normal color lambda func. Made by AI.
			float u = vertex.UV.v[0], v = vertex.UV.v[1];
			vertex.Color = { (uint8_t)(u * 255), (uint8_t)(v * 255), 0,  255};
			vbuf.push_back(vertex);
		}
	}

	/*int vertexCount = vbuf.size();

	for (int i = 0; i < vertexCount ; i++)
	{
		std::cout << "Vertex.pos" << i << ": " << vbuf[i].Pos.v[0] << " " << vbuf[i].Pos.v[1] << " " << vbuf[i].Pos.v[2] << "\n";
	}*/
	//////////////////////////////////////////////

	///////////////////////////////////////
	//	Idx Allocate & Face Allocate	//
	//////////////////////////////////////

	// We'll be using quad since the vertices are allocated as a grid and then slice that quad into two trinagle.
	//  

	for (ring = 0; ring < stacks; ring++)
	{
		for (segment = 0; segment < slices; segment++)
		{
			/*
									 seg	 seg + 1
									  |			|
						ring	 ---- a ------- b   
									  |			|
						ring + 1 ---- c ------- d
			*/

			uint32_t a, b, c, d;

			a = ring * (slices + 1) + segment;
			b = a + 1;
			c = a + (slices + 1);
			d = c + 1;
			ibuf.push_back(a); ibuf.push_back(c); ibuf.push_back(d);
			ibuf.push_back(a); ibuf.push_back(d); ibuf.push_back(b);

		}
	}

	/*int idxcount = ibuf.size();
	for (int i = 0; i < idxcount / 3; i++)
	{
		std::cout << "face" << i << ": " << ibuf[3 * i] << " " << ibuf[3*i + 1] << " " << ibuf[3*i + 2]<< "\n";
	}*/
	/*for (int ring = 0; ring <= stacks; ring++)
	{
		const Vertex& f = vbuf[ring * (slices + 1) + 0];
		const Vertex& l = vbuf[ring * (slices + 1) + slices];
		std::cout << ring
			<< " dPos=" << (f.Pos.v[0] - l.Pos.v[0]) << ","
			<< (f.Pos.v[1] - l.Pos.v[1]) << ","
			<< (f.Pos.v[2] - l.Pos.v[2])
			<< " u=" << f.UV.v[0] << "/" << l.UV.v[0] << "\n";
	}*/

}

// written by AI. This function is to check if there is any distortion on the textured object. (visually)
void GenerateQuad(float halfSize, float y,
	std::vector<Vertex>& vbuf, std::vector<uint32_t>& ibuf)
{
	vbuf.clear();
	ibuf.clear();

	Vertex v;
	v.Normal = { 0.0f, 1.0f, 0.0f };
	v.Color = { 255, 255, 255, 255 };

	//  z = +halfSize (far)     0 --- 1
	//                          |     |
	//  z = -halfSize (near)    2 --- 3
	v.Pos = { -halfSize, y,  halfSize }; v.UV = { 0.0f, 1.0f }; vbuf.push_back(v);
	v.Pos = { halfSize, y,  halfSize }; v.UV = { 1.0f, 1.0f }; vbuf.push_back(v);
	v.Pos = { -halfSize, y, -halfSize }; v.UV = { 0.0f, 0.0f }; vbuf.push_back(v);
	v.Pos = { halfSize, y, -halfSize }; v.UV = { 1.0f, 0.0f }; vbuf.push_back(v);

	ibuf = { 0, 1, 2,   2, 1, 3 };	
}

int main(int argc, char* argv[])
{
	std::vector<Color>			Framebuffer(WIDTH * HEIGHT);
	std::vector<float>			ZBuffer(WIDTH * HEIGHT, 1.0f); // init Zbuffer as 1.0f (far)
	std::vector<Vertex>			VertexBuffer;
	std::vector<uint32_t>		IndexBuffer;

	// Absolute root for now, later will be changed to relative root.
	Texture Tex = LoadTexture("C:/C++/Project/RFS/Assets/test_nonsquare.png");
	
	if (Tex.Width == 0) return 1;

	GenerateUVSphere(12, 16, 70.0f, VertexBuffer, IndexBuffer);

	//GenerateQuad(100.0f, 10.0f, VertexBuffer, IndexBuffer);

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
	Vec3	   Angle = { 0.0f, 0.0f ,0.0f };

	const float RotationSpeedDegPerSec = 45.0f;

	camera.AT = Translation;
	camera.EYE = { 300, 10, 30 };
	camera.UP = { 0, 1, 0 };

	camera.n = normal(camera.AT - camera.EYE);
	camera.u = normal(cross(camera.UP, camera.n));
	camera.v = normal(cross(camera.n, camera.u));

	Uint64 startTicks = SDL_GetTicks();

	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	bool running = true;
	SDL_Event event;
	
    while (running)
	{
        while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				running = false;

			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				// Added on 29th July
				if (event.key.key == SDLK_UP)    pitch -= 15.0f;
				if (event.key.key == SDLK_LEFT)  yaw -= 15.0f;
				if (event.key.key == SDLK_DOWN)  pitch += 15.0f;
				if (event.key.key == SDLK_RIGHT) yaw += 15.0f;
				
			}

        }
		
		/*Uint64 nowTicks = SDL_GetTicks();
		float elapsedSec = (nowTicks - startTicks) / 1000.0f;*/
		Angle = { pitch, yaw, 0.0f };

		std::fill(Framebuffer.begin(), Framebuffer.end(), Color{});
		std::fill(ZBuffer.begin(), ZBuffer.end(), 1.0f);

		VertexBuffer = VertexBufferOriginal;

		// MODELTRANSFORM REFACTOR: PASS THE HOMOGENOUS MATRIX INSTEAD OF PARTS OF THESE.
		Mat4 TransformM = HomogenousMatrix(Translation, Angle, Scale);
		Mat4 NormalM = NormalMatrix(TransformM);

		ModelTransform(VertexBuffer, TransformM, NormalM);
		ViewTransform(VertexBuffer, camera);
		PerspectiveProjection(VertexBuffer, fovy, ASPECT);
		ViewPortTransfrom(VertexBuffer, WIDTH, HEIGHT);
		Draw(VertexBuffer, IndexBuffer, Framebuffer, ZBuffer, Tex);

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
