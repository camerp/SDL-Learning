#pragma once

union Color32
{
	Uint8 Array[4];
	Uint32 Hex;
	struct { Uint8 r, g, b, a; };
};

union v2
{
	float array[2];
	struct { float x, y; };
	struct { float u, v; };
};