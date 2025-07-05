/////////////////////////////////////////////////////////////////////////
// Run settings implementation
/////////////////////////////////////////////////////////////////////////
#include <utility/runsettings.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <optional>

#include <compression/compression.h>
#include <compression/identity.h>
#include <compression/simple.h>

namespace
{
	using algorithm_t = utility::runsettings::settings::algorithm;

	// Shortcut for calling the algorithm with the correct mode
	template <typename T> bytes::stream::buffer_t run_algorithm(utility::runsettings::settings::mode mode, bytes::stream::buffer_t&& input)
	{
		if (mode == utility::runsettings::settings::mode::decompress)
			return decompress<T>(std::move(input));

		return compress<T>(std::move(input));
	}

	// Map algorithm types to classes
	template <algorithm_t> struct algorithm_choice;
	template <> struct algorithm_choice<algorithm_t::identity> { using algorithm = typename compression::identity; };
	template <> struct algorithm_choice<algorithm_t::simple2> { using algorithm = typename compression::simple2; };
	template <> struct algorithm_choice<algorithm_t::simple3> { using algorithm = typename compression::simple3; };
	template <> struct algorithm_choice<algorithm_t::simple4> { using algorithm = typename compression::simple4; };
	template <> struct algorithm_choice<algorithm_t::simple5> { using algorithm = typename compression::simple5; };
	template <> struct algorithm_choice<algorithm_t::simple6> { using algorithm = typename compression::simple6; };
	template <> struct algorithm_choice<algorithm_t::simple7> { using algorithm = typename compression::simple7; };

	// Parsing of algorithm type
	auto algorithm_from_string(const std::string& algorithm)
	{
		std::optional<algorithm_t> result {};
		std::unordered_map<std::string, algorithm_t> algorithms {};

		algorithms["identity"] = algorithm_t::identity;
		algorithms["simple2"] = algorithm_t::simple2;
		algorithms["simple3"] = algorithm_t::simple3;
		algorithms["simple4"] = algorithm_t::simple4;
		algorithms["simple5"] = algorithm_t::simple5;
		algorithms["simple6"] = algorithm_t::simple6;
		algorithms["simple7"] = algorithm_t::simple7;

		if (algorithms.contains(algorithm))
			result = algorithms.at(algorithm);

		return result;
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
			auto value = std::string(argv[i]);
			if (value.compare("-m") == 0)	// Mode
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
			else if (value.compare("-a") == 0)
			{
				// Require the mode to be specified
				if(i+1 >= argc)
				{
					std::cerr << "Please supply an algorithm with the '-a' option." << std::endl;
					isValid	 = false;
					break;
				}

				auto algorithm_str = std::string(argv[++i]);
				auto algorithm = algorithm_from_string(algorithm_str);
				if (algorithm.has_value())
				{
					_algorithm = algorithm.value();
				}
				else
				{
					std::cerr << "Invalid algorithm \"" << algorithm_str << "\" specified for the '-a' option." << std::endl;
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
			case settings::algorithm::simple2:
				return run_algorithm<compression::simple2>(_mode, std::move(input));
			case settings::algorithm::simple3:
				return run_algorithm<compression::simple3>(_mode, std::move(input));
			case settings::algorithm::simple4:
				return run_algorithm<compression::simple4>(_mode, std::move(input));
			case settings::algorithm::simple5:
				return run_algorithm<compression::simple5>(_mode, std::move(input));
			case settings::algorithm::simple6:
				return run_algorithm<compression::simple6>(_mode, std::move(input));
			case settings::algorithm::simple7:
				return run_algorithm<compression::simple7>(_mode, std::move(input));
		}
	}
}
