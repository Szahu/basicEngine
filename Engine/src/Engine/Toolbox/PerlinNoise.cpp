#include "EGpch.h"
#include "PerlinNoise.h"

#include "glm/gtx/compatibility.hpp"

#define M_PI 3.14159265358979323846

PerlinNoise::PerlinNoise(int seed, int octaves, float amplitude, float roughness)
{
	m_Seed = seed;
	m_Octaves = octaves;
	m_Amplitude = amplitude;
	m_Roughness = roughness;
}

PerlinNoise::PerlinNoise(int octaves, float amplitude, float roughness)
{
	float rand = Engine::Random::Float();
	m_Seed = (int)(rand * 99999999);
	m_Octaves = octaves;
	m_Amplitude = amplitude;
	m_Roughness = roughness;
}

float PerlinNoise::getPerlinNoise(int x, int y)
{
	float total = 0;
	float d = (float)pow(2, m_Octaves - 1);
	for (int i = 0; i < m_Octaves; i++)
	{
		float freq = (float)(pow(2, i) / d);
		float amp = (float)pow(m_Roughness, i) * m_Amplitude;
		total += getInterpolatedNoise(x * freq, y * freq) * amp;
	}
	return total;
}

float PerlinNoise::getSmoothNoise(int x, int y)
{
	float corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1)
		+ getNoise(x + 1, y + 1)) / 16.0f;
	float sides = (getNoise(x - 1, y) + getNoise(x + 1, y) + getNoise(x, y - 1) + getNoise(x, y + 1)) / 8.0f;
	float center = getNoise(x, y) / 4.0f;
	return corners + sides + center;
}

float PerlinNoise::getNoise(int x, int y)
{
	return Engine::Random::Float() * (x * 49632 + y * 325176 + m_Seed) * 2.0f - 1.0f;
}

float PerlinNoise::getInterpolatedNoise(float x, float y)
{
	int intX = (int)x;
	float fracX = x - intX;
	int intY = (int)y;
	float fracY = y - intY;

	float v1 = getSmoothNoise(intX, intY);
	float v2 = getSmoothNoise(intX + 1, intY);
	float v3 = getSmoothNoise(intX, intY + 1);
	float v4 = getSmoothNoise(intX + 1, intY + 1);
	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracY);
}

float PerlinNoise::interpolate(float a, float b, float blend)
{
	double theta = blend * M_PI;
	float f = (float)((1.0f - cos(theta)) * 0.5f);
	return a * (1 - f) + b * f;
}
