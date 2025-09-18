# Vars
CC = gcc
CFLAGS = -Wall -Werror -std=c99
SRC = src/assg.c src/vector.c src/spheres.c src/color.c
OBJECTS = assg.o vector.o spheres.o color.o
HEADERS = src/assg.h src/vector.h src/spheres.h src/color.h

FS_OBJECTS = FSassg.o vector.o spheres.o color.o
MS1_OBJECTS = MS1assg.o vector.o spheres.o color.o
MS2_OBJECTS = MS2assg.o vector.o spheres.o color.o



all: MS1 MS2 FS
# Executables


MS1: $(MS1_OBJECTS)
	$(CC) $(CFLAGS) $(MS1_OBJECTS) -o MS1_assg -lm
MS2:  $(MS2_OBJECTS)
	$(CC) $(CFLAGS) $(MS2_OBJECTS)  -o MS2_assg -lm

FS: $(FS_OBJECTS)
	$(CC) $(CFLAGS) $(FS_OBJECTS) -o FS_assg -lm


MS1assg.o: src/assg.c  $(HEADERS)
	$(CC) $(CFLAGS) -DMS1 -c src/assg.c -o MS1assg.o

MS2assg.o: src/assg.c  $(HEADERS)
	$(CC) $(CFLAGS) -DMS2 -c src/assg.c -o MS2assg.o

FSassg.o:src/assg.c  $(HEADERS)
	$(CC) $(CFLAGS) -DFS -c src/assg.c -o FSassg.o

assg.o: src/assg.c $(HEADERS)
	$(CC) $(CFLAGS) -c src/assg.c -o assg.o

vector.o: src/vector.c $(HEADERS)
	$(CC) $(CFLAGS) -c src/vector.c -o vector.o -c

spheres.o: src/spheres.c $(HEADERS)
	$(CC) $(CFLAGS) -c src/spheres.c -o spheres.o

color.o: src/color.c $(HEADERS)
	$(CC) $(CFLAGS) -c src/color.c -o color.o

# Clean up

clean:
	rm -f $(OBJECTS) $(FS_OBJECTS) $(MS1_OBJECTS) $(MS2_OBJECTS) MS1_assg MS2_assg FS_assg