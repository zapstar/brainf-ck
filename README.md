# Brainfuck Interpreter

My sunday evening attempt on writing an interpreter for the BF language.

## Tests

Some tests to validate the interpreter. Some taken from [here](http://www.hevanet.com/cristofd/brainfuck/tests.b), please check their individual writeups.

* [hello.b](tests/hello.b) Customary "Hello, World!" program
* [eol.b](tests/eol.b) End of line testing
* [eod.b](tests/eod.b) End of data cell testing
* [lowerbound.b](tests/lowerbound.b) Test data pointer underflow
* [upperbound.b](tests/upperbound.b) Test data pointer overflow
* [obscure.b](tests/obscure.b) Tests obscure problems
* [leftunmatch.b](tests/leftunmatch.b) Left paranthesis unmatched (without stack overflow)
* [stkoverflow.b](tests/stkoverflow.b) Stack Overflow test (left parenthesis)
* [rightunmatch.b](tests/rightunmatch.b) Right paranthesis unmatched
* [rot13.b](tests/rot13.b) ROT13 Cipher test
* [numwarp.b](tests/numwarp.b) Numwarp test [Output Screenshot](http://www.hevanet.com/cristofd/brainfuck/numwarp.png)

Language details [here](http://www.muppetlabs.com/~breadbox/bf/).
