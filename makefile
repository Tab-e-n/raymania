raymania.out : main.c rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o
	gcc -o raymania.out main.c rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o -lraylib

raymania.exe : main.c rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o
	gcc -o raymania.out main.c rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o -lraylib -lgdi32 -lwinmm

rmlib.o : rmlib.c
	gcc -c rmlib.c -lraylib
blocks.o : blocks.c rmlib.h asset.h
	gcc -c blocks.c -lraylib
racecar.o : racecar.c rmlib.h blocks.h asset.h
	gcc -c racecar.c -lraylib
camera.o : camera.c rmlib.h
	gcc -c camera.c -lraylib
track.o : track.c rmlib.h blocks.h racecar.h profiles.h
	gcc -c track.c -lraylib
profiles.o : profiles.c rmlib.h racecar.h
	gcc -c profiles.c -lraylib
demos.o : demos.c rmlib.h track.h profiles.h
	gcc -c demos.c -lraylib
asset.o : asset.c rmlib.h
	gcc -c asset.c -lraylib
ui.o : ui.c rmlib.h profiles.h asset.h
	gcc -c ui.c -lraylib
clean : 
	rm raymania.out rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o
