#pragma once
#include "../../External/cyCodeBase/cyPoint.h"

namespace cs6610
{
	namespace ConfigurableParameters
	{
		extern float waveSpeed;
		extern float tiling;
		extern float waveStrength;
		extern float reflectivity;

		extern bool autoLight;
		extern cyPoint3f lightColor;
		void Update();
	}
}
