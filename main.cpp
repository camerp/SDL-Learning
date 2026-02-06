
#include <SDL3/SDL.h>
#include "application.h"



int main(int argc, char* argv[])
{
	SDL_Log("SIMD Level: %s", SIMD::SelectHighestLevel());
	SDLApp* mainApp = new SDLApp("SDL Tutorial");

	/************Parse Command Line Arguments***************/
	for (int i = 1; i < argc; i++)
	{
		char* arg = argv[i];
		if (strncmp("w:", arg, 2) == 0)
		{
			mainApp->width = SDL_atoi(arg + 2);
		}
		if (strncmp("h:", arg, 2) == 0)
		{
			mainApp->height = SDL_atoi(arg + 2);
		}
		if (strncmp("d:", arg, 2) == 0)
		{
			mainApp->driver = &arg[2];
		}
		if (strncmp("r:", arg, 2) == 0)
		{
			mainApp->windowScalingBehavior = (SDL_RendererLogicalPresentation)SDL_atoi(arg + 2);
		}
		
	}

	mainApp->CreateWindow();
	
	mainApp->AppLoop();
	
	delete mainApp;

	return 0;
}