output="a.out"

files=""
files=$files" main.c"
files=$files" rmlib.c"
files=$files" blocks.c"
files=$files" racecar.c"
files=$files" camera.c"
files=$files" track.c"
files=$files" profiles.c"
files=$files" demos.c"

gcc -o $output $files -lraylib