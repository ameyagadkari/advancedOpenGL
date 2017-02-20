#pragma once

namespace
{
	const uint8_t bit_mask = 0x80;
}

inline bool cs6610::Math::BitManipulator::IsBitSet(uint8_t number, uint8_t bitNumber)
{
	return (number & (bit_mask >> bitNumber)) != 0;
}
inline bool cs6610::Math::BitManipulator::IsBitClear(uint8_t number, uint8_t bitNumber)
{
	return !IsBitSet(number, bitNumber);
}
inline void cs6610::Math::BitManipulator::SetBit(uint8_t& number, uint8_t bitNumber)
{
	number |= (bit_mask >> bitNumber);
}
inline void cs6610::Math::BitManipulator::ClearBit(uint8_t& number, uint8_t bitNumber)
{
	number &= ~(bit_mask >> bitNumber);
}