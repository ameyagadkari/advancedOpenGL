#pragma once

#include <cstdint>

namespace cs6610
{
	namespace Math
	{
		namespace BitManipulator
		{
			bool IsBitSet(uint8_t number, uint8_t bitNumber);
			bool IsBitClear(uint8_t number, uint8_t bitNumber);
			void SetBit(uint8_t& number, uint8_t bitNumber);
			void ClearBit(uint8_t& number, uint8_t bitNumber);
		}
	}
}

#include "BitManipulator.inl"