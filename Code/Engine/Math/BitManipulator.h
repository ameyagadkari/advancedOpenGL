#pragma once

#include <cstdint>

namespace cs6610
{
	namespace Math
	{
		namespace BitManipulator
		{
			bool IsBitSet(uint32_t number, uint32_t bitNumber);
			bool IsBitClear(uint32_t number, uint32_t bitNumber);
			void SetBit(uint32_t& number, uint32_t bitNumber);
			void ClearBit(uint32_t& number, uint32_t bitNumber);
		}
	}
}

#include "BitManipulator.inl"