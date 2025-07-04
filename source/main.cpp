#include <iostream>
#include <string>

#include <bytes/stream.h>
#include <utility/runsettings.h>

int main(int argc, const char** argv)
{
	utility::runsettings settings { argc, argv };
	bytes::stream::buffer_t buffer {};

	// Ensure valid parameters
	if (!settings.valid())
	{
		std::cerr << "Could not parse arguments!" << std::endl;
		exit(-1);
	}

	// Get input from stdin, i.e. pipe input
	std::string input;
	while (getline(std::cin, input, '\n'))
	{
		buffer.insert(buffer.end(), input.cbegin(), input.cend());
		if (!std::cin.eof())
			buffer.push_back('\n');
	}

	// Perform the requested operation
	auto output = settings.run(std::move(buffer));

	// Write the output
	for (auto i = output.cbegin(); i != output.cend(); i++)
		std::cout << *i;
}
