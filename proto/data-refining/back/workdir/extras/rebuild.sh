export CPLUS_INCLUDE_PATH=/usr/include/gdal
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/hdf5/serial

g++ -std=c++11 -c -fpic main.cpp
g++ -std=c++11 -c -fpic logger.cpp
g++ -std=c++11 -o ncparse.exe main.o logger.o -lgdal -lnetcdf -lhdf5_hl -lhdf5
