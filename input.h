#pragma once

#include <SDL3\SDL.h>


#define MAX_BUTTONS 64
#define MAX_EVENTS 255

struct SDLApp;

typedef void (*Action)(SDL_Event*, SDLApp*);
struct ActionMap
{
	Action action = NULL;
};

struct InputSystem
{
	ActionMap keyMap[SDL_SCANCODE_COUNT];
	ActionMap mouseMap[MAX_BUTTONS];
	ActionMap padMap[MAX_BUTTONS];
	ActionMap mouseMove;
	ActionMap padMove;
	ActionMap mouseWheel;
	SDL_Event eventQ[MAX_EVENTS];

	SDLApp* app = NULL;
	bool* running = NULL;

	ActionMap* AM;


	void MapKeyAction(SDL_Scancode key, const ActionMap* a)
	{
		if (keyMap[key].action != NULL)
		{
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Overwrite Key Action:%d", key);
		}
		keyMap[key].action = a->action;
	}

	void MapMouseAction(Uint8 button, const ActionMap* a)
	{
		if (mouseMap[button].action != NULL)
		{
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Overrite Mouse Button:%d", button);
		}
		mouseMap[button].action = a->action;
	}

	void MapPadAction(Uint8 button, const ActionMap* a)
	{
		if (padMap[button].action != NULL)
		{
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Overrite Gamepad Button:%d", button);
		}
		padMap[button].action = a->action;
	}
	void ProcessInput()
	{
		SDL_PumpEvents();
		int count = SDL_PeepEvents(eventQ, MAX_EVENTS, SDL_GETEVENT, SDL_EVENT_FIRST, SDL_EVENT_LAST);
		AM = NULL;
		for (int i = 0; i < count; i++)
		{
			switch (eventQ[i].type)
			{
			case SDL_EVENT_KEY_DOWN: case SDL_EVENT_KEY_UP:
				AM = &keyMap[eventQ[i].key.scancode];
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN: case SDL_EVENT_MOUSE_BUTTON_UP:
				AM = &mouseMap[eventQ[i].button.button];
				break;
			case SDL_EVENT_GAMEPAD_BUTTON_DOWN: case SDL_EVENT_GAMEPAD_BUTTON_UP:
				AM = &padMap[eventQ[i].gbutton.button];
				break;
			case SDL_EVENT_MOUSE_MOTION:
				AM = &mouseMove;
				break;
			case SDL_EVENT_GAMEPAD_AXIS_MOTION:
				AM = &padMove;
				break;
			case SDL_EVENT_MOUSE_WHEEL:
				AM = &mouseWheel;
				break;
			case SDL_EVENT_QUIT:
				if (running != NULL)
					*running = false;
				break;
			}
			if (AM != NULL) //This means the event does not map to a key, button, or mouse press
			{
				if (AM->action != NULL)
				{
					AM->action(&eventQ[i], app);
				}
			}

		}

	}

};






