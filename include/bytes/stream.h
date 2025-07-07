/////////////////////////////////////////////////////////////////////////
// Stream definition
//
// Note: This class is not thread-safe.
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <cassert>
#include <bitset>
#include <cstdint>
#include <vector>

namespace bytes
{
	class stream
	{
		public:
			// Aliases
			using byte_t = uint8_t;
			using buffer_t = std::vector<byte_t>;

			// Constructors / destructor
			stream();
			~stream();

			// Initialize from buffer
			explicit stream(buffer_t&&);		// Take ownership of buffer
			explicit stream(const buffer_t&);	// Copy buffer

			// Copy construction/assignment
			stream(const stream&);
			stream& operator=(const stream&);

			// Move construction/assignment
			stream(stream&&);
			stream& operator=(stream&&);

			// Public interface
			inline void put_fast(byte_t byte);
			void put(byte_t byte);
			template <std::size_t n> inline void put_bits(std::bitset<n>);
			void put_bits(const struct dynamic_bitset&);

			byte_t read();
			byte_t peek() const;
			template <std::size_t n> std::bitset<n> read_bits();
			template <std::size_t n> std::bitset<n> peek_bits() const;

			void seek(std::size_t index, byte_t bitindex = 0);

			bool at_end() const { return _index >= _buffer.size(); }
			std::size_t index() const { return _index; }
			byte_t bitindex() const { return _bitindex; }

			const buffer_t& buffer() const { return _buffer; }

		private:
			buffer_t _buffer;
			std::size_t _index;	// Current byte
			byte_t _bitindex;	// Current bit within byte
	};

	// Fast inline put implementation
	void stream::put_fast(byte_t byte)
	{
		assert(_index < _buffer.size());
		assert(_bitindex == 0);

		_buffer[_index++] = byte;
	}

	// Specialization for writing 1 bit to the stream
	template <> inline void stream::put_bits(std::bitset<1> bits)
	{
		byte_t mask = (bits.test(0) ? 1 : 0) << _bitindex;

		if (_index >= _buffer.size())
		{
			// If the byte is not yet in the buffer, then it must be the first bit
			assert(_bitindex == 0);
			_buffer.push_back(mask);
		}
		else
		{
			// Clear and set the bit
			auto& byte = _buffer[_index];
			byte &= ~mask;
			byte |= mask;
		}

		// If all bits are set, go to next byte
		if (++_bitindex >= 8)
		{
			_bitindex = 0;
			++_index;
		}
	}

	// General template for writing bits to the stream
	template <std::size_t n> inline void stream::put_bits(std::bitset<n> bits)
	{
		put_bits(std::bitset<1>(bits[0]));
		put_bits(std::bitset<n-1>(bits.to_ulong() >> 1));
	}

	// Read bits from stream
	template <std::size_t n> inline auto stream::read_bits() -> std::bitset<n>
	{
		std::bitset<n> result { 0 };

		for (std::size_t i = 0; i < n; i++)
		{
			byte_t mask = 1 << _bitindex;
			auto value = (_buffer[_index] & mask) > 0;

			result.set(i, value);
			++_bitindex;
			if (_bitindex > 7)
			{
				_bitindex = 0;
				++_index;
			}
		}

		return result;
	}

	// Peek bits from stream (i.e. read without changing indices)
	template <std::size_t n> inline auto stream::peek_bits() const -> std::bitset<n>
	{
		std::bitset<n> result { 0 };
		decltype(_index) index { _index };
		decltype(_bitindex) bitindex { _bitindex };

		for (std::size_t i = 0; i < n; i++)
		{
			byte_t mask = 1 << bitindex;
			auto value = (_buffer[index] & mask) > 0;

			result.set(i, value);
			++bitindex;
			if (bitindex > 7)
			{
				bitindex = 0;
				++index;
			}
		}

		return result;
	}
}
