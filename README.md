## Project Pied Piper
The project (inspired by the HBO series "Silicon Valley") makes it easy to experiment with bit-streams for developing e.g. compression algorithms.
The emphasis is on experimentation, not on performance.

The bytes/stream-class provides the essential utilities for reading or writing individual bits, and the executable target (main.cpp) provides a simple tool for running the developed algorithms, e.g.:

`cat myfile | ./p3run -m compress -a simple5 > compressed_file`

and

`cat compressed_file | ./p3run -m decompress -a simple5 > recovered_file`.

The project relies on gtest for testing the algorithms etc.
