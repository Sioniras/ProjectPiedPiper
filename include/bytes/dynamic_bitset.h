/////////////////////////////////////////////////////////////////////////
// Dynamic bitset definition
//
// The number of bits may change dynamically.
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <cassert>
#include <bitset>
#include <vector>

namespace bytes
{
	struct dynamic_bitset
	{
		public:
			// Constructors / destructor
			explicit dynamic_bitset() : bits({}) {}
			explicit dynamic_bitset(std::vector<bool> value) : bits({})
			{
				for (auto i = value.cbegin(); i != value.cend(); i++)
					bits.push_back(std::bitset<1>(*i));
			}
			~dynamic_bitset() {}

			// Copy construction / assignment
			dynamic_bitset(const dynamic_bitset& bitset) : bits(bitset.bits) {}
			dynamic_bitset& operator=(const dynamic_bitset& bitset)
			{
				bits = bitset.bits;
				return *this;
			}

			// Move construction / assignment
			dynamic_bitset(dynamic_bitset&& bitset) : bits(std::move(bitset.bits)) {}
			dynamic_bitset& operator=(dynamic_bitset&& bitset)
			{
				bits = std::move(bitset.bits);
				return *this;
			}

			// Convert to number
			std::size_t to_uint() const
			{
				std::size_t result = 0;
				for (auto i = bits.cbegin(); i != bits.cend(); i++)
					result = (result << 1) | i->count();

				return result;
			}

			// Hash
			std::size_t hash() const
			{
				return std::hash<std::size_t>{}(to_uint());
			}

			// The data
			std::vector<std::bitset<1>> bits;
	};
}
