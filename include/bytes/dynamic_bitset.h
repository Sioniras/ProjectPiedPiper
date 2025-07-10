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
			// Constructor / destructor
			explicit dynamic_bitset() : bits({}) {}
			~dynamic_bitset() {}

			// Construct and add bits from vector
			explicit dynamic_bitset(std::vector<bool> value) : bits({})
			{
				for (auto i = value.cbegin(); i != value.cend(); i++)
					bits.push_back(std::bitset<1>(*i));
			}

			// Construct and add bits from bitset
			template <std::size_t n> explicit dynamic_bitset(std::bitset<n> value) : bits({})
			{
				add<n>(value);
			}

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
			std::size_t to_uint() const noexcept { return append_bits(0); }

			// Hash
			std::size_t hash() const
			{
				// Append bits to 1 such that "10" and "010" will yield different hashes
				return std::hash<std::size_t>{}(append_bits(1));
			}

			// Adding a bits
			template <std::size_t n> inline void add(std::bitset<n> value);

			void add(const dynamic_bitset& value)
			{
				bits.insert(bits.end(), value.bits.begin(), value.bits.end());
			}

			// The data
			std::vector<std::bitset<1>> bits;

		private:
			// Appends bits to a value
			std::size_t append_bits(std::size_t value) const
			{
				for (auto i = bits.cbegin(); i != bits.cend(); i++)
					value = (value << 1) | i->count();
				return value;
			}
	};

	// Specialization for adding a single bit
	template <> inline void dynamic_bitset::add<1>(std::bitset<1> value)
	{
		bits.push_back(value);
	}

	// General template for adding bits
	template <std::size_t n> inline void dynamic_bitset::add(std::bitset<n> value)
	{
		add(std::bitset<1>(value[n-1]));
		add(std::bitset<n-1>(value.to_ulong()));
	}
}

// Hash specialization
template <> struct std::hash<const bytes::dynamic_bitset>
{
		std::size_t operator()(const bytes::dynamic_bitset& l) const { return l.hash(); }
};
