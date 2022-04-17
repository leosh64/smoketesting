# Smoke Testing

This repository contains an attempt implement smoke-testing of a CLI application. The application (`xtractor`) itself is simple:
Given input data in the form `TIMESTAMP, VALUE` and number of outputs `X`, it extracts the `TIMESTAMP`s of the `X` input data entries
with greatest `VALUE`. Input data could be supplied via file or via `stdin`.

The focus is on testing the application, both with unit tests as well as smoke-tests of the resulting binary (testing the user interface). Bazel is used
to resolve the external dependencies and provide a clean and cacheable implementation.

## General

Requirements:

* C++17

The code was tested on the following system:

* `Ubuntu 20.04.1 LTS`
* `g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0`
* `bazel 4.1.0`  (it's highly recommended to use bazel!)

## The bazel way

### Build

Bazel builds automatically before running/testing.

### Run

Run the main target and provide required arguments to the binary after the double-dash (`--`). For input via filename (path to data must be absolute):

```sh
bazel run //xtractor:main -- 3 $(pwd)/xtractor/testdata/sample_data.txt
```

For input via stdin, use `--run_under` to prepend a command:

```sh
bazel run //xtractor:main --run_under="cat $(pwd)/xtractor/testdata/sample_data.txt |" -- 3
```

### Test

Run all tests via

```sh
bazel test //...
```

Or run tests individually via

```sh
bazel test //xtractor:unit_tests
bazel test //xtractor:smoke_tests
```

## The traditional way

### Build

Build from the workspace root (the directory that contains this `README.md`) and provide the correct include path for headers:

```sh
g++ -std=c++17 -I$(pwd) xtractor/main.cpp
```

### Run

Call the compiled binary (e.g. `a.out`) with at least one parameter (size `X`). Data input must be provided either via stdin or as filename:

```sh
./a.out 3 xtractor/testdata/sample_data.txt
```

```sh
cat xtractor/testdata/sample_data.txt | ./a.out 3
```

Help is shown when running without parameters:

```sh
./a.out
```

### Test

Tests have some dependencies (e.g. [GoogleTest](https://github.com/google/googletest) for the C++ unit tests, or [assert.sh](https://github.com/torokmark/assert.sh) for the shell smoke-tests). Bazel will fetch these automatically (see above). In the traditional way, these dependencies have to be provided manually.
