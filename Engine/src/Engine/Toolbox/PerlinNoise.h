#pragma once

#include "engine/Toolbox/Random.h"

class PerlinNoise
{
public:
	PerlinNoise(int seed, int octaves, float amplitude, float roughness);

	PerlinNoise(int octaves, float amplitude, float roughness);

	PerlinNoise() {}

	float getPerlinNoise(int x, int y);
	float getSmoothNoise(int x, int y);
	float getNoise(int x, int y);
	float getInterpolatedNoise(float x, float y);
	float interpolate(float a, float b, float blend);

private:
	int m_Seed;
	float m_Roughness;
	int m_Octaves;
	float m_Amplitude;
};