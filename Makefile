CC = gcc
CCFLAGS = -Iinc -O2

.PHONY: textdemo md5hash drmemtest clean

# build target to (build and) run the executable with a random string
textdemo: md5hash.exe
	md5hash.exe -t abcdefghijklmnopqrstuvwxyz

# build targets to make executable md5 hash command line program
md5hash: md5hash.exe
md5hash.exe: obj/md5hash.o obj/MD5.o
	$(CC) -o $@ obj/MD5.o obj/md5hash.o $(CCFLAGS)

# build targets for object files
obj/md5hash.o: src/md5hash.c inc/MD5.h inc/md5hash.h ./obj
	$(CC) -c src/md5hash.c -o $@ $(CCFLAGS)
obj/MD5.o: src/MD5.c inc/MD5.h ./obj
	$(CC) -c src/MD5.c -o $@ $(CCFLAGS)

# build target to create directory for object files
./obj:
	mkdir $@

# build target to delete all files generated while building the executable
clean:
	rmdir obj /s /q
	del md5hash.exe

# build targets to test output executable with Dr Memory
drmemtexttest: src/md5hash.c src/MD5.c
	$(CC) -static -o md5hash.exe src/md5hash.c src/MD5.c -Iinc
	drmemory md5hash.exe -t abcdefghijklmnopqrstuvwxyz

drmemfiletest: src/md5hash.c src/MD5.c
	$(CC) -static -o md5hash.exe src/md5hash.c src/MD5.c -Iinc
	drmemory md5hash.exe -f LICENSE
