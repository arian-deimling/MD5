CC = gcc
CCFLAGS = -Iinc -O2

main.exe: obj/main.o obj/MD5.o
	$(CC) -o $@ obj/MD5.o obj/main.o $(CCFLAGS)

obj/main.o: src/main.c inc/MD5.h ./obj
	$(CC) -c src/main.c -o $@ $(CCFLAGS)

obj/MD5.o: src/MD5.c inc/MD5.h ./obj
	$(CC) -c src/MD5.c -o $@ $(CCFLAGS)

./obj:
	mkdir $@
