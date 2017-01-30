#pragma once
#include <bitset>

namespace cs6610
{
	namespace UserInput
	{
		bool Initialize(void);
		static std::bitset<256> keys;
	}
}
