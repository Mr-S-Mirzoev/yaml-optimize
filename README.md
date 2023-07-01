# yaml-optimize

Yaml optimiser is a library and set of command-line tools written in C++. The library helps optimise large YAML configurations using aliases (anchors), references, and merge keys (overrides). It leverages the rapidyaml library for YAML parsing and manipulation.

The project aims to provide a fast and convenient way to optimise large YAML configurations.

## Dependencies

- [rapidyaml](https://github.com/biojppm/rapidyaml): A fast YAML parser and emitter for C++.
- [Lyra](https://github.com/bfgroup/Lyra.git): A simple-to-use, composing, header-only, command line arguments parser for C++ 11 and beyond.
- [fmt](https://github.com/fmtlib/fmt): A formatting library providing a fast and safe alternative to C stdio and C++ iostreams.

## Roadmap

- [x] Replace identical blocks with anchors to the first one
- [ ] Replace similar blocks with merge keys
- [ ] Preserve the original document's comments and spacing (?). Also, preserve block/flow format.

## Installation

Installation is fairly easy. First of all, clone the repo, downloading its submodules recursively:

```bash
git clone https://github.com/Mr-S-Mirzoev/yaml-optimize.git --recursive
```

If you only want to test the repo features, it may be a good idea to clone only the top of the repo:

```bash
git clone https://github.com/Mr-S-Mirzoev/yaml-optimize.git --recursive --depth=1
```

Next, in the repo directory, generate the project with CMake:

```bash
cmake -B build -DYO_BUILD_TOOLS=1 -DYO_BUILD_TESTS=1 -DCMAKE_BUILD_TYPE=Release
```

Here you see the flags that can be used when building the yaml-optimize library. YO_BUILD_TOOLS and YO_BUILD_TESTS are 0 by default, as I want the build to be minimal. If you tick the YO_BUILD_TOOLS option, tools yaml-optimize, and yaml-resolve are built:

```
USAGE:
  yaml-optimize [-?|-h|--help] [-l|--limit <optimization limit>] -i|--input <input> [-o|--output <output>] [-v|--verbose]

YAML Optimizer - A tool for optimising YAML configurations with the use of anchors, references and merge keys

OPTIONS, ARGUMENTS:
  -?, -h, --help          
  -l, --limit <optimization limit>
                          limit to when to stop optimisation
  -i, --input <input>     path to input file
  -o, --output <output>   path to output file (defaults to input filename + ".optimized")
  -v, --verbose           enable verbose mode

```

```
USAGE:
  yaml-resolve [-?|-h|--help] -i|--input <input> [-o|--output <output>] [-v|--verbose]

YAML Resolver - A tool for resolving YAML optimised configurations with anchors, references, and merge keys, substituting them to obtain the complete YAML configuration

OPTIONS, ARGUMENTS:
  -?, -h, --help          
  -i, --input <input>     path to input file
  -o, --output <output>   path to output file (defaults to input filename + ".resolved")
  -v, --verbose           enable verbose mode

```

Now, we can build the project:

```bash
cmake --build build -j8 --config Debug
```
