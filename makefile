raymania.out : main.c rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o
	gcc -o raymania.out main.c rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o -lraylib

raymania.exe : main.c rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o
	gcc -o raymania.out main.c rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o -lraylib -lgdi32 -lwinmm

rmlib.o : rmlib.c rmlib.h
	gcc -c rmlib.c rmlib.h -lraylib
blocks.o : blocks.c blocks.h rmlib.h asset.h
	gcc -c blocks.c blocks.h -lraylib
racecar.o : racecar.c racecar.h rmlib.h blocks.h asset.h
	gcc -c racecar.c racecar.h -lraylib
camera.o : camera.c camera.h rmlib.h
	gcc -c camera.c camera.h -lraylib
track.o : track.c track.h rmlib.h blocks.h racecar.h profiles.h
	gcc -c track.c track.h -lraylib
profiles.o : profiles.c profiles.h rmlib.h racecar.h
	gcc -c profiles.c profiles.h -lraylib
demos.o : demos.c demos.h rmlib.h track.h profiles.h
	gcc -c demos.c demos.h -lraylib
asset.o : asset.c asset.h rmlib.h
	gcc -c asset.c asset.h -lraylib
ui.o : ui.c ui.h rmlib.h profiles.h asset.h
	gcc -c ui.c ui.h -lraylib

clean : 
	rm raymania.out rmlib.o blocks.o racecar.o \
	camera.o track.o profiles.o demos.o asset.o ui.o
