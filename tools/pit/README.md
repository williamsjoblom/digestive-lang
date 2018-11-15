# `pit`
Tool for conducting regression tests.

#### Usage
`pit directory [--exit] [directory ...]`
This will run all tests with the `.dgr` extension present in given directories.

By setting the `--exit`-flag `pit` will pass tests that exits with code `0`. This is useful when testing individual compiler features.
#### Example
A simple example of a test:
```python
# This is the test header which contains metadata for the test.
# A metadata field starts with `#@`.
# There are currently two available header fields: name and description.
#@ name: pln
#@ description: Tests the built-in pln function

pln 42;
pln 43;

# This is the test footer which contains the expected stdout text.
# Every expected line of output starts with a `#$`.
# The test passes if the program outputs these expected lines.
#$ 42
#$ 43
```

Since all test information is treated as comments by the compiler these tests can also be run as regular scripts.
