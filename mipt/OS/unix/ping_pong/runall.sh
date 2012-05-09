./recompile_libs.sh
./clear
make
./processes_impl.o 1 &
./processes_impl.o 2 &
