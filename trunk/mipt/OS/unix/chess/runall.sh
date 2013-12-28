export LD_LIBRARY_PATH+=:.
./recompile_libs.sh
make
./server.o &
./client.o ololo 1 &
./client.o alala 2 &
