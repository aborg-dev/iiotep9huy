cd ./libs
gcc -c -fPIC string.c -o string.o
gcc -shared -fPIC -o libstring.so string.o
mv libstring.so ../
rm string.o

gcc -c -fPIC list.c -o list.o
gcc -shared -fPIC -o liblist.so list.o
mv liblist.so ../
rm list.o
