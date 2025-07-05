///////////////////////////////////////////////////////////////////////
// Tests of the identity algorithm
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <string>

#include <compression/compression.h>
#include <compression/simple.h>

TEST(algorithm_simple, compress_decompress_text)
{
	// Arrange
	const std::string input = { "Hello world!" };

	// Act
	auto compressed = compress<compression::simple>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_simple, compress_decompress_text2)
{
	// Arrange
	const std::string input = { "Hello world! This is a long test string that is being compressed and then decompressed by the algorithm..." };

	// Act
	auto compressed = compress<compression::simple>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_simple, compress_decompress_text3)
{
	// Arrange
	const std::string input = { "The quick brown fox jumps over the lazy dog" };

	// Act
	auto compressed = compress<compression::simple>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}
