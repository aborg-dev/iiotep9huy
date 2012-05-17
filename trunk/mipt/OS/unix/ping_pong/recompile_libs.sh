cd ./libs
gcc -c -fPIC string.c -o string.o
gcc -shared -fPIC -o libstring.so string.o
mv libstring.so ../
rm string.o

gcc -c -fPIC named_sem.c -o named_sem.o
gcc -shared -fPIC -o libnamed_sem.so named_sem.o
mv libnamed_sem.so ../
rm named_sem.o
