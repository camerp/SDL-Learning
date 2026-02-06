#pragma once
#include <string>
#include <SDL3\SDL.h>
#include "input.h"
#include "shapes.h"
#include "particles.h"
#include "Utility.h"


union foo
{
	void* a;
	void (*b)();
};

#define MAX_SHAPES 128
#define MAX_PARTICLE_SYSTEMS 10

static struct SDLApp
{
	//Application Logic
	bool running;

	//Window and Renderer
	SDL_Window* window;
	SDL_Renderer* _r;
	
	//Display/Window Settings
	char* driver = nullptr;
	int width = 800, height = 600;
	Uint8 clearColor[4];
	const char* title;
	Uint16 targetFrameRate = 100;
	bool lockFrameRate = false;
	SDL_RendererLogicalPresentation windowScalingBehavior = SDL_LOGICAL_PRESENTATION_DISABLED;

	//Input System
	InputSystem _inputSystem;

	//Shape Buffer systems
	Shape* shapes = (Shape*)calloc(MAX_SHAPES, sizeof(Shape));
	Shape* currentShape = shapes;
	Shape* lastShape = shapes + (MAX_SHAPES - 1);
	Shape ActiveShape;
	ShapeType shapeTool = LINE;
	Uint32 shapeColor = 0xffffffff;
	bool drawActiveShape = false;
	bool shapesFull = false;

	//Particle System
	ParticleSystem* particles = (ParticleSystem*)calloc(MAX_PARTICLE_SYSTEMS, sizeof(ParticleSystem));
	Uint8 currentSystem = 0;

	//Scratch Members
	SDL_Surface* StampSurface;

#pragma region Constructor/Destructor
	SDLApp(const char* Title = "App", int Width = 800, int Height = 600)
	{
		title = Title;
		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
		{
			SDL_Log("Failed to initialize:%s", SDL_GetError());
		}
		running = true;
		window = nullptr;
		InputBinding();
		_inputSystem.app = this;
		_inputSystem.running = &running;
		lockFrameRate = true;
		StampSurface = SDL_LoadBMP("./pirate_cat.bmp");
	}
	~SDLApp()
	{
		SDL_Quit();
		free(shapes);
		free(particles);
	}
#pragma endregion

	//Manages the shape buffer, should probably be a separate library at some point.
	void NextShape()
	{
		if (ActiveShape.type == STAMP)
			currentShape->tex = ActiveShape.tex;
		else
			currentShape->color = ActiveShape.color;
		currentShape->data = ActiveShape.data;
		currentShape->type = ActiveShape.type;
		currentShape->draw = ActiveShape.draw;
		if (currentShape == lastShape)
		{
			currentShape = shapes;
			shapesFull = true;
		}
		else
		{
			currentShape++;
		}
	}

	

#pragma region Input Handling

	//Actions to be mapped to inputs
	static void SetStartPoint(SDL_Event* e, SDLApp* app)
	{
		Shape* activeShape = &app->ActiveShape;
		if (e->button.down)
		{
			app->drawActiveShape = true;
			activeShape->draw = drawShape;
			activeShape->type = app->shapeTool;
			activeShape->data.x = e->button.x;
			activeShape->data.y = e->button.y;
			activeShape->data.z = e->button.x;
			activeShape->data.w = e->button.y;
			if (activeShape->type == STAMP)
			{
				activeShape->tex = SDL_CreateTextureFromSurface(app->_r, app->StampSurface);
			}
			else
			{
				activeShape->color.Hex = app->shapeColor;
			}
		}
		else if (!e->button.down)
		{
			app->drawActiveShape = false;
			app->NextShape();
		}
	}

	static void MouseWheel(SDL_Event* e, SDLApp* app)
	{
		Shape* activeShape = &app->ActiveShape;
		Uint8 currentTool = app->shapeTool;
		if (e->wheel.y > 0)
			if (currentTool < SHAPE_TYPE_COUNT - 1)
			{
				currentTool++;
			}
			else
			{
				currentTool = 0;
			}
		else if (e->wheel.y < 0)
		{
			if (currentTool > 0)
			{
				currentTool--;
			}
			else
			{
				currentTool = SHAPE_TYPE_COUNT - 1;
			}
		}
		app->shapeTool = (ShapeType)currentTool;
		activeShape->type = app->shapeTool;
		SDL_Log("Current Tool:%s", ShapeTypes[currentTool]);
	}

	static void TrackMouse(SDL_Event* e, SDLApp* app)
	{
		app->ActiveShape.data.z = e->motion.x;
		app->ActiveShape.data.w = e->motion.y;
	}

	static void ChangeClearColor(SDL_Event* e, SDLApp* app)
	{
		if (e->button.down)
		{
			*((Uint32*)app->clearColor) = SDL_rand_bits();
			app->clearColor[3] = 255;
			SDL_Log("\033[0;31mR:%03d \033[0;32mG:%03d \033[0;34mB:%03d\033[0m", app->clearColor[0], app->clearColor[1], app->clearColor[2]);
		}
			
	}

	static void Forward(SDL_Event* e, SDLApp* app)
	{
		if (e->key.down)
			SDL_Log("Forward:%c", e->key.key);
	}

	static void SpawnParticleSystem(SDL_Event* e, SDLApp* app)
	{
		if (e->button.down)
		{
			ParticleSystem* ps = &app->particles[app->currentSystem];
			setupParticleSystem(ps);
			ps->origin.x = e->button.x;
			ps->origin.y = e->button.y;
			ps->color.Hex = SDL_rand_bits();
			for (int i = 0; i < MAX_PARTICLES; i++)
			{
				QuickSpawnParticle(ps);
			}
			if (app->currentSystem < MAX_PARTICLE_SYSTEMS-1)
			{
				app->currentSystem++;
			}
			else
			{
				app->currentSystem = 0;
			}
		}
		return;
	}

	static void ToggleFullscreenBorderless(SDL_Event* e, SDLApp* app)
	{
		if (e->key.down)
		{
			if ((SDL_GetWindowFlags(app->window) & SDL_WINDOW_FULLSCREEN) != SDL_WINDOW_FULLSCREEN)
			{
				SDL_SetWindowFullscreen(app->window, true);
			}
			else
			{
				SDL_SetWindowFullscreen(app->window, false);
			}
		}
		return;
	}

	//Binding Actions to the input system
	void InputBinding()
	{
		ActionMap am;
		am.action = Forward;
		_inputSystem.MapKeyAction(SDL_SCANCODE_W, &am);
		am.action = ToggleFullscreenBorderless;
		_inputSystem.MapKeyAction(SDL_SCANCODE_F2, &am);
		am.action = ChangeClearColor;
		_inputSystem.MapMouseAction(3, &am);
		am.action = SetStartPoint;
		_inputSystem.MapMouseAction(1, &am);
		am.action = SpawnParticleSystem;
		_inputSystem.MapMouseAction(2, &am);
		_inputSystem.mouseMove.action = TrackMouse;
		_inputSystem.mouseWheel.action = MouseWheel;
	}
#pragma endregion

#pragma region Window and Render Setup
	bool CreateWindow()
	{
		return CreateWindow(width, height);
	}

	bool CreateWindow(int Width, int Height)
	{
		SDL_Log("Available renderer drivers:");
		for (int i = 0; i < SDL_GetNumRenderDrivers(); i++)
		{
			SDL_Log("%d. %s", i + 1, SDL_GetRenderDriver(i));
		}
		window = SDL_CreateWindow(title, Width, Height, SDL_WINDOW_RESIZABLE);
		_r = SDL_CreateRenderer(window, driver);
		if (_r == nullptr)
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Failed To Create Renderer with Driver:%s\nAttempting with Default", driver);
			_r = SDL_CreateRenderer(window, NULL);
			if (_r == nullptr)
			{
				SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "Failed To Create Renderer with Default Driver");
			}
		}
		SDL_Log("Selected Driver:%s", SDL_GetRendererName(_r));

	
		width = Width;
		height = Height;

		SDL_SetRenderLogicalPresentation(_r, width, height, windowScalingBehavior);

		return true;
	}
#pragma endregion


	void tick()
	{
		_inputSystem.ProcessInput();
		Render(window);
	}

	void AppLoop()
	{
		Uint64 currentTick, deltaTime, lastTime, fps, budget;
		std::string temp, label;
		lastTime = 0;
		fps = 0;
		budget = 1000000000 / targetFrameRate;
		while (running)
		{
			SDL_GetWindowSize(window, &width, &height);
			label = " " + std::to_string(width) + "x" + std::to_string(height) + " FPS:";
			currentTick = SDL_GetTicksNS();
			tick();
			deltaTime = SDL_GetTicksNS() - currentTick;
			//Locked Framerate Delay
			lockFrameRate? SDL_DelayPrecise(budget - deltaTime) : SDL_DelayPrecise(0);

			//FPS Calculation
			fps++;
			if (currentTick > lastTime + 1000000000)
			{
				lastTime = currentTick;
				temp = title + label + std::to_string(fps);
				SDL_SetWindowTitle(window, temp.c_str());
				fps = 0;
			}

		}
	}

	void Render(SDL_Window* target)
	{
		SDL_SetRenderDrawColor(_r, clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		SDL_RenderClear(_r);
		Uint64 shapeCount = 0;
		if(shapesFull)
		{
			shapeCount = MAX_SHAPES;
		}
		else
		{
			shapeCount = currentShape - shapes;
		}
		
		DrawShapes(_r, shapes, shapeCount);


		if (drawActiveShape)
			ActiveShape.draw(_r, &ActiveShape);
		for (int i = 0; i < MAX_PARTICLE_SYSTEMS; i++)
		{
			DrawParticleSystem(_r, &particles[i]);

			double time = ((double)UpdateParticles(&particles[i]))/1000000.0;
			std::string temp = "Update MS:" +  std::to_string(time);
			SDL_RenderDebugText(_r, 5.0f, 10.0f*i, temp.c_str());
		}

		SDL_RenderPresent(_r);
		return;
	}
};





