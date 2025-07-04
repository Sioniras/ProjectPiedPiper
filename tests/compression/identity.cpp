///////////////////////////////////////////////////////////////////////
// Tests of the identity algorithm
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <string>

#include <compression/compression.h>
#include <compression/identity.h>

TEST(algorithm_identity, compress)
{
	const std::string input = { "Hello world!" };
	auto output = compress<compression::identity>(bytes::stream::buffer_t { input.cbegin(), input.cend() });

	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(output.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(output[i], expected[i]);
}

TEST(algorithm_identity, decompress)
{
	const std::string input = { "Hello world!" };
	auto output = decompress<compression::identity>(bytes::stream::buffer_t { input.cbegin(), input.cend() });

	bytes::stream::buffer_t expected { input.cbegin(), input.cend() };
	EXPECT_EQ(output.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
		EXPECT_EQ(output[i], expected[i]);
}
