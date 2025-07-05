/////////////////////////////////////////////////////////////////////////
// Simple Compression Algorithm
//
// Assigns shorter bitpatterns to the most frequent byte values.
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <map>
#include <algorithm>
#include <variant>
#include <compression/compression.h>

namespace
{
	struct constants
	{
		constexpr static int short_symbol_bits = 6;
		constexpr static int long_symbol_bits = 14;
	};

	constexpr int short_symbols() { return (1 << constants::short_symbol_bits) - 1; }
	constexpr int long_symbols() { return 1 << (constants::long_symbol_bits - constants::short_symbol_bits); }
	constexpr int total_symbols() { return short_symbols() + long_symbols(); }

	using short_symbol_t = std::bitset<constants::short_symbol_bits>;
	using long_symbol_t = std::bitset<constants::long_symbol_bits>;
	using symbol = std::variant<short_symbol_t, long_symbol_t>;
	using byte = bytes::stream::byte_t;
	using frequency_map = std::map<byte, std::size_t>;
	using alphabet = std::map<byte, symbol>;
	using inverse_alphabet = std::map<symbol, byte>;

	// Extracts the most frequent byte from a frequency map
	byte extract_most_frequent(frequency_map& freqs)
	{
		// Find the most frequent value
		auto comparison = [](const frequency_map::value_type& lhs, const frequency_map::value_type& rhs) { return lhs.second < rhs.second; };
		auto most_frequent = std::max_element(freqs.cbegin(), freqs.cend(), comparison);

		assert(most_frequent != freqs.cend());

		// Extract, remove and return the byte
		byte value = most_frequent->first;
		freqs.erase(most_frequent);
		return value;
	}

	// Create a symbol for a specific byte
	constexpr symbol get_symbol(std::size_t number)
	{
		if (number < short_symbols())
			return std::bitset<constants::short_symbol_bits>(number);

		// Set the first short_symbol_bits number of bits to 1 and then count from there on
		auto bitpattern = ((number - short_symbols() + 1) << constants::short_symbol_bits) | short_symbols();

		return std::bitset<constants::long_symbol_bits>(bitpattern);
	}

	// Put-operation for symbols
	struct put_symbol_operation
	{
		public:
			put_symbol_operation(bytes::stream& output) : _output(output) {}
			~put_symbol_operation() {}

			void operator()(const short_symbol_t& s) const { _output.put_bits(s); }
			void operator()(const long_symbol_t& s) const { _output.put_bits(s); }

		private:
			bytes::stream& _output;
	};

	// Read the next encoded symbol from the stream
	symbol read_symbol_from_stream(bytes::stream& input)
	{
		// Peek at the next short-symbol length bits
		auto next_short = input.peek_bits<constants::short_symbol_bits>();

		// Check whether this is a short code
		if ((next_short.to_ulong() & short_symbols()) == short_symbols())
			return input.read_bits<constants::short_symbol_bits>();

		// Otherwise read it as a long code
		return input.read_bits<constants::long_symbol_bits>();
	}
}

namespace compression
{
	class simple
	{
		public:
			// Perform the compression operation
			static bool compress(bytes::stream& input, bytes::stream& output)
			{
				static_assert(total_symbols() > 255, "There are not enough symbols available to cover all possible byte values.");

				// Obtain frequencies for each byte
				frequency_map freqs {};
				while (!input.at_end())
					++freqs[input.read()];

				// Build symbol translation table
				alphabet translator {};
				std::vector<byte> alphabet_order {};
				while (!freqs.empty())
				{
					auto next = extract_most_frequent(freqs);
					translator[next] = get_symbol(translator.size());
					alphabet_order.push_back(next);
				}

				// Reserve 3 bits for final bitindex in the output buffer
				output.put_bits(std::bitset<3> { 0 });

				// Put size of alphabet
				output.put(static_cast<byte>(translator.size()));

				// Write the bytes that are part of the alphabet in the order used to generate translator
				for (auto i = alphabet_order.cbegin(); i != alphabet_order.cend(); i++)
					output.put(*i);

				// Prepare put-operation and reset input stream position
				const put_symbol_operation put_symbol { output };
				input.seek(0);

				// Write the stream using the alphabet
				while (!input.at_end())
					std::visit(put_symbol, translator.at(input.read()));

				// Write bitindex to the first bits
				output.seek(0);
				output.put_bits(std::bitset<3> { input.bitindex() });

				return true;
			}

			// Compressions and decompression is identical here
			static bool decompress(bytes::stream& input, bytes::stream& output)
			{
				static_assert(total_symbols() > 255, "There are not enough symbols available to cover all possible byte values.");

				// Ensure that some data is available
				if (input.at_end() || input.buffer().size() < 2)
					return false;

				// Read final bitindex (3 first bits of the stream)
				byte bitindex = static_cast<byte>(input.read_bits<3>().to_ulong());

				// Get size of the alphabet
				auto alphabet_size = static_cast<std::size_t>(input.read_bits<8>().to_ulong());

				// Ensure that there are bytes enough
				if (input.buffer().size() < input.index() + alphabet_size + 1)
					return false;

				// Get the alphabet
				inverse_alphabet translator {};
				for (auto i = 0; i < alphabet_size; i++)
				{
					byte next = input.read();
					translator[get_symbol(i)] = next;
				}

				// Decompress
				auto input_size = input.buffer().size();
				while (!input.at_end())
				{
					// Get the next symbol
					symbol next = read_symbol_from_stream(input);

					// Look up the corresponding byte value
					auto decoded = translator.find(next);
					if (decoded == translator.end())
						return false;

					// Write the byte value to the output stream
					output.put(decoded->second);

					// There may be excess bits in the final byte
					if (input.index() >= input_size - 1 && input.bitindex() >= bitindex)
						break;
				}

				return true;
			}
	};
}
