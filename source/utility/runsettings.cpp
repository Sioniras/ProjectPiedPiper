/////////////////////////////////////////////////////////////////////////
// Run settings implementation
/////////////////////////////////////////////////////////////////////////
#include <utility/runsettings.h>

#include <iostream>
#include <string>

#include <compression/compression.h>
#include <compression/identity.h>

namespace
{
	// Shortcut for calling the algorithm with the correct mode
	template <typename T> bytes::stream::buffer_t run_algorithm(utility::runsettings::settings::mode mode, bytes::stream::buffer_t&& input)
	{
		if (mode == utility::runsettings::settings::mode::decompress)
			return decompress<compression::identity>(std::move(input));

		return compress<compression::identity>(std::move(input));
	}
}

namespace utility
{
	// ----------------------------------------------------------------------
	// Default Constructor / destructor
	// ----------------------------------------------------------------------
	// Constructor
	runsettings::runsettings() :
		_mode(settings::mode::compress),
		_algorithm(settings::algorithm::identity),
		_valid(true)
	{
	}

	// Destructor
	runsettings::~runsettings()
	{
	}

	// ----------------------------------------------------------------------
	// Commandline parsing constructor
	// ----------------------------------------------------------------------
	runsettings::runsettings(int argc, const char** argv) :
		_mode(settings::mode::compress),
		_algorithm(settings::algorithm::identity),
		_valid(false)
	{
		auto isValid = true;

		for (int i = 1; i < argc; i++)
		{
			if (std::string(argv[i]).compare("-m") == 0)	// Mode
			{
				// Require the mode to be specified
				if(i+1 >= argc)
				{
					std::cerr << "Please supply a mode with the '-m' option." << std::endl;
					isValid	 = false;
					break;
				}

				auto mode = std::string(argv[++i]);
				if (mode.compare("compress") == 0)
				{
					_mode = settings::mode::compress;
				}
				else if (mode.compare("decompress") == 0)
				{
					_mode = settings::mode::decompress;
				}
				else
				{
					std::cerr << "Invalid mode \"" << mode << "\" specified for the '-m' option." << std::endl;
					isValid	 = false;
					break;
				}
			}
			else
			{
				isValid	 = false;
				break;
			}
		}

		_valid = isValid;
	}

	// ----------------------------------------------------------------------
	// Public interface
	// ----------------------------------------------------------------------
	// Run an algorithm
	bytes::stream::buffer_t runsettings::run(bytes::stream::buffer_t&& input)
	{
		switch (_algorithm)
		{
			case settings::algorithm::identity:
				return run_algorithm<compression::identity>(_mode, std::move(input));
		}
	}
}
