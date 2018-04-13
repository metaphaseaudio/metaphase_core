# Metaphase Core Library
This library contains objects, functions, and macros used by Metaphase
projects.

### Requirements:
  - [CMake](https://cmake.org) for builds
  - The [JUCE](https://github.com/WeAreROLI/JUCE) library 
  - [Google Test](https://github.com/google/googletest) for unit testing 

### Build:
It is recommended to use the [MUDS](https://github.com/metaphaseaudio/MUDS) 
project structure when building this library.  JUCE must be built first, and
then this library can be added as a subdirectory to a top-level CMakeLists.txt
file.

To build unit tests, pass `-DBUILD_TESTS=ON` to the build script.
