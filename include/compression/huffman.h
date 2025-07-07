/////////////////////////////////////////////////////////////////////////
// Huffman coding compression algorithm
//
// Uses Huffman coding to generate symbol codes.
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <compression/compression.h>

namespace compression
{
	class huffman
	{
		public:
			static bool compress(bytes::stream& input, bytes::stream& output);
			static bool decompress(bytes::stream& input, bytes::stream& output);
	};
}
