cd ./libs
gcc -c -fPIC string.c -o string.o
gcc -shared -fPIC -o libstring.so string.o
mv libstring.so ../
rm string.o
