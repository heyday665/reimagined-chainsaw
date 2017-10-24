build-clang:
	clang -o JS4_FileSystem JS4.c
build-gcc:
	gcc -o JS4_FileSystem JS4.c

run:
	./JS4_FileSystem