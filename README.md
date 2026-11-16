# File calculator

A C integer calculator that evaluates operations sequentially from a text file. The first line is an integer; remaining lines alternate an operator (`+`, `-`, `*`, `/`) and an integer operand. Division truncates toward zero.

## Build and run

Requires Clang or GCC and GNU Make.

```sh
make
./build/debug/calculator examples/result-27.txt
```

Prints the result, or `ERROR` with a nonzero exit status for malformed input, overflow, or division by zero. Integer values use the platform's `long` range. Lines are limited to 127 bytes.

Use `make release` for an optimized build, `make sanitize` for AddressSanitizer/UBSan, and `make clean` to remove build files. Binaries are stored in `build/<mode>/`.
