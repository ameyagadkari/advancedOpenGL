#pragma once

#include <bitset>

namespace cs6610
{
	namespace UserInput
	{
		class UserInput
		{
		public:
			static bool Initialize(void);
			static std::bitset<256>keys;
			static float xRot;
			static float zRot;
		};
	}
}
