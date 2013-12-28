export LD_LIBRARY_PATH+=.
./recompile_libs.sh
make
./processes_impl.o 1 &
./processes_impl.o 2 &
