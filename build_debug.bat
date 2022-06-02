
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=./bin -DCMAKE_VERBOSE_MAKEFILE=ON
cmake --build build --config Debug -j10 -v
cmake --install build --config Debug