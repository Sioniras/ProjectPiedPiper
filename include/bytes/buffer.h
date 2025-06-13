/////////////////////////////////////////////////////////////////////////
// Byte buffer definition
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <cstdint>

namespace bytes
{
	class buffer
	{
		public:
			// Constructors / destructor
			buffer();
			~buffer();

			// Copy construction/assignment
			buffer(const buffer&);
			buffer& operator=(const buffer&);

			// Move construction/assignment
			buffer(buffer&&);
			buffer& operator=(buffer&&);

			// Public interface
			auto& get() { return _buffer; }
			auto& get(std::size_t index) { return _buffer[index]; }
			const auto& get() const { return _buffer; }
			const auto& get(std::size_t index) const { return _buffer[index]; }
			std::size_t size() const { return _buffer.size(); }

			void reserve(std::size_t);

			// Push back with possible reallocation of buffer
			void push_back(uint8_t byte) { _buffer.push_back(byte); }

		private:
			std::vector<uint8_t> _buffer;
	};
}
