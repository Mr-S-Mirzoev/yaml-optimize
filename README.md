# yaml-optimize

Yaml optimiser is a command-line tool written in C++ that optimises large YAML configurations using aliases (anchors), references, and merge keys (overrides). It leverages the rapidyaml library for YAML parsing and manipulation.

## Dependencies

- [rapidyaml](https://github.com/biojppm/rapidyaml): A fast YAML parser and emitter for C++.
- [clara](https://github.com/catchorg/Clara): A simple to use, composable command line parser for C++.
- [fmt](https://github.com/fmtlib/fmt): A formatting library providing a fast and safe alternative to C stdio and C++ iostreams.

## Aims

[ ] Provide a fast and convenient way to optimize large YAML configurations
[ ] Replace absolutely identical blocks with anchors to the first one
[ ] Replace similar blocks with merge keys
[ ] Preserve comments and spacing (?) of the original document. Also, preserve block/flow format.