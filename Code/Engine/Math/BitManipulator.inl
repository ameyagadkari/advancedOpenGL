#pragma once

namespace
{
	const uint32_t bit_mask = 0x80000000;
}

inline bool cs6610::Math::BitManipulator::IsBitSet(uint32_t number, uint32_t bitNumber)
{
	return (number & (bit_mask >> bitNumber)) != 0;
}
inline bool cs6610::Math::BitManipulator::IsBitClear(uint32_t number, uint32_t bitNumber)
{
	return !IsBitSet(number, bitNumber);
}
inline void cs6610::Math::BitManipulator::SetBit(uint32_t& number, uint32_t bitNumber)
{
	number |= (bit_mask >> bitNumber);
}
inline void cs6610::Math::BitManipulator::ClearBit(uint32_t& number, uint32_t bitNumber)
{
	number &= ~(bit_mask >> bitNumber);
}