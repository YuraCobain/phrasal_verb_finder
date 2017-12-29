# Quick start

Exautable 'phrasal_verb_finder' is located under 'build' directory.
Utility to find phrasal verb according to dictonary match
Usage:
  phrasal_verb_finder [OPTION...]

  -f, --file arg  target file to search in
  -d, --dict arg  path to dictonary
  -v, --verbose   debug 
  -h, --help      print help

# Example of use

./phrasal_verb_finder --dict ../res/dict_db.txt --file ../test_file.txt

# Building with CMake

From the root of project directopry:

mkdir -p build/
cd build/
cmake ..
make


