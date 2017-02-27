#pragma once


namespace cs6610
{
	namespace Graphics
	{
		struct Color
		{
			float r, g, b, a;
			Color(float i_r = 1.0f, float i_g = 1.0f, float i_b = 1.0f, float i_a = 1.0f) :r(i_r), g(i_g), b(i_b), a(i_a)
			{}
		};
	}
}