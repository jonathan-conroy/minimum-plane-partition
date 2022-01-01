## Dependencies for HEAD:

On MacOS, I needed to install `llvm` (using `brew install llvm`) to enable OpenMP (a tool for parallelization required by the `-fopenmp` flag in the Makefile), as the default version of `clang++` provided by Apple does not support OpenMP. I changed the Makefile parameters `CXX` and `LINK` to point to this installation of `clang++`, and I changed the `LIBS` parameter to use  `-L/usr/local/opt/llvm/lib -lomp` to include the appropriate OpenMP libraries. If you use a different compiler, then you will likely need to update these parameters.

After these changes, running `make` successfully produced the `head` executable.

## Dependencies for parse.cpp:
Library for reading JSON files: `brew install nlohmann-json`  
Compile with `g++ -o parse -std=c++17 parse.cpp` or similar