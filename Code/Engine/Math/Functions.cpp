#include "Functions.h"

const float cs6610::Math::Pi = 3.141592654f;

float cs6610::Math::ConvertDegreesToRadians(const float i_degrees)
{
	return i_degrees * Pi / 180.0f;
}

float cs6610::Math::ConvertRadiansToDegrees(const float i_radians)
{
	return i_radians * 180.0f / Pi;
}