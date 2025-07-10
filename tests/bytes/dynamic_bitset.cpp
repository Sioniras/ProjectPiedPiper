///////////////////////////////////////////////////////////////////////
// Tests of the dynamic_bitset class
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <bytes/dynamic_bitset.h>

TEST(bytes_dynamic_bitset, adding_bits)
{
	bytes::dynamic_bitset bitset {};
	EXPECT_EQ(bitset.bits.size(), 0);
	EXPECT_EQ(bitset.to_uint(), 0);

	bitset.add(std::bitset<1>(0));
	EXPECT_EQ(bitset.bits.size(), 1);
	EXPECT_EQ(bitset.to_uint(), 0);

	bitset.add(std::bitset<1>(1));
	EXPECT_EQ(bitset.bits.size(), 2);
	EXPECT_EQ(bitset.to_uint(), 1);

	bitset.add(std::bitset<1>(1));
	EXPECT_EQ(bitset.bits.size(), 3);
	EXPECT_EQ(bitset.to_uint(), 3);

	bitset.add(std::bitset<4>(0b1011));
	EXPECT_EQ(bitset.bits.size(), 7);
	EXPECT_EQ(bitset.to_uint(), 0b0111011);

	bytes::dynamic_bitset bitset2 {};
	bitset2.add(std::bitset<3>(0b010));
	EXPECT_EQ(bitset2.bits.size(), 3);
	EXPECT_EQ(bitset2.to_uint(), 0b010);

	bitset.add(bitset2);
	EXPECT_EQ(bitset.bits.size(), 10);
	EXPECT_EQ(bitset.to_uint(), 0b0111011010);
}

TEST(bytes_dynamic_bitset, construct_from_bitset)
{
	bytes::dynamic_bitset bitset { std::bitset<10>(0b0011001011) };
	EXPECT_EQ(bitset.bits.size(), 10);
	EXPECT_EQ(bitset.to_uint(), 0b0011001011);
}

TEST(bytes_dynamic_bitset, construct_from_vector)
{
	std::vector<bool> v { true, true, false, true };
	bytes::dynamic_bitset bitset { v };
	EXPECT_EQ(bitset.bits.size(), 4);
	EXPECT_EQ(bitset.to_uint(), 0b1101);
}

TEST(bytes_dynamic_bitset, hash)
{
	// Differentiate on leading zeros
	bytes::dynamic_bitset bitset { std::bitset<10>(0b0011001011) };
	bytes::dynamic_bitset bitset2 { std::bitset<8>(0b11001011) };
	EXPECT_EQ(bitset.hash(), bitset.hash());
	EXPECT_EQ(bitset2.hash(), bitset2.hash());
	EXPECT_NE(bitset.hash(), bitset2.hash());
}
