///////////////////////////////////////////////////////////////////////
// Tests of the stream class
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <bytes/stream.h>

TEST(bytes, put_bytes)
{
	bytes::stream::buffer_t buffer;
	bytes::stream stream { buffer };

	stream.put(0x42);
	stream.put(0x12);
	stream.put(0x82);

	EXPECT_EQ(stream.buffer()[0], 0x42);
	EXPECT_EQ(stream.buffer()[1], 0x12);
	EXPECT_EQ(stream.buffer()[2], 0x82);
}

TEST(bytes, put_bytes_fast)
{
	bytes::stream::buffer_t buffer { 0, 0, 0 };
	bytes::stream stream { buffer };

	stream.put_fast(0x42);
	stream.put_fast(0x12);
	stream.put_fast(0x82);

	EXPECT_EQ(stream.buffer()[0], 0x42);
	EXPECT_EQ(stream.buffer()[1], 0x12);
	EXPECT_EQ(stream.buffer()[2], 0x82);
}

TEST(bytes, put_bits)
{
	bytes::stream stream;

	stream.put_bits(std::bitset<32>(0xDEADBEEF));

	EXPECT_EQ(stream.buffer()[0], 0xEF);
	EXPECT_EQ(stream.buffer()[1], 0xBE);
	EXPECT_EQ(stream.buffer()[2], 0xAD);
	EXPECT_EQ(stream.buffer()[3], 0xDE);
}

TEST(bytes, put_bits_and_bytes)
{
	bytes::stream stream;

	stream.put(0x42);
	stream.put_bits(std::bitset<4>(0xC));
	stream.put(0x82);
	stream.put_bits(std::bitset<4>(0xA));
	stream.put(0x12);
	stream.put_bits(std::bitset<32>(0xDEADBEEF));
	stream.put(0xFF);

	EXPECT_EQ(stream.buffer()[0], 0x42);
	EXPECT_EQ(stream.buffer()[1], 0x2C);
	EXPECT_EQ(stream.buffer()[2], 0xA8);
	EXPECT_EQ(stream.buffer()[3], 0x12);
	EXPECT_EQ(stream.buffer()[4], 0xEF);
	EXPECT_EQ(stream.buffer()[5], 0xBE);
	EXPECT_EQ(stream.buffer()[6], 0xAD);
	EXPECT_EQ(stream.buffer()[7], 0xDE);
	EXPECT_EQ(stream.buffer()[8], 0xFF);
}

TEST(bytes, read_and_peek)
{
	bytes::stream::buffer_t buffer { 0xDE, 0xAD, 0xBE, 0xEF };
	bytes::stream stream { buffer };

	EXPECT_EQ(stream.read(), 0xDE);
	EXPECT_EQ(stream.read(), 0xAD);
	EXPECT_EQ(stream.peek(), 0xBE);
	EXPECT_EQ(stream.read(), 0xBE);
	EXPECT_EQ(stream.peek(), 0xEF);
	EXPECT_EQ(stream.read(), 0xEF);
}

TEST(bytes, read_bits)
{
	bytes::stream::buffer_t buffer { 0xDE, 0xAD, 0xEF, 0xBE };
	bytes::stream stream { buffer };

	EXPECT_EQ(stream.read_bits<4>().to_ulong(), 0xE);
	EXPECT_EQ(stream.read_bits<8>().to_ulong(), 0xDD);
	EXPECT_EQ(stream.read_bits<4>().to_ulong(), 0xA);
	EXPECT_EQ(stream.read_bits<16>().to_ulong(), 0xBEEF);
}

TEST(bytes, peek_bits)
{
	bytes::stream::buffer_t buffer { 0xEF, 0xBE, 0xAD, 0xDE };
	bytes::stream stream { buffer };

	EXPECT_EQ(stream.peek_bits<32>().to_ulong(), 0xDEADBEEF);
	EXPECT_EQ(stream.peek_bits<16>().to_ulong(), 0xBEEF);
	EXPECT_EQ(stream.peek_bits<4>().to_ulong(), 0xF);
}

TEST(bytes, read_and_peek_bits)
{
	bytes::stream::buffer_t buffer { 0xEF, 0xBE, 0xAD, 0xDE };
	bytes::stream stream { buffer };

	EXPECT_FALSE(stream.at_end());
	EXPECT_EQ(stream.peek_bits<32>().to_ulong(), 0xDEADBEEF);
	EXPECT_EQ(stream.peek_bits<16>().to_ulong(), 0xBEEF);
	EXPECT_EQ(stream.read_bits<16>().to_ulong(), 0xBEEF);
	EXPECT_EQ(stream.peek_bits<16>().to_ulong(), 0xDEAD);
	EXPECT_EQ(stream.read_bits<16>().to_ulong(), 0xDEAD);
	EXPECT_TRUE(stream.at_end());
}

TEST(bytes, seek_and_read_bits)
{
	bytes::stream::buffer_t buffer { 0xEF, 0xBE, 0xAD, 0xDE };
	bytes::stream stream { buffer };

	EXPECT_EQ(stream.peek_bits<32>().to_ulong(), 0xDEADBEEF);

	stream.seek(2, 0);
	EXPECT_EQ(stream.peek_bits<16>().to_ulong(), 0xDEAD);

	stream.seek(1, 4);
	EXPECT_EQ(stream.peek_bits<8>().to_ulong(), 0xDB);

	EXPECT_FALSE(stream.at_end());
	stream.seek(4, 0);
	EXPECT_TRUE(stream.at_end());

	stream.seek(0, 0);
	EXPECT_FALSE(stream.at_end());
	EXPECT_EQ(stream.peek_bits<32>().to_ulong(), 0xDEADBEEF);
}
