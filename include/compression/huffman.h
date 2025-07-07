/////////////////////////////////////////////////////////////////////////
// Huffman coding compression algorithm
//
// Uses Huffman coding to generate symbol codes.
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <unordered_map>
#include <algorithm>
#include <memory>

#include <compression/compression.h>
#include <bytes/dynamic_bitset.h>

namespace
{
	using byte = bytes::stream::byte_t;

	struct node
	{
		using node_ptr = std::shared_ptr<node>;

		explicit node(byte v) : value(v), first(nullptr), second(nullptr) {}
		node(node_ptr f, node_ptr s) : value(0), first(f), second(s) { assert(first != nullptr && second != nullptr); }
		~node() {}

		const byte value;
		const node_ptr first;
		const node_ptr second;

		bool is_leaf() const noexcept { return first == nullptr; }
	};
}

template <> struct std::hash<node>
{
	std::size_t operator()(const node& l) const { return std::hash<bytes::stream::byte_t>()(l.value); }
};

template <> struct std::hash<const bytes::dynamic_bitset>
{
	std::size_t operator()(const bytes::dynamic_bitset& l) const { return l.hash(); }
};

namespace compression
{
	class huffman
	{
		private:
			using frequency_node_map = std::unordered_map<node::node_ptr, std::size_t>;
			using frequency_map = std::unordered_map<byte, std::size_t>;
			using frequency_pair = std::pair<node::node_ptr, std::size_t>;
			using alphabet = std::unordered_map<byte, const bytes::dynamic_bitset>;
			using inverse_alphabet = std::unordered_map<std::size_t, byte>;

		public:
			static bool compress(bytes::stream& input, bytes::stream& output)
			{
				// Obtain frequencies for each byte
				frequency_map freqs {};
				while (!input.at_end())
					++freqs[input.read()];

				// Generate leaf nodes
				frequency_node_map freq_nodes {};
				for (auto i = freqs.cbegin(); i != freqs.cend(); i++)
					freq_nodes[std::make_shared<node>(i->first)] = i->second;

				// Build the binary tree
				while (freq_nodes.size() > 1)
				{
					auto first = extract_least_frequent(freq_nodes);
					auto second = extract_least_frequent(freq_nodes);

					auto branch = std::make_shared<node>(first.first, second.first);
					auto combined_frequency = first.second + second.second;
					freq_nodes.emplace(branch, combined_frequency);
				}

				// Build the alphabet from the tree
				alphabet translator {};
				if (freq_nodes.size() > 0)
					traverse_tree(freq_nodes.begin()->first, {}, true, translator);

				// Reserve 3 bits for final bitindex in the output buffer
				output.put_bits(std::bitset<3> { 0 });

				// Put size of alphabet. Note: Both 0 and 256 should be possible (257 possible values), so 8 bits is not enough
				output.put_bits<9>(translator.size());

				// Write the alphabet, as that has to be supplied for decompression
				for (auto i = translator.cbegin(); i != translator.cend(); i++)
				{
					assert(i->second.bits.size() < 257);	// There may only be 256 possible values, so longer codes than that should not be possible
					output.put_bits<8>(i->first);
					output.put_bits<9>(std::bitset<9>(i->second.bits.size()));
					output.put_bits(i->second);
				}

				// Reset input stream position and erite the stream using the alphabet
				input.seek(0);
				while (!input.at_end())
					output.put_bits(translator.at(input.read()));

				// Write bitindex to the first bits
				byte output_bitindex = output.bitindex();
				output.seek(0);
				output.put_bits(std::bitset<3> { output_bitindex });

				return true;
			}

			static bool decompress(bytes::stream& input, bytes::stream& output)
			{
				// Ensure that some data is available
				if (input.at_end() || input.buffer().size() < 2)
					return false;

				// Read final bitindex (3 first bits of the stream)
				byte bitindex = static_cast<byte>(input.read_bits<3>().to_ulong());

				// Get size of the alphabet
				auto alphabet_size = static_cast<std::size_t>(input.read_bits<9>().to_ulong());

				// Get the alphabet
				inverse_alphabet translator {};
				for (auto i = 0; i < alphabet_size; i++)
				{
					byte next = input.read();
					auto bitsize = input.read_bits<9>().to_ulong();
					bytes::dynamic_bitset symbol {};
					for (int k = 0; k < bitsize; k++)
						symbol.bits.push_back(input.read_bits<1>());

					translator.emplace(symbol.to_uint(), next);
				}

				// Decompress
				auto input_size = input.buffer().size();
				while (!input.at_end())
				{
					// There may be excess bits in the final byte
					if (bitindex > 0 && input.index() >= input_size - 1 && input.bitindex() >= bitindex)
						break;

					// Get the next symbol
					auto next = read_symbol_from_stream(input, translator);

					// Look up the corresponding byte value
					auto decoded = translator.find(next.to_uint());
					if (decoded == translator.end())
						return false;

					// Write the byte value to the output stream
					output.put(decoded->second);
				}

				return true;
			}

			// Extracts the least frequent byte from a frequency map
			static frequency_pair extract_least_frequent(frequency_node_map& freqs)
			{
				// Find the most frequent value
				auto comparison = [](const frequency_node_map::value_type& lhs, const frequency_node_map::value_type& rhs) { return lhs.second < rhs.second; };
				auto least_frequent = std::min_element(freqs.cbegin(), freqs.cend(), comparison);

				assert(least_frequent != freqs.cend());

				// Extract, remove and return the node
				auto value = frequency_pair(*least_frequent);
				freqs.erase(least_frequent);
				return value;
			}

			// Traverse the binary tree while building symbols
			static void traverse_tree(node::node_ptr n, std::vector<bool> code, bool next_bit, alphabet& translator)
			{
				code.push_back(next_bit);

				if (n->is_leaf())
				{
					translator.emplace(n->value, bytes::dynamic_bitset { code });
				}
				else
				{
					traverse_tree(n->first, code, true, translator);
					traverse_tree(n->second, code, false, translator);
				}
			}

			// Read the next encoded symbol from the stream
			static bytes::dynamic_bitset read_symbol_from_stream(bytes::stream& input, const inverse_alphabet& translator)
			{
				bytes::dynamic_bitset result {};

				// Keep reading bits until it matches a symbol
				while(!translator.contains(result.to_uint()) && !input.at_end())
				{
					result.bits.push_back(input.read_bits<1>());
					assert(result.bits.size() <= 256);
				}

				// Return the found code
				return result;
			}
	};
}
