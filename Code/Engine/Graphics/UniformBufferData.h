#pragma once

namespace cs6610
{
	namespace Graphics
	{
		struct MaterialBuffer
		{
			struct Color
			{
				float r, g, b, a;
				Color(float i_r, float i_g, float i_b) :r(i_r), g(i_g), b(i_b), a(1.0f)
				{}
			}ambient, diffuse, specular;
			union
			{
				float specularExponent;
				float register16[4];
			};
			MaterialBuffer(const float*const i_ka, const float*const i_kd, const float*const i_ks, const float i_specularExponent) :
				ambient(i_ka[0], i_ka[1], i_ka[2]),
				diffuse(i_kd[0], i_kd[1], i_kd[2]),
				specular(i_ks[0], i_ks[1], i_ks[2]),
				specularExponent(i_specularExponent)
			{}
		};
	}
}