#pragma once

#include <SDL3\SDL.h>


namespace SIMD
{ 
	Uint8 OpStep = 1;
	namespace AVX512
	{
		Uint8 add_floats(float* a, float* b, float* c)
		{
			__m512 aTarg = _mm512_load_ps(a);
			__m512 bTarg = _mm512_load_ps(b);
			_mm512_storeu_ps(c, _mm512_add_ps(aTarg, bTarg));
			return 16;
		}

		Uint8 add_float(float* a, float b, float* c)
		{
			__m512 aTarg = _mm512_load_ps(a);
			__m512 bTarg = _mm512_set1_ps(b);
			_mm512_storeu_ps(c, _mm512_add_ps(aTarg, bTarg));
			return 16;
		}

		Uint8 mul_floats(float* a, float* b, float* c)
		{
			__m512 aTarg = _mm512_load_ps(a);
			__m512 bTarg = _mm512_load_ps(b);
			_mm512_storeu_ps(c, _mm512_mul_ps(aTarg, bTarg));
			return 16;
		}

		Uint8 mul_float(float* a, float b, float* c)
		{
			__m512 aTarg = _mm512_load_ps(a);
			__m512 bTarg = _mm512_set1_ps(b);
			_mm512_storeu_ps(c, _mm512_mul_ps(aTarg, bTarg));
			return 16;
		}

		Uint8 particleUpdate(float* pts, float* vel, float drag, float grav)
		{
			__m512 Points = _mm512_load_ps(pts);
			__m512 Velocity = _mm512_load_ps(vel);
			_mm512_store_ps(pts, _mm512_add_ps(Points, Velocity));

			__m512 Drag = _mm512_set1_ps(drag);
			__m512 Gravity = _mm512_setr_ps(0.0f, grav, 0.0f, grav, 0.0f, grav, 0.0f, grav, 0.0f, grav, 0.0f, grav, 0.0f, grav, 0.0f, grav);
			_mm512_store_ps(vel, _mm512_fmadd_ps(Velocity, Drag, Gravity));

			return 16;
		}

	}
	namespace AVX2
	{
		Uint8 add_floats(float* a, float* b, float* c)
		{
			__m256 aTarg = _mm256_load_ps(a);
			__m256 bTarg = _mm256_load_ps(b);
			_mm256_storeu_ps(c, _mm256_add_ps(aTarg, bTarg));
			return 8;
		}
		Uint8 add_float(float* a, float b, float* c)
		{
			__m256 aTarg = _mm256_load_ps(a);
			__m256 bTarg = _mm256_set1_ps(b);
			_mm256_storeu_ps(c, _mm256_add_ps(aTarg, bTarg));
			return 8;
		}

		Uint8 mul_floats(float* a, float* b, float* c)
		{
			__m256 aTarg = _mm256_load_ps(a);
			__m256 bTarg = _mm256_load_ps(b);
			_mm256_storeu_ps(c, _mm256_mul_ps(aTarg, bTarg));
			return 8;
		}

		Uint8 mul_float(float* a, float b, float* c)
		{
			__m256 aTarg = _mm256_load_ps(a);
			__m256 bTarg = _mm256_set1_ps(b);
			_mm256_storeu_ps(c, _mm256_mul_ps(aTarg, bTarg));
			return 8;
		}

		Uint8 particleUpdate(float* pts, float* vel, float drag, float grav)
		{
			__m256 Points = _mm256_load_ps(pts);
			__m256 Velocity = _mm256_load_ps(vel);
			_mm256_store_ps(pts, _mm256_add_ps(Points, Velocity));

			__m256 Drag = _mm256_set1_ps(drag);
			__m256 Gravity = _mm256_setr_ps(0.0f, grav, 0.0f, grav, 0.0f, grav, 0.0f, grav);
			_mm256_store_ps(vel, _mm256_fmadd_ps(Velocity, Drag, Gravity));

			return 8;
		}
	}
	namespace SSE
	{
		Uint8 add_floats(float* a, float* b, float* c)
		{
			__m128 aTarg = _mm_load_ps(a);
			__m128 bTarg = _mm_load_ps(b);
			_mm_storeu_ps(c, _mm_add_ps(aTarg, bTarg));
			return 4;
		}
		Uint8 add_float(float* a, float b, float* c)
		{
			__m128 aTarg = _mm_load_ps(a);
			__m128 bTarg = _mm_set1_ps(b);
			_mm_storeu_ps(c, _mm_add_ps(aTarg, bTarg));
			return 4;
		}

		Uint8 mul_floats(float* a, float* b, float* c)
		{
			__m128 aTarg = _mm_load_ps(a);
			__m128 bTarg = _mm_load_ps(b);
			_mm_storeu_ps(c, _mm_mul_ps(aTarg, bTarg));
			return 4;
		}

		Uint8 mul_float(float* a, float b, float* c)
		{
			__m128 aTarg = _mm_load_ps(a);
			__m128 bTarg = _mm_set1_ps(b);
			_mm_storeu_ps(c, _mm_mul_ps(aTarg, bTarg));
			return 4;
		}
		Uint8 add_v2fs(float* a, float* b, float* c)
		{
			float bx = *b;
			float by = *(b + 1);
			__m128 aTarg = _mm_load_ps(a);
			__m128 bTarg = _mm_setr_ps(bx, by, bx, by);
			_mm_storeu_ps(c, _mm_add_ps(aTarg, bTarg));
			return 4;
		}


		Uint8 particleUpdate(float* pts, float* vel, float drag, float grav)
		{
			__m128 Points = _mm_load_ps(pts);
			__m128 Velocity = _mm_load_ps(vel);
			_mm_store_ps(pts, _mm_add_ps(Points, Velocity));

			__m128 Drag = _mm_set1_ps(drag);
			__m128 Gravity = _mm_setr_ps(0.0f, grav, 0.0f, grav);
			_mm_store_ps(vel, _mm_fmadd_ps(Velocity, Drag, Gravity));
			
			return 4;
		}

	}
	namespace None
	{
		Uint8 add_floats(float* a, float* b, float* c)
		{
			*c = *a + *b;
			return 1;
		}
		Uint8 add_float(float* a, float b, float* c)
		{
			*c = *a + b;
			return 1;
		}
		Uint8 mul_floats(float* a, float* b, float* c)
		{
			*c = *a * *b;
			return 1;
		}
		Uint8 mul_float(float* a, float b, float* c)
		{
			*c = *a * b;
			return 1;
		}

		Uint8 particleUpdate(float* pts, float* vel, float drag, float grav)
		{
			pts[0] += vel[0];
			pts[1] += vel[1];
			vel[0] *= drag;
			vel[1] = fmaf(vel[1], drag, grav);
			return 2;
		}
	}


	const char* SIMDLevels[] = { "None", "SSE", "AVX2", "AVX512" };

	Uint8(*add_floats)(float*, float*, float*) = None::add_floats;
	Uint8(*add_float)(float*, float, float*) = None::add_float;
	Uint8(*mul_floats)(float*, float*, float*) = None::mul_floats;
	Uint8(*mul_float)(float*, float, float*) = None::mul_float;
	Uint8(*particleUpdate)(float*, float*, float, float) = None::particleUpdate;
	
	const char* SetAVX512()
	{
		add_floats = AVX512::add_floats;
		add_float = AVX512::add_float;
		mul_floats = AVX512::mul_floats;
		mul_float = AVX512::mul_float;
		OpStep = 16;
		return SIMDLevels[3];
	}

	const char* SetAVX2()
	{
		add_floats = AVX2::add_floats;
		add_float = AVX2::add_float;
		mul_floats = AVX2::mul_floats;
		mul_float = AVX2::mul_float;
		OpStep = 8;
		return SIMDLevels[2];
	}

	const char* SetSSE()
	{
		add_floats = SSE::add_floats;
		add_float = SSE::add_float;
		mul_floats = SSE::mul_floats;
		mul_float = SSE::mul_float;
		OpStep = 4;
		return SIMDLevels[1];
	}

	const char* SetNone()
	{
		add_floats = None::add_floats;
		add_float = None::add_float;
		mul_floats = None::mul_floats;
		mul_float = None::mul_float;
		OpStep = 1;
		return SIMDLevels[0];
	}
	
	
	const char* SelectHighestLevel()
	{
		if (SDL_HasAVX512F)
			return SetAVX512();
		else if (SDL_HasAVX2)
			return SetAVX2();
		else if (SDL_HasSSE)
			return SetSSE();
		return SetNone();

	}




}
