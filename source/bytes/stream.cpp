/////////////////////////////////////////////////////////////////////////
// Stream implementation
/////////////////////////////////////////////////////////////////////////
#include <bytes/stream.h>

namespace bytes
{
	// ----------------------------------------------------------------------
	// Constructors / destructor
	// ----------------------------------------------------------------------
	// Constructor
	stream::stream() : _buffer(), _index(0), _bitindex(0)
	{
	}

	// Construct from buffer (take ownership)
	stream::stream(buffer_t&& b) : _buffer(std::move(b)), _index(0), _bitindex(0)
	{
	}

	// Construct from buffer copy
	stream::stream(const buffer_t& b) : _buffer(b), _index(0), _bitindex(0)
	{
	}

	// Destructor
	stream::~stream()
	{
	}

	// ----------------------------------------------------------------------
	// Copy construction / assignment
	// ----------------------------------------------------------------------
	// Copy-construction
	stream::stream(const stream& s) :
		_buffer(s._buffer),
		_index(s._index),
		_bitindex(s._bitindex)
	{
	}

	// Copy-assignment
	stream& stream::operator=(const stream& s)
	{
		_buffer = s._buffer;
		_index = s._index;
		_bitindex = s._bitindex;
		return *this;
	}

	// ----------------------------------------------------------------------
	// Move construction / assignment
	// ----------------------------------------------------------------------
	// Move-construction
	stream::stream(stream&& s) :
		_buffer(std::move(s._buffer)),
		_index(s._index),
		_bitindex(s._bitindex)
	{
	}

	// Move-assignment
	stream& stream::operator=(stream&& s)
	{
		_buffer = std::move(s._buffer);
		_index = s._index;
		_bitindex = s._bitindex;
		return *this;
	}

	// ----------------------------------------------------------------------
	// Public interface
	// ----------------------------------------------------------------------
	// Write a byte to the stream with possible reallocation
	void stream::put(byte_t byte)
	{
		// If bitindex is not zero, add the byte as individual bits
		if(_bitindex != 0)
			return put_bits(std::bitset<8>(byte));

		// Check whether there is enough space, and make sure to allocate if needed
		if (_index < _buffer.size())
			_buffer[_index] = byte;
		else
			_buffer.push_back(byte);

		++_index;
	}

	// Change the current position within the stream
	void stream::seek(std::size_t index, byte_t bitindex)
	{
		// Note: index may be set to buffersize to point to next byte to add
		assert(index <= _buffer.size());
		assert(bitindex < 8);

		_index = index;
		_bitindex = bitindex;
	}

	// Read the next byte and move index
	auto stream::read() -> stream::byte_t
	{
		assert(_index + (_bitindex > 0 ? 1 : 0) < _buffer.size());

		// If bitindex is not zero, read the byte as individual bits
		if(_bitindex != 0)
			return static_cast<byte_t>(read_bits<8>().to_ulong() & 0xFF);


		return _buffer[_index++];
	}

	// Read the next byte without changing the index
	auto stream::peek() const -> stream::byte_t
	{
		assert(_index + (_bitindex > 0 ? 1 : 0) < _buffer.size());

		// If bitindex is not zero, read the byte as individual bits
		if(_bitindex != 0)
			return static_cast<byte_t>(peek_bits<8>().to_ulong() & 0xFF);

		return _buffer[_index];
	}
}
