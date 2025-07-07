///////////////////////////////////////////////////////////////////////
// Tests of the simple algorithm
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <string>

#include <compression/compression.h>
#include <compression/simple.h>

TEST(algorithm_simple, compress_decompress_text_7bitsymbols)
{
	// Arrange
	const std::string input = { "Hello world!" };

	// Act
	auto compressed = compress<compression::simple7>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple7>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_simple, compress_decompress_text_6bitsymbols)
{
	// Arrange
	const std::string input = { "Hello world!" };

	// Act
	auto compressed = compress<compression::simple6>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple6>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_simple, compress_decompress_text_5bitsymbols)
{
	// Arrange
	const std::string input = { "Hello world!" };

	// Act
	auto compressed = compress<compression::simple5>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple5>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_simple, compress_decompress_text_4bitsymbols)
{
	// Arrange
	const std::string input = { "Hello world!" };

	// Act
	auto compressed = compress<compression::simple4>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple4>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_simple, compress_decompress_text_3bitsymbols)
{
	// Arrange
	const std::string input = { "Hello world!" };

	// Act
	auto compressed = compress<compression::simple3>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple3>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_simple, compress_decompress_text_2bitsymbols)
{
	// Arrange
	const std::string input = { "Hello world!" };

	// Act
	auto compressed = compress<compression::simple2>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple2>(std::move(compressed));

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
	auto compressed = compress<compression::simple6>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple6>(std::move(compressed));

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
	auto compressed = compress<compression::simple6>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple6>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_simple, compress_decompress_full_range)
{
	// Arrange
	std::vector<bytes::stream::byte_t> input {};
	for (auto i = 0; i <= 255; i++)
		input.push_back(static_cast<bytes::stream::byte_t>(i));

	// Act
	auto compressed = compress<compression::simple5>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple5>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}

TEST(algorithm_simple, compress_decompress_empty_range)
{
	// Arrange
	std::vector<bytes::stream::byte_t> input {};

	// Act
	auto compressed = compress<compression::simple5>(bytes::stream::buffer_t { input.cbegin(), input.cend() });
	auto decompressed = decompress<compression::simple5>(std::move(compressed));

	// Assert
	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(decompressed.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(decompressed[i], expected[i]);
}
