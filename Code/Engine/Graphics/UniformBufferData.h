#pragma once
#include "../../External/cyCodeBase/cyMatrix.h"
#include "Color.h"

namespace cs6610
{
	namespace Graphics
	{
		namespace UniformBufferData
		{
			struct MaterialBuffer
			{
				Color ambient, diffuse, specular;
				union
				{
					float specularExponent;
					float register16[4];
				};
				union
				{
					uint32_t textureUnitMask;
					uint32_t registerTUM[4];
				};
				MaterialBuffer(const float*const i_ka, const float*const i_kd, const float*const i_ks, const float i_specularExponent, const uint32_t i_textureUnitMask) :
					ambient(i_ka[0], i_ka[1], i_ka[2]),
					diffuse(i_kd[0], i_kd[1], i_kd[2]),
					specular(i_ks[0], i_ks[1], i_ks[2]),
					specularExponent(i_specularExponent),
					textureUnitMask(i_textureUnitMask)
				{}
			};

			struct DrawcallBuffer
			{
				cyMatrix4f model;
				cyMatrix4f view;
				cyMatrix4f projection;
				cyMatrix4f lightSpaceMatrix;
				cyMatrix4f shadowMatrix;
			};
		}
	}
}
