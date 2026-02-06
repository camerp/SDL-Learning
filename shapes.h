#pragma once

#include <SDL3\SDL.h>
#include "Utility.h"

union Shape;
static void drawShape(SDL_Renderer* r, Shape* s);

#define SHAPE_TYPE_COUNT 6
enum ShapeType : Uint8
{
	LINE = 0, POINT = 1, RECT = 2, RECTF = 3, CIRCLE = 4, STAMP = 5
};

const char* ShapeTypes[] = { "Line", "Point", "Rectangle", "Rectangle Fill", "Circle", "Stamp"};


union ShapeData
{
	float Array[4];
	struct { float x, y, z, w; };
	struct { float xs, ys, xe, ye; };
};




union Shape
{
	struct
	{	
		enum ShapeType type;
		ShapeData data;
		void (*draw)(SDL_Renderer*, Shape*);
		Color32 color;
	};

	struct
	{
		enum ShapeType type;
		ShapeData data;
		void (*draw)(SDL_Renderer*, Shape*);
		SDL_Texture* tex;
	};
};

static void drawShape(SDL_Renderer* r, Shape* s)
{
	SDL_SetRenderDrawColor(r, s->color.r, s->color.g, s->color.b, s->color.a);
	ShapeData data = s->data;
	SDL_FRect trect = { data.x, data.y, data.z - data.x, data.w - data.y };
	SDL_Texture* tex = s->tex;
	float radius, angleStep, xs, ys, xe, ye;
	int segments;
	switch (s->type)
	{
		case LINE:
			SDL_RenderLine(r, data.x, data.y, data.z, data.w);
			break;
		case POINT:
			SDL_RenderPoint(r, data.x, data.y);
			break;
		case RECT:
			SDL_RenderRect(r, &trect);
			break;
		case RECTF:
			SDL_RenderFillRect(r, &trect);
			break;
		case STAMP:
			SDL_RenderTexture(r, tex, NULL, &trect);
			break;
		case CIRCLE:
			radius = sqrtf(powf(data.x - data.z, 2) + pow(data.y - data.w, 2));
			segments = (int)roundf(radius) + 7;
			angleStep = (SDL_PI_F * 2) / segments;
			xs = data.x + cosf(0 * angleStep) * radius;
			ys = data.y + sinf(0 * angleStep) * radius;
			xe, ye;
			for (int i = 1; i <= segments; i++)
			{
				xe = data.x + cosf(i * angleStep) * radius;
				ye = data.y + sinf(i * angleStep) * radius;
				SDL_RenderLine(r, xs, ys, xe, ye);
				xs = xe;
				ys = ye;
			}
			break;
	}
}

static void DrawShapes(SDL_Renderer* r, Shape* s, Uint64 count)
{
	Shape* current = s;
	for (int i = 0; i < count; i++)
	{
		s->draw(r, s);
		s++;
	}
}