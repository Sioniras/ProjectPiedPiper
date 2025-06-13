/////////////////////////////////////////////////////////////////////////
// Stream definition
//
// Note: This class is not thread-safe.
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <concepts>

#include <bytes/stream.h>

template <typename T> concept compression_algorithm = requires(T c, typename bytes::stream& arg)
{
	{ T::compress(arg, arg) } -> std::same_as<bool>;
	{ T::decompress(arg, arg) } -> std::same_as<bool>;
};

template <typename T> requires compression_algorithm<T>
auto compress(bytes::stream::buffer_t&& in) -> bytes::stream::buffer_t
{
	bytes::stream input { std::move(in) };
	bytes::stream output {};

	auto is_success = T::compress(input, output);
	assert(is_success);

	return output.buffer();
}

template <typename T> requires compression_algorithm<T>
auto decompress(bytes::stream::buffer_t&& in) -> bytes::stream::buffer_t
{
	bytes::stream input { std::move(in) };
	bytes::stream output {};

	auto is_success = T::decompress(input, output);
	assert(is_success);

	return output.buffer();
}
