#!/bin/bash

make all

# Customary Hello world program
echo "Test #1: Prints Hello world"
./bf tests/hello.b

# End of line test, prints LA
echo "Test #2: Print LA (line ending test)"
./bf tests/eol.b < tests/eol.txt

# End of data test, prints a #
echo "Test #3: Prints # at end of 30,000 cell"
./bf tests/eod.b

# Data pointer underflow test
# Print underflow + wc should output 0
echo "Test #4: Data pointer underflow test"
./bf tests/lowerbound.b | wc

# Data pointer overflow test
# Print overflow + wc should be MAX_LEN - 1
echo "Test #5: Data pointer overflow test"
./bf tests/upperbound.b | wc

# Left paranthesis unmatched test
echo "Test #6: Left paranthesis unmatched test"
./bf tests/leftunmatch.b

# Stack Overflow tests
echo "Test #7: Left paranthesis stack overflow test"
./bf tests/stkoverflow.b

# Right paranthesis unmatched test
echo "Test #8: Right paranthesis unmatched test"
./bf tests/rightunmatch.b

# ROT13 Cipher test
echo "Test #9: ROT 13 cipher test"
./bf tests/rot13.b < tests/rot13.txt

# Numwarp test
echo "Test #10: Numwarp test"
./bf tests/numwarp.b < tests/numwarp.txt

make clean
