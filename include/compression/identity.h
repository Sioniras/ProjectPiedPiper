/////////////////////////////////////////////////////////////////////////
// "Identity" Compression Algorithm
//
// Performs the identity operation, i.e. copies input to output.
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <bytes/stream.h>

namespace compression
{
	class identity
	{
		public:
			// Perform the identity operation
			static bool compress(bytes::stream& input, bytes::stream& output)
			{
				while (!input.at_end())
					output.put(input.read());

				return true;
			}

			// Compressions and decompression is identical here
			static bool decompress(bytes::stream& input, bytes::stream& output)
			{
				compress(input, output);
				return true;
			}
	};
}
