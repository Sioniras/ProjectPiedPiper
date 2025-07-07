///////////////////////////////////////////////////////////////////////
// Tests of the Huffman algorithm
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <string>

#include <compression/compression.h>
#include <compression/huffman.h>

TEST(algorithm_huffman, compress_decompress_text)
{
	// Arrange
	const std::string input = { "Hello world!" };

	// Act
	auto compressed = compress<compression::huffman>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::huffman>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_huffman, compress_decompress_text2)
{
	// Arrange
	const std::string input = { "Hello world! This is a long test string that is being compressed and then decompressed by the algorithm..." };

	// Act
	auto compressed = compress<compression::huffman>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::huffman>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_huffman, compress_decompress_text3)
{
	// Arrange
	const std::string input = { "The quick brown fox jumps over the lazy dog" };

	// Act
	auto compressed = compress<compression::huffman>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::huffman>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_huffman, compress_decompress_full_range)
{
	// Arrange
	std::vector<bytes::stream::byte_t> input {};
	for (auto i = 0; i <= 255; i++)
		input.push_back(static_cast<bytes::stream::byte_t>(i));

	// Act
	auto compressed = compress<compression::huffman>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::huffman>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_huffman, compress_decompress_empty_range)
{
	// Arrange
	std::vector<bytes::stream::byte_t> input {};

	// Act
	auto compressed = compress<compression::huffman>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::huffman>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}
