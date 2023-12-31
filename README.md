[![MacOS](https://github.com/stewkk/nnmcpp/actions/workflows/macos.yml/badge.svg)](https://github.com/stewkk/nnmcpp/actions/workflows/macos.yml)
[![Windows](https://github.com/stewkk/nnmcpp/actions/workflows/windows.yml/badge.svg)](https://github.com/stewkk/nnmcpp/actions/workflows/windows.yml)
[![Ubuntu](https://github.com/stewkk/nnmcpp/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/stewkk/nnmcpp/actions/workflows/ubuntu.yml)
[![Style](https://github.com/stewkk/nnmcpp/actions/workflows/style.yml/badge.svg)](https://github.com/stewkk/nnmcpp/actions/workflows/style.yml)
[![codecov](https://codecov.io/gh/stewkk/nnmcpp/branch/master/graph/badge.svg?token=0APZJY8Q3J)](https://codecov.io/gh/stewkk/nnmcpp)

# Nnmcpp

Parser of topic descriptions on the NNM tracker

## Features

- Library for description parsing
- CLI with recursive filesystem traversal and serialization to CSV
- Cross-platform

## Usage

``` text
Usage:
  ./Nnmcpp [OPTION...] positional parameters

  -h, --help     Show help
  -v, --version  Print the current version number
  -d, --dry      Only print paths to .info files
```

### Download binary release for Windows
[Nnmcpp.exe](https://github.com/stewkk/nnmcpp/releases/download/v1.1.0/Nnmcpp.exe)

### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/Nnmcpp --help
```

### Build and run test suite

Use the following commands from the project's root directory to run the test suite.

```bash
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# or simply call the executable: 
./build/test/NnmcppTests
```

To collect code coverage information, run CMake with the `-DENABLE_TEST_COVERAGE=1` option.

### Run clang-format

Use the following commands from the project's root directory to check and fix C++ and CMake source style.
This requires _clang-format_, _cmake-format_ and _pyyaml_ to be installed on the current system.

```bash
cmake -S test -B build/test

# view changes
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```

See [Format.cmake](https://github.com/TheLartians/Format.cmake) for details.
These dependencies can be easily installed using pip.

```bash
pip install clang-format==14.0.6 cmake_format==0.6.11 pyyaml
```

### Build everything at once

The project also includes an `all` directory that allows building all targets at the same time.
This is useful during development, as it exposes all subprojects to your IDE and avoids redundant builds of the library.

```bash
cmake -S all -B build
cmake --build build

# run tests
./build/test/NnmcppTests
# format code
cmake --build build --target fix-format
# run standalone
./build/standalone/Nnmcpp --help
# build docs
cmake --build build --target GenerateDocs
```

### Additional tools

The test and standalone subprojects include the [tools.cmake](cmake/tools.cmake) file which is used to import additional tools on-demand through CMake configuration arguments.
The following are currently supported.

#### Sanitizers

Sanitizers can be enabled by configuring CMake with `-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined'>`.

#### Static Analyzers

Static Analyzers can be enabled by setting `-DUSE_STATIC_ANALYZER=<clang-tidy | iwyu | cppcheck>`, or a combination of those in quotation marks, separated by semicolons.
By default, analyzers will automatically find configuration files such as `.clang-format`.
Additional arguments can be passed to the analyzers by setting the `CLANG_TIDY_ARGS`, `IWYU_ARGS` or `CPPCHECK_ARGS` variables.

#### Ccache

Ccache can be enabled by configuring with `-DUSE_CCACHE=<ON | OFF>`.
