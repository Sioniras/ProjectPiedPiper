/////////////////////////////////////////////////////////////////////////
// Run Settings
//
// Collection of settings for running the application, with parsing of
// commandline arguments to change defaults.
/////////////////////////////////////////////////////////////////////////
#pragma once

#include <bytes/stream.h>

namespace utility
{
	class runsettings
	{
		public:
			struct settings
			{
				enum class mode
				{
					compress,
					decompress
				};

				enum class algorithm
				{
					identity
				};
			};

		public:
			// Constructors / destructor
			runsettings();
			runsettings(int argc, const char** argv);
			~runsettings();

			// No need for copy or move
			runsettings(const runsettings&) = delete;
			runsettings(runsettings&&) = delete;

			// Public interface
			auto mode() const { return _mode; }
			auto algorithm() const { return _algorithm; }
			bool valid() const { return _valid; }

			bytes::stream::buffer_t run(bytes::stream::buffer_t&&);

		private:
			settings::mode _mode;
			settings::algorithm _algorithm;
			bool _valid;
	};
}
