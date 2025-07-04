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
