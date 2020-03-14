gcc -c revert_string.c -o revert_string.static.o
ar rcs revert_string.a revert_string.static.o

gcc -fPIC -c revert_string.c -o revert_string.dynamic.o
gcc -shared  revert_string.dynamic.o -o revert_string.dynamic.so
