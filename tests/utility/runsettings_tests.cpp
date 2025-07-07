///////////////////////////////////////////////////////////////////////
// Tests of run settings module
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <utility/runsettings.h>

namespace
{
	using rs = utility::runsettings;
}

TEST(utility_runsettings, defaults)
{
	rs settings;

	EXPECT_EQ(settings.mode(), rs::settings::mode::compress);
	EXPECT_TRUE(settings.valid());
}

TEST(utility_runsettings, set_mode_compression)
{
	const int argc = 3;
	const char* argv[argc] { "p3run", "-m", "compress" };
	rs settings { argc, argv };

	EXPECT_EQ(settings.mode(), rs::settings::mode::compress);
	EXPECT_TRUE(settings.valid());
}

TEST(utility_runsettings, set_mode_decompression)
{
	const int argc = 3;
	const char* argv[argc] { "p3run", "-m", "decompress" };
	rs settings { argc, argv };

	EXPECT_EQ(settings.mode(), rs::settings::mode::decompress);
	EXPECT_TRUE(settings.valid());
}

TEST(utility_runsettings, fail_on_wrong_number_of_parameters)
{
	const int argc = 4;
	const char* argv[argc] { "p3run", "-m", "decompress", "whatever" };
	rs settings { argc, argv };

	EXPECT_FALSE(settings.valid());
}

TEST(utility_runsettings, set_algorithm_simple2)
{
	const int argc = 3;
	const char* argv[argc] { "p3run", "-a", "simple2" };
	rs settings { argc, argv };

	EXPECT_EQ(settings.algorithm(), rs::settings::algorithm::simple2);
	EXPECT_TRUE(settings.valid());
}

TEST(utility_runsettings, set_algorithm_simple3)
{
	const int argc = 3;
	const char* argv[argc] { "p3run", "-a", "simple3" };
	rs settings { argc, argv };

	EXPECT_EQ(settings.algorithm(), rs::settings::algorithm::simple3);
	EXPECT_TRUE(settings.valid());
}

TEST(utility_runsettings, set_algorithm_simple4)
{
	const int argc = 3;
	const char* argv[argc] { "p3run", "-a", "simple4" };
	rs settings { argc, argv };

	EXPECT_EQ(settings.algorithm(), rs::settings::algorithm::simple4);
	EXPECT_TRUE(settings.valid());
}

TEST(utility_runsettings, set_algorithm_simple5)
{
	const int argc = 3;
	const char* argv[argc] { "p3run", "-a", "simple5" };
	rs settings { argc, argv };

	EXPECT_EQ(settings.algorithm(), rs::settings::algorithm::simple5);
	EXPECT_TRUE(settings.valid());
}

TEST(utility_runsettings, set_algorithm_simple6)
{
	const int argc = 3;
	const char* argv[argc] { "p3run", "-a", "simple6" };
	rs settings { argc, argv };

	EXPECT_EQ(settings.algorithm(), rs::settings::algorithm::simple6);
	EXPECT_TRUE(settings.valid());
}

TEST(utility_runsettings, set_algorithm_simple7)
{
	const int argc = 3;
	const char* argv[argc] { "p3run", "-a", "simple7" };
	rs settings { argc, argv };

	EXPECT_EQ(settings.algorithm(), rs::settings::algorithm::simple7);
	EXPECT_TRUE(settings.valid());
}

TEST(utility_runsettings, set_algorithm_huffman)
{
	const int argc = 3;
	const char* argv[argc] { "p3run", "-a", "huffman" };
	rs settings { argc, argv };

	EXPECT_EQ(settings.algorithm(), rs::settings::algorithm::huffman);
	EXPECT_TRUE(settings.valid());
}
