export LD_LIBRARY_PATH+=.
./recompile_libs.sh
make
./threads_impl.o &
